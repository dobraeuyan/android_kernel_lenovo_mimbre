# derived from external/uefi_shim.

LOCAL_PATH := $(call my-dir)

EFITOOLS_CFLAGS :=  -fpic -Wall -fshort-wchar -fno-strict-aliasing -fno-merge-constants -mno-red-zone -fno-stack-protector -g

include $(CLEAR_VARS)

LOCAL_MODULE := sign-efi-sig-list
LOCAL_SRC_FILES := sign-efi-sig-list.c lib/guid.c
LOCAL_SHARED_LIBRARIES := libcrypto-host
LOCAL_CFLAGS := $(EFITOOLS_CFLAGS)
LOCAL_C_INCLUDES := \
    external/gnu-efi/gnu-efi-3.0/inc \
    external/gnu-efi/gnu-efi-3.0/inc/ia32 \
    external/openssl/include/ \
    $(LOCAL_PATH)/lib \
    $(LOCAL_PATH)/include

include $(BUILD_HOST_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE := cert-to-efi-sig-list
LOCAL_SRC_FILES := cert-to-efi-sig-list.c lib/guid.c
LOCAL_SHARED_LIBRARIES := libcrypto-host
LOCAL_CFLAGS := $(EFITOOLS_CFLAGS)
LOCAL_C_INCLUDES := \
    external/gnu-efi/gnu-efi-3.0/inc \
    external/gnu-efi/gnu-efi-3.0/inc/ia32 \
    external/openssl/include/ \
    $(LOCAL_PATH)/lib \
    $(LOCAL_PATH)/include

include $(BUILD_HOST_EXECUTABLE)


