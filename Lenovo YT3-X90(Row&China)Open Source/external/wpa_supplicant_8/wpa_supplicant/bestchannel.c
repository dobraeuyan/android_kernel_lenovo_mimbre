/*
 * WPA Supplicant - best channel module
 * Copyright (c) 2013, Intel Corporation. All rights reserved.
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#include "bestchannel.h"
#include "includes.h"
#include "common.h"
#include "config.h"
#include "wpa_supplicant_i.h"
#include "driver_i.h"

int channel2index(unsigned int channel)
{
	int index = -1;

	/* 2.4 GHz channels*/
	if ((channel >= 1) && (channel <= 13)) {
			index = channel - 1;
	}

	/* 5.1 GHz channels*/
	else if ((channel >= 36) && (channel <= 64)) {
			if ((channel % 4) == 0)
				index = (channel / 4) + (NUM_CHANNELS_2_4_GHZ - 9);
	}

	/* 5.7 GHz channels*/
	else if ((channel >= 149) && (channel <= 165)) {
			if (((channel - 1) % 4) == 0)
				index = ((channel - 1) / 4) - (37 -(NUM_CHANNELS_2_4_GHZ + NUM_CHANNELS_5_1_GHZ));
	}

	return index;
}

int index2channel(int index)
{
	int channel;

	/* Channel 1..13*/
	if ((index >= 0) && (index <= (NUM_CHANNELS_2_4_GHZ - 1)))
		channel = index + 1;

	/* Channel 36..64*/
	else if ((index >= NUM_CHANNELS_2_4_GHZ) &&
			(index <= (NUM_CHANNELS_2_4_GHZ + NUM_CHANNELS_5_1_GHZ - 1)))
		channel = (index - (NUM_CHANNELS_2_4_GHZ - 9)) * 4;

	/* Channel 149..165*/
	else if ((index >= (NUM_CHANNELS_2_4_GHZ + NUM_CHANNELS_5_1_GHZ)) &&
			(index <= (NUM_CHANNELS_2_4_GHZ + NUM_CHANNELS_5_1_GHZ +
					   NUM_CHANNELS_5_7_GHZ - 1)))
		channel = ((index + (37 -
				   (NUM_CHANNELS_2_4_GHZ + NUM_CHANNELS_5_1_GHZ))) * 4) + 1;

	else
		channel = -1;

	return channel;
}


/**
 * bestOper_init - Alloc and Init the algo structure
 */
int best_channel_init(struct wpa_global *global)
{
	int i;

	global->best_channel_channels = (struct best_op_channel*) malloc(sizeof (struct best_op_channel) *
									 (NUM_CHANNELS_2_4_GHZ +
									  NUM_CHANNELS_5_1_GHZ +
									  NUM_CHANNELS_5_7_GHZ));
	if(global->best_channel_channels == NULL) {
		wpa_printf(MSG_ERROR, "best_channel: malloc failed!");
		return -1;
	}

	memset(global->best_channel_channels, 0, sizeof (struct best_op_channel) *
		(NUM_CHANNELS_2_4_GHZ +
		 NUM_CHANNELS_5_1_GHZ +
		 NUM_CHANNELS_5_7_GHZ));

	for (i = 0 ; i < (NUM_CHANNELS_2_4_GHZ + NUM_CHANNELS_5_1_GHZ + NUM_CHANNELS_5_7_GHZ); i++)
		global->best_channel_channels[i].channel_number = index2channel(i);

	global->best_chan_disable = 0;
	return 0;
}

/**
 * bestOper_uninit - Free the algo structure
 */
void best_channel_uninit(struct wpa_global *global)
{
	if(global->best_channel_channels)
		free(global->best_channel_channels);
}

/**
 * bestOper_resetAcessPoints - Reset the previously applied Access Points
 */
void best_channel_reset_ap(struct wpa_supplicant *wpa_s)
{
	int i;
	if (wpa_s->global->best_channel_channels == NULL)
		return;

	for (i = 0; i < (NUM_CHANNELS_2_4_GHZ + NUM_CHANNELS_5_1_GHZ +
		 NUM_CHANNELS_5_7_GHZ); i++)
		wpa_s->global->best_channel_channels[i].nb_of_aps = 0;
}

/**
 * best_channel_add_ap - Add a detected Access point to the list
 *      @channel: Channel number of the access point
 *      @rssi:    RSSI of the access point
 *
 * Returns: 0 on success,
 *          -1 on error
 */
int best_channel_add_ap(struct wpa_supplicant *wpa_s,
		unsigned int channel)
{
	int index = channel2index(channel);

	if (index == -1) {
		wpa_printf(MSG_ERROR,
				"Add access point on channel %d [UNSUPPORTED]", channel);
		return -1;
	}

	if (wpa_s->global->best_channel_channels == NULL)
		return -1;

	if (wpa_s->global->best_channel_channels[index].enabled != 1)
		wpa_printf(MSG_DEBUG,
				"Add access point on channel %d [BANNED]", channel);
	else
		wpa_printf(MSG_EXCESSIVE,"Add access point on channel %d", channel);

	wpa_s->global->best_channel_channels[index].nb_of_aps++;

	return 0;
}

/**
 * best_channel_auth_channel - Consider the specified channel as Authorized
 *      @channel: Channel to authorize
 *
 * Returns: 0 on success,
 *          -1 on error
 */
static int best_channel_auth_channel(struct wpa_supplicant *wpa_s,
		unsigned int channel)
{
	int index = channel2index(channel);

	if (index == -1)
		return -1;

	if (wpa_s->global->best_channel_channels == NULL)
		return -1;

	wpa_printf(MSG_EXCESSIVE, "Authorize channel %d", channel);
	wpa_s->global->best_channel_channels[index].enabled = 1;

	return 0;
}

/**
 *  best_channel_get_freq_list - Get the supported frequencies for best channel
 *
 * Returns:  0 on success
 *           -1 if failure
 */
int best_channel_get_freq_list(struct wpa_supplicant *wpa_s)
{
	struct hostapd_hw_modes *modes;
	int i, j;

	modes = wpa_s->hw.modes;
	if (modes == NULL)
		return -1;

	for (i = 0; i < wpa_s->hw.num_modes; i++) {
		for (j = 0; j < modes[i].num_channels; j++) {
			if (modes[i].channels[j].flag &
					(HOSTAPD_CHAN_DISABLED |
					 HOSTAPD_CHAN_PASSIVE_SCAN |
					 HOSTAPD_CHAN_NO_IBSS |
					 HOSTAPD_CHAN_RADAR)
					|| (modes[i].channels[j].freq >= 5260
					 && modes[i].channels[j].freq <= 5700))
				continue;
			best_channel_auth_channel(wpa_s, modes[i].channels[j].chan);
		}
	}
	return 0;
}

/**
 *  best_channel_compute - Compute the best channel to connect to
 *
 * Returns:  the best channel if success
 *           -1 if failure
 */
int best_channel_compute(struct wpa_supplicant *wpa_s)
{
	int i;
	unsigned char score;

	unsigned int best_2_4_channel = 1;
	unsigned char best_2_4_score   = 255;
	/* Best 5GHz channel set to one in case no 5GHz channels are authorized*/
	unsigned int best_5_channel   = 1;
	unsigned char best_5_score     = 255;

	unsigned int best_channel     = 1;
	unsigned int channel_num;

	if (wpa_s->global->best_channel_channels == NULL)
		return -1;
	/**
	 * Compute the best channel on 2.4GHz band
	 */
	for (i = 0; i < NUM_CHANNELS_2_4_GHZ; i++) {
		/* If the channel is not authorized, skip it */
		if (wpa_s->global->best_channel_channels[i].enabled != 1)
			continue;

		/* Ponder the scored of current channel with neighboring channels:
		 * X is the current channel and i is its index
		 * X[i] = 1*X[i] + 4*(X[i-1] + X[i+1]) + 2*(X[i-2] +X[i+2])
		 */

		channel_num = wpa_s->global->best_channel_channels[i].channel_number;

		/* Channel 1*/
		if (channel_num == 1) {
			score  = W0_CHAN * wpa_s->global->best_channel_channels[i].nb_of_aps +
					 W1_CHAN * wpa_s->global->best_channel_channels[i + 1].nb_of_aps +
					 W2_CHAN * wpa_s->global->best_channel_channels[i + 2].nb_of_aps;
		}

		/* Channel 2*/
		else if ((i > 0) &&
				 (channel_num == 2)) {
			score  = W0_CHAN *  wpa_s->global->best_channel_channels[i].nb_of_aps +
					 W1_CHAN * (wpa_s->global->best_channel_channels[i - 1].nb_of_aps +
					 wpa_s->global->best_channel_channels[i + 1].nb_of_aps) +
					 W2_CHAN *  wpa_s->global->best_channel_channels[i + 2].nb_of_aps;
		}

		/* Channel 3 to 11*/
		else if ((i > 1) &&
				 (channel_num >= 3) &&
				 (channel_num <= 11)) {
			score  = W0_CHAN *  wpa_s->global->best_channel_channels[i].nb_of_aps +
					 W1_CHAN * (wpa_s->global->best_channel_channels[i - 1].nb_of_aps +
					 wpa_s->global->best_channel_channels[i + 1].nb_of_aps) +
					 W2_CHAN * (wpa_s->global->best_channel_channels[i - 2].nb_of_aps +
					 wpa_s->global->best_channel_channels[i + 2].nb_of_aps);
		}

		/* Channel 12*/
		else if ((i > 1) &&
				 (channel_num == 12)) {
			score  = W0_CHAN *  wpa_s->global->best_channel_channels[i].nb_of_aps +
					 W1_CHAN * (wpa_s->global->best_channel_channels[i - 1].nb_of_aps +
					 wpa_s->global->best_channel_channels[i + 1].nb_of_aps) +
					 W2_CHAN *  wpa_s->global->best_channel_channels[i - 2].nb_of_aps;
		}

		/* Channel 13*/
		else if ((i > 1) &&
				(channel_num == 13)) {
			score  = W0_CHAN * wpa_s->global->best_channel_channels[i].nb_of_aps +
					 W1_CHAN * wpa_s->global->best_channel_channels[i - 1].nb_of_aps +
					 W2_CHAN * wpa_s->global->best_channel_channels[i - 2].nb_of_aps;
		} else {
			wpa_printf(MSG_ERROR, "best_channel: Internal error");
			score = 255;
		}

		if (score < best_2_4_score) {
			best_2_4_channel = channel_num;
			best_2_4_score   = score;
		}
	}

	/**
	 * Compute the best channel on 5GHz band
	 */
	for ( ; i < NUM_CHANNELS_2_4_GHZ +
	NUM_CHANNELS_5_1_GHZ +
	NUM_CHANNELS_5_7_GHZ ; i++) {
		/* If the channel is not authorized, skip it*/
		if (wpa_s->global->best_channel_channels[i].enabled != 1)
			continue;

		channel_num = wpa_s->global->best_channel_channels[i].channel_number;

		/* Ponder the scored of current channel with neighboring channels:
		 * X is the current channel and i is its index
		 * X[i] = 1*X[i] + 4*(X[i-1] +X[i+1])
		 */
		if ((i < ( NUM_CHANNELS_2_4_GHZ +  NUM_CHANNELS_5_1_GHZ +
				   NUM_CHANNELS_5_7_GHZ - 1)) &&
			((channel_num == 36)  ||
			 (channel_num == 149))) {
				score  = W0_CHAN * wpa_s->global->best_channel_channels[i].nb_of_aps +
						 W2_CHAN * wpa_s->global->best_channel_channels[i + 1].nb_of_aps;
		}

		else if ((i < ( NUM_CHANNELS_2_4_GHZ +  NUM_CHANNELS_5_1_GHZ +
						NUM_CHANNELS_5_7_GHZ - 1)) &&
				(((channel_num >= 40)  &&
				  (channel_num <= 60)) ||
				 ((channel_num >= 153) &&
				  (channel_num <= 161)))) {
			score  = W0_CHAN *  wpa_s->global->best_channel_channels[i].nb_of_aps +
					 W2_CHAN * (wpa_s->global->best_channel_channels[i - 1].nb_of_aps +
					 wpa_s->global->best_channel_channels[i + 1].nb_of_aps);
		}

		else if ((channel_num == 64 ) ||
				 (channel_num == 165)) {
			score  = W0_CHAN *  wpa_s->global->best_channel_channels[i].nb_of_aps +
					 W2_CHAN *  wpa_s->global->best_channel_channels[i - 1].nb_of_aps;
		} else {
			wpa_printf(MSG_ERROR, "best_channel: Internal error");
			score = 255;
		}

		if (score < best_5_score) {
			best_5_channel = channel_num;
			best_5_score   = score;
		}
	}

	/**
	 * Now it is time to choose the frequency band : 5 GHz or 2.4 GHz ?
	 * 5 Ghz is really less used than 2.4 GHz , hence prefer 5 GHz
	 * For that, add 2 points to the best 2.4Ghz channels
	 */
	if (best_5_score <= (best_2_4_score + 2))
		best_channel = best_5_channel;
	else
		best_channel = best_2_4_channel;

	wpa_printf(MSG_DEBUG, "Best Operating channel is = %d\n", best_channel);

	return best_channel;

}
