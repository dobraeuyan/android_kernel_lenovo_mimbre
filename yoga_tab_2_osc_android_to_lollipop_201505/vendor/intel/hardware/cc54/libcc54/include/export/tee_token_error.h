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

#ifndef __TEE_TOKEN_ERROR_H__
#define __TEE_TOKEN_ERROR_H__

#define TEE_TOKEN_BASE_ERROR                 0xCAB1A200
#define TEE_TOKEN_NULL_PTR                   (TEE_TOKEN_BASE_ERROR + 0x01)
#define TEE_TOKEN_NOT_FOUND                  (TEE_TOKEN_BASE_ERROR + 0x02)
#define TEE_TOKEN_READ_ERROR                 (TEE_TOKEN_BASE_ERROR + 0x03)
#define TEE_TOKEN_INVALID_SIZE               (TEE_TOKEN_BASE_ERROR + 0x04)
#define TEE_TOKEN_INVALID_PARAM              (TEE_TOKEN_BASE_ERROR + 0x05)
#define TEE_TOKEN_MEMORY_ERROR               (TEE_TOKEN_BASE_ERROR + 0x06)
#define TEE_TOKEN_PSID_ERROR                 (TEE_TOKEN_BASE_ERROR + 0x07)
#define TEE_TOKEN_TRANSACTION_ERROR          (TEE_TOKEN_BASE_ERROR + 0x08)
#define TEE_TOKEN_VERIFY_ERROR               (TEE_TOKEN_BASE_ERROR + 0x09)
#define TEE_TOKEN_INVALID_TOKEN              (TEE_TOKEN_BASE_ERROR + 0x0a)
#define TEE_TOKEN_LIFETIME_ERROR             (TEE_TOKEN_BASE_ERROR + 0x0b)
#define TEE_TOKEN_VERSION_ERROR              (TEE_TOKEN_BASE_ERROR + 0x0c)
#define TEE_TOKEN_CALLBACK_ERROR             (TEE_TOKEN_BASE_ERROR + 0x0d)
#define TEE_TOKEN_DUPLICATE_TOKEN            (TEE_TOKEN_BASE_ERROR + 0x0e)
#define TEE_TOKEN_BINDING_ERROR              (TEE_TOKEN_BASE_ERROR + 0x0f)
#define TEE_TOKEN_INVALID_CONTAINER          (TEE_TOKEN_BASE_ERROR + 0x10)
// libcc-54 -specific errors
#define TEE_TOKEN_LIB_DX_INIT_ERROR          0xCAB1A220
#define TEE_TOKEN_LIB_INIT_CTX_ERROR         0xCAB1A221
#define TEE_TOKEN_LIB_SHARED_MEM_ERROR       0xCAB1A222
#endif // tee_token_error
