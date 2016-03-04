/*
 * Copyright (C) 2014 Intel Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 or later as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 *
 */

#ifndef __THERMAL_API_SERVER__
#define __THERMAL_API_SERVER__ 1


#include <binder/IBinder.h>
#include <binder/IInterface.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <binder/ProcessState.h>

#include <utils/String16.h>

#include <vector>

using namespace android;

#define SERVICE_NAME "thermal_daemon"
#define META_INTERFACE_NAME "android.thermal.binder.IThermalAPI"

namespace thermal_api {

	enum THERMAL_API_ENUM {
		SEND_PROFILE_START=IBinder::FIRST_CALL_TRANSACTION, SEND_SENSOR_MSG,
		SEND_THERMAL_ZONE_MSG, SEND_THROTTLE_MSG, SEND_CDEV_MSG
	};

	class IThermalAPI : public IInterface
	{
		public:
		DECLARE_META_INTERFACE(ThermalAPI);
		virtual status_t sendProfileStart(int newProfile) = 0;
		virtual status_t sendSensorMsg(struct ThermalSensorMessage *smsg) = 0;
		virtual status_t sendThermalZoneMsg(struct ThermalZoneMessage *zmsg) = 0;
		virtual status_t sendThrottleMsg(struct ThrottleMessage *tmsg) = 0;
		virtual status_t sendThermalCdevInfoMsg(struct ThermalCdevInfoMessage *cmsg) = 0;
	};

	class BpThermalAPI : public BpInterface<IThermalAPI>
	{
		public:
		BpThermalAPI(const sp<IBinder>& impl );
		virtual status_t sendProfileStart(int newProfile);
		virtual status_t sendSensorMsg(struct ThermalSensorMessage *smsg);
		virtual status_t sendThermalZoneMsg(struct ThermalZoneMessage *zmsg);
		virtual status_t sendThrottleMsg(struct ThrottleMessage *tmsg);
		virtual status_t sendThermalCdevInfoMsg(struct ThermalCdevInfoMessage *cmsg);
	};

	const int MAX_CDEVS = 15;
	const int MAX_SENSORS = 10;
	const int HYST = 0;
	struct ThermalSensorMessage {
		int name_len;
		char *name;
		int path_len;
		char *path;
	};

	struct ThermalZoneMessage {
		int len;
		char *name;
		int psv;
		int max;
	};

	struct ThrottleMessage {
		int len;
		char *name;
		int val;
	};

	struct ThermalCdevInfoMessage {
		int name_len;
		char *name;
		int path_len;
		char *path;
		int nval;
		int critval;
		int step;
	};

	struct CoolingDevice {
		char *name;
		char *path;
		int nval;
		int critval;
		int step;
	};

	struct ThermalSensor {
		char *name;
		char *path;
	};

	struct ThermalZone {
		char *name;
		int psv;
		int max;
		int numSensors;
		int numCDevs;
		struct ThermalSensor sensors[MAX_SENSORS];
		struct CoolingDevice cdevs[MAX_CDEVS];
	};

	class BnThermalAPI: public BnInterface<IThermalAPI> {
		public:
		virtual status_t onTransact(uint32_t code, const Parcel& data, Parcel* reply,
				uint32_t flags = 0);
	};
	class ThermalAPI : public BnThermalAPI {
		public:
		virtual status_t sendProfileStart(int newProfile);
		virtual status_t sendSensorMsg(struct ThermalSensorMessage *msg);
		virtual status_t sendThermalZoneMsg(struct ThermalZoneMessage *zmsg);
		virtual status_t sendThrottleMsg(struct ThrottleMessage *tmsg);
		virtual status_t sendThermalCdevInfoMsg(struct ThermalCdevInfoMessage *cmsg);
	};
	// Communication with ituxd
	typedef struct {
		const char *source;
		const char *sub_string;
	} substitute_string_t;
	extern substitute_string_t substitute_strings[];
}

#endif
