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

#ifndef TEE_DISK_ENC_H
#define TEE_DISK_ENC_H

#include <stdint.h>

/*!
 * \brief tee_create_disk_enc_key
 * Create a master key that is stored within the TEE and is protected from the REE
 * \param masterkey A key that will be used to index the actual encryption key in the TEE
 * \return 0 on success
 */
int32_t tee_create_disk_enc_key(uint8_t* masterkey);

/*!
 * \brief tee_verify_disk_enc_masterkey
 * Validate that the master key that is used as the index is the valid one as understood by the TEE
 * \param masterkey The key that is used as the index
 * \return 0 on success
 */
int32_t tee_verify_disk_enc_masterkey(uint8_t* masterkey);

#endif // TEE_DISK_ENC_H
