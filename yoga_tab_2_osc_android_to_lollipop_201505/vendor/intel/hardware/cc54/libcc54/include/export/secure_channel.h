/*************************************************************************
** Copyright (C) 2013 Intel Corporation. All rights reserved.          **
**                                                                     **
** Permission is hereby granted, free of charge, to any person         **
** obtaining a copy of this software and associated documentation      **
** files (the "Software"), to deal in the Software without             **
** restriction, including without limitation the rights to use, copy,  **
** modify, merge, publish, distribute, sublicense, and/or sell copies  **
** of the Software, and to permit persons to whom the Software is      **
** furnished to do so, subject to the following conditions:            **
**                                                                     **
** The above copyright notice and this permission notice shall be      **
** included in all copies or substantial portions of the Software.     **
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,     **
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF  **
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND               **
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS **
** BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN  **
** ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN   **
** CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE    **
** SOFTWARE.                                                           **
*************************************************************************/

#ifndef SECURE_CHANNEL_H
#define SECURE_CHANNEL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * The Type is used to free the data buffer that has been created for returning data from SEP
 */
#define SECURE_CH_DATA_FREE_RETURN_DATA   0

/*!
 * \brief secure_channel_callback
 * This function provides an interface between Modem Manager and Chaabi to facilitate the use of
 * flashless modems.
 * \param type [IN/OUT] The type of data that is being passed.  It is updated on return to be
 *  consumed by the caller
 * \param length [IN/OUT] The lenght of the data in bytes. Updated when the called party wants
 * to return data
 * \param data [IN/OUT] The data that is being passed.  If data is being returned this will
 * be updated to contain the new data.  In the case where data is returned the caller *MUST*
 * send a SECURE_CH_DATA_FREE_RETURN_DATA message once they have used the data, to enable
 * this structure to be freed correctly.  Failure to do so *will* result in a memory leak.
 * \returns 0 on SUCCESS, any other value on error.
 */
uint32_t secure_channel_callback(uint32_t *type, uint32_t *length, uint8_t **data);

#ifdef __cplusplus
}
#endif

#endif // SECURE_CHANNEL_H
