ifneq ($(filter msm8960 msm8610 msm8916 msm8909,$(TARGET_BOARD_PLATFORM)),)
# Disable temporarily for compilling error
ifneq ($(BUILD_TINY_ANDROID),true)
LOCAL_PATH := $(call my-dir)

# import st480 algorithm library
include $(CLEAR_VARS)
LOCAL_PREBUILT_LIBS := algo/common/st480_compass/libst480.a
include $(BUILD_MULTI_PREBUILT)

# HAL module implemenation stored in
include $(CLEAR_VARS)

ifneq ($(filter msm8610,$(TARGET_BOARD_PLATFORM)),)
  LOCAL_MODULE := sensors.$(TARGET_BOARD_PLATFORM)
  LOCAL_CFLAGS := -DTARGET_8610
else
  ifneq ($(filter msm8916 msm8909,$(TARGET_BOARD_PLATFORM)),)
    LOCAL_MODULE := sensors.$(TARGET_BOARD_PLATFORM)
  else
    LOCAL_MODULE := sensors.msm8930
  endif
endif

ifdef TARGET_2ND_ARCH
LOCAL_MODULE_RELATIVE_PATH := hw
else
LOCAL_MODULE_PATH := $(TARGET_OUT_SHARED_LIBRARIES)/hw
endif

LOCAL_MODULE_TAGS := optional

LOCAL_CFLAGS += -DLOG_TAG=\"Sensors\"
ifeq ($(call is-board-platform,msm8960),true)
  LOCAL_CFLAGS += -DTARGET_8930
endif

LOCAL_C_INCLUDES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_ADDITIONAL_DEPENDENCIES := $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr

LOCAL_SRC_FILES :=	\
		sensors.cpp 			\
		SensorBase.cpp			\
		LightSensor.cpp			\
		ProximitySensor.cpp		\
		CompassSensor.cpp		\
		Accelerometer.cpp				\
		Gyroscope.cpp				\
		Bmp180.cpp				\
		InputEventReader.cpp \
		CalibrationManager.cpp \
		NativeSensorManager.cpp \
		VirtualSensor.cpp	\
		sensors_XML.cpp

LOCAL_C_INCLUDES += external/libxml2/include	\

ifeq ($(call is-platform-sdk-version-at-least,20),true)
    LOCAL_C_INCLUDES += external/icu/icu4c/source/common
else
    LOCAL_C_INCLUDES += external/icu4c/common
endif

LOCAL_SHARED_LIBRARIES := liblog libcutils libdl libxml2 libutils libcalmodule_akm

include $(BUILD_SHARED_LIBRARY)

ifneq ($(LCT_HARDWARE_PLATFORM),p5000_pcb)
include $(CLEAR_VARS)

LOCAL_MODULE := libcalmodule_common
LOCAL_SRC_FILES := \
		   algo/common/common_wrapper.c \
		   algo/common/compass/AKFS_AOC.c \
		   algo/common/compass/AKFS_Device.c \
		   algo/common/compass/AKFS_Direction.c \
		   algo/common/compass/AKFS_VNorm.c

LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_STATIC_LIBRARIES := libst480
LOCAL_LDFLAGS += $(LOCAL_PATH)/algo/common/st480_compass/libst480.a
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional

ifdef TARGET_2ND_ARCH
LOCAL_MODULE_PATH_32 := $(TARGET_OUT_VENDOR)/lib
LOCAL_MODULE_PATH_64 := $(TARGET_OUT_VENDOR)/lib64
else
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)
endif

include $(BUILD_SHARED_LIBRARY)
else

include $(CLEAR_VARS)

LOCAL_MODULE := libcalmodule_common
LOCAL_SRC_FILES := \
		   algo/common/common_wrapper.c \
		   algo/common/compass/AKFS_AOC.c \
		   algo/common/compass/AKFS_Device.c \
		   algo/common/compass/AKFS_Direction.c \
		   algo/common/compass/AKFS_VNorm.c

LOCAL_SHARED_LIBRARIES := liblog libcutils
LOCAL_STATIC_LIBRARIES := libst480
LOCAL_LDFLAGS += $(LOCAL_PATH)/algo/common/st480_compass/libst480.a
LOCAL_PRELINK_MODULE := false
LOCAL_MODULE_TAGS := optional

ifdef TARGET_2ND_ARCH
LOCAL_MODULE_PATH_32 := $(TARGET_OUT_VENDOR)/lib
LOCAL_MODULE_PATH_64 := $(TARGET_OUT_VENDOR)/lib64
else
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)
endif

include $(BUILD_SHARED_LIBRARY)
## Build AKM cal module start
include $(CLEAR_VARS)

AKMD_DEVICE_TYPE=9911
# Define Device
ifeq ($(AKMD_DEVICE_TYPE), 8975)
DEVICE_STRING=8975
else ifeq ($(AKMD_DEVICE_TYPE), 8963)
DEVICE_STRING=8963
else ifeq ($(AKMD_DEVICE_TYPE), 9912)
DEVICE_STRING=09912
else ifeq ($(AKMD_DEVICE_TYPE), 9911)
DEVICE_STRING=09911
else
$(error AKMD_DEVICE_TYPE is not defined)
endif

LOCAL_PREBUILT_LIBS := libakm/libSmartCompass/libAK09911wPGplus.a
LOCAL_MODULE_TAGS := optional
include $(BUILD_MULTI_PREBUILT)

include $(CLEAR_VARS)

# Define Device
ifeq ($(AKMD_DEVICE_TYPE), 8975)
DEVICE_STRING=8975
LOCAL_CFLAGS := -DAKMD_AK8975
else ifeq ($(AKMD_DEVICE_TYPE), 8963)
DEVICE_STRING=8963
LOCAL_CFLAGS := -DAKMD_AK8963
else ifeq ($(AKMD_DEVICE_TYPE), 9912)
DEVICE_STRING=09912
LOCAL_CFLAGS := -DAKMD_AK09912
else ifeq ($(AKMD_DEVICE_TYPE), 9911)
DEVICE_STRING=09911
LOCAL_CFLAGS := -DAKMD_AK09911
else
$(error AKMD_DEVICE_TYPE is not defined)
endif


LOCAL_C_INCLUDES += \
	$(LOCAL_PATH) \
	$(LOCAL_PATH)/libakm/ \
	$(LOCAL_PATH)/libSmartCompass

LOCAL_SRC_FILES += \
	libakm/AKMD_APIs.c \
	libakm/FileIO.c \
	libakm/Measure.c \
	libakm/akm_wrapper.c

LOCAL_CFLAGS += -DAKSC_DISABLE_D6D

## Enable ALOGE
#LOCAL_CFLAGS += -UNDEBUG
#LOCAL_CFLAGS += -DENABLE_AKMDEBUG=1
#LOCAL_CFLAGS += -DAKM_LOG_ENABLE
LOCAL_CFLAGS += -DENABLE_AKM_PG
LOCAL_CFLAGS += -DAKMD_DISABLE_DOEPLUS

LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_SHARED_LIBRARIES)
LOCAL_WHOLE_STATIC_LIBRARIES += libAK09911wPGplus
LOCAL_SHARED_LIBRARIES += liblog
LOCAL_MODULE:=libcalmodule_akm
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false
include $(BUILD_SHARED_LIBRARY)
## Build AKM cal module end
endif

include $(CLEAR_VARS)

LOCAL_MODULE := calmodule.cfg
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_CLASS := ETC
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR_ETC)
LOCAL_SRC_FILES := calmodule.cfg

include $(BUILD_PREBUILT)

endif #BUILD_TINY_ANDROID
endif #TARGET_BOARD_PLATFORM
