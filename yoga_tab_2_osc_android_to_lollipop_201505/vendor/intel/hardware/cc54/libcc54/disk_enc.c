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

#include "tee_client_api.h"
#include "dx_cclib.h"
#include <string.h>

#define DISK_ENC_APPLET_UUID "INTEL DISK ENC01"
#define DISK_ENC_CMD_CREATE_NEW_HMAC_KEY 1
#define DISK_ENC_CMD_HMAC_IKEY 2
#define DISK_ENC_CMD_RUN_UNIT_TESTS 3
#define DISK_ENC_CMD_IMPORT_KEY 4
#define DISK_ENC_CMD_CREATE_KEY 5
#define DISK_ENC_CMD_VERIFY_MASTERKEY 6
#define DISK_ENC_CMD_DO_CRYPTO 7

#define MASTERKEY_SIZE 16

int32_t tee_create_disk_enc_key(uint8_t* masterkey)
{
    TEEC_Context context;
    TEEC_SharedMemory sharedMem;
    TEEC_Session session;
    TEEC_Operation operation;
    static const uint8_t destination[] = DISK_ENC_APPLET_UUID;
    TEEC_Result ret;
    uint32_t returnOrigin = 0;

    if (NULL == masterkey)
        return TEEC_ERROR_NO_DATA;

    ret = DX_CclibInit();
    if (TEEC_SUCCESS != ret )
        return ret;

    ret = TEEC_InitializeContext(NULL, /* default TEE */
                                 &context);
    if (TEEC_SUCCESS != ret )
        goto CclibFini;

    ret = TEEC_OpenSession(&context,
                           &session,
                           (TEEC_UUID*)&destination,
                           TEEC_LOGIN_PUBLIC,
                           NULL,
                           NULL,
                           &returnOrigin);
    if (TEEC_SUCCESS != ret )
        goto FinalizeContext;

    sharedMem.buffer = (void *)masterkey;
    sharedMem.size = MASTERKEY_SIZE;
    sharedMem.flags = TEEC_MEM_INPUT;

    ret = TEEC_RegisterSharedMemory(&context,
                                    &sharedMem);
    if (TEEC_SUCCESS != ret )
        goto CloseSession;

    memset(&operation, 0, sizeof(operation));
    TEEC_PARAM_TYPE_SET(operation.paramTypes,0,TEEC_MEMREF_WHOLE);
    operation.params[0].memref.parent = &sharedMem;

    ret = TEEC_InvokeCommand(&session,
                             DISK_ENC_CMD_CREATE_KEY,
                             &operation,
                             &returnOrigin);

    TEEC_ReleaseSharedMemory (&sharedMem);
CloseSession:
    TEEC_CloseSession (&session);
FinalizeContext:
    TEEC_FinalizeContext(&context);
CclibFini:
    DX_CclibFini();

    return ret;
}

int32_t tee_verify_disk_enc_masterkey(uint8_t* masterkey)
{
    TEEC_Context context;
    TEEC_SharedMemory sharedMem;
    TEEC_Session session;
    TEEC_Operation operation;
    static const uint8_t destination[] = DISK_ENC_APPLET_UUID;
    TEEC_Result ret;
    uint32_t returnOrigin = 0;

    if (NULL == masterkey)
        return TEEC_ERROR_NO_DATA;

    ret = DX_CclibInit();
    if (TEEC_SUCCESS != ret )
        return ret;

    ret = TEEC_InitializeContext(NULL, /* default TEE */
                                 &context);
    if (TEEC_SUCCESS != ret )
        goto CclibFini;

    ret = TEEC_OpenSession(&context,
                           &session,
                           (TEEC_UUID*)&destination,
                           TEEC_LOGIN_PUBLIC,
                           NULL,
                           NULL,
                           &returnOrigin);
    if (TEEC_SUCCESS != ret )
        goto FinalizeContext;

    sharedMem.buffer = (void *)masterkey;
    sharedMem.size = MASTERKEY_SIZE;
    sharedMem.flags = TEEC_MEM_INPUT;

    ret = TEEC_RegisterSharedMemory(&context,
                                    &sharedMem);
    if (TEEC_SUCCESS != ret )
        goto CloseSession;

    memset(&operation, 0, sizeof(operation));
    TEEC_PARAM_TYPE_SET(operation.paramTypes,0,TEEC_MEMREF_WHOLE);
    operation.params[0].memref.parent = &sharedMem;

    ret = TEEC_InvokeCommand(&session,
                             DISK_ENC_CMD_VERIFY_MASTERKEY,
                             &operation,
                             &returnOrigin);

    TEEC_ReleaseSharedMemory (&sharedMem);
CloseSession:
    TEEC_CloseSession (&session);
FinalizeContext:
    TEEC_FinalizeContext(&context);
CclibFini:
    DX_CclibFini();

    return ret;
}
