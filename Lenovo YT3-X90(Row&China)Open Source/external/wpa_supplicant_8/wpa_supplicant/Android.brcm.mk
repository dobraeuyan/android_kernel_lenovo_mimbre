#
# Copyright (C) 2008 The Android Open Source Project
#
# This software may be distributed under the terms of the BSD license.
# See README for more details.
#

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := wpa_supplicant-brcm
LOCAL_MODULE_PATH := $(PRODUCT_OUT)/$(HAL_BM_SOURCE_WIFI_PREFIX)/brcm

LOCAL_STATIC_LIBRARIES += lib_driver_cmd_bcmdhd
LOCAL_SHARED_LIBRARIES := libc libcutils liblog
ifdef CONFIG_EAP_PROXY
LOCAL_STATIC_LIBRARIES += $(LIB_STATIC_EAP_PROXY)
LOCAL_SHARED_LIBRARIES += $(LIB_SHARED_EAP_PROXY)
endif
ifeq ($(CONFIG_TLS), openssl)
LOCAL_SHARED_LIBRARIES += libcrypto libssl libkeystore_binder
endif
ifneq ($(wildcard external/libnl),)
LOCAL_SHARED_LIBRARIES += libnl
else
LOCAL_STATIC_LIBRARIES += libnl_2
endif

LOCAL_CFLAGS := $(L_CFLAGS)
LOCAL_SRC_FILES := $(OBJS)
LOCAL_C_INCLUDES := $(INCLUDES)

include $(BUILD_EXECUTABLE)

WPA_SUPPLICANT_AUTODETECT_BRCM:
	$(hide) mkdir -p $(PRODUCT_OUT)/system/bin
	$(hide) mkdir -p $(PRODUCT_OUT)/system/lib
	$(hide) mkdir -p $(PRODUCT_OUT)/$(HAL_BM_SOURCE_WIFI_PREFIX)/brcm/lib
	$(hide) ln -sf $(HAL_BM_SOURCE_WIFI_PREFIX)/aosp/wpa_cli $(PRODUCT_OUT)/$(HAL_BM_SOURCE_WIFI_PREFIX)/brcm/wpa_cli
	$(hide) ln -sf wpa_supplicant-brcm $(PRODUCT_OUT)/$(HAL_BM_SOURCE_WIFI_PREFIX)/brcm/wpa_supplicant
	$(hide) ln -sf $(HAL_BM_SOURCE_WIFI_PREFIX)/aosp/lib/libwpa_client.so $(PRODUCT_OUT)/$(HAL_BM_SOURCE_WIFI_PREFIX)/brcm/lib/libwpa_client.so
ifeq (x86_64, $(TARGET_ARCH))
	$(hide) mkdir -p $(PRODUCT_OUT)/$(HAL_BM_SOURCE_WIFI_PREFIX)/brcm/lib64
	$(hide) ln -sf $(HAL_BM_SOURCE_WIFI_PREFIX)/aosp/lib64/libwpa_client.so $(PRODUCT_OUT)/$(HAL_BM_SOURCE_WIFI_PREFIX)/brcm/lib64/libwpa_client.so
endif

ALL_DEFAULT_INSTALLED_MODULES += WPA_SUPPLICANT_AUTODETECT_BRCM
