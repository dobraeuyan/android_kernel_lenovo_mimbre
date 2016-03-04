ifeq ($(BOARD_HAVE_QCOM_FM),true)

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

ifneq ($(filter Lenovo,$(LCT_CUSTOM_NAME)),)
$(shell cp -r $(LOCAL_PATH)/custom/lenovo/res $(LOCAL_PATH)/)
else
$(shell cp -r $(LOCAL_PATH)/custom/def/res $(LOCAL_PATH)/)
endif

LOCAL_SRC_FILES := $(call all-java-files-under, src/com/codeaurora/fmradio)
LOCAL_PACKAGE_NAME := FMMidTest
LOCAL_CERTIFICATE := platform
LOCAL_JNI_SHARED_LIBRARIES := libqcomfm_jni
LOCAL_JAVA_LIBRARIES := qcom.fmradio
include $(BUILD_PACKAGE)

endif
