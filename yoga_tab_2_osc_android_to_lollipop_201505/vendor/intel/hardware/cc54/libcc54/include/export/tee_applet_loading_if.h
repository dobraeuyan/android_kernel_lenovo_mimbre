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

#ifndef __TEE_APPLET_LOADING_IF_H
#define __TEE_APPLET_LOADING_IF_H

#include "tee_client_api.h"

/*!
 * \brief tee_load_applet
 * loads a static or dynamic applet
 * \param img_buf Applet in a buffer
 * \param img_size Size of the applet
 * \return 0 on success
 * \note only supports primary vrl and that refers to one applet
 */
int32_t tee_load_applet(uint8_t *img_buf, uint32_t img_size);

/*!
 *  \brief tee_unload_applet     unloads a dynamic applet
 *  \param applet_uuid           uuid of the applet to unload
 *  \note                        valid only for dynamic applets
 */
int32_t tee_unload_applet(TEEC_UUID *applet_uuid);

#endif /* end of __TEE_APPLET_LOADING_IF_H */
