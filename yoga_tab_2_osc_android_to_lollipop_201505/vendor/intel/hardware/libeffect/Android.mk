LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)
LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include
LOCAL_SRC_FILES:= effect.c test/test.c
LOCAL_MODULE := libeffect
LOCAL_SHARED_LIBRARIES:= libcutils libutils
LOCAL_MODULE_TAGS := optional
LOCAL_PRELINK_MODULE := false

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)

LOCAL_C_INCLUDES:= $(LOCAL_PATH)/include
LOCAL_SRC_FILES:= test/effect_test.c
LOCAL_MODULE := effecttest
LOCAL_SHARED_LIBRARIES:= libcutils libutils libeffect
LOCAL_MODULE_TAGS := optional

include $(BUILD_EXECUTABLE)

