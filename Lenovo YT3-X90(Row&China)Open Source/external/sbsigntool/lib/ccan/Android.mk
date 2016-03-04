LOCAL_PATH:= $(call my-dir)


libccan_a_cflags := \
    -Wall -Wextra  --std=gnu99  \
    -pipe -fno-strict-aliasing -Wno-sign-compare \
    -Wno-return-type -Wno-missing-field-initializers \
    -Wno-unused-parameter \

libccan_a_includes = \
    $(LOCAL_PATH)/ \
    $(LOCAL_PATH)/../../ \

libccan_a_SOURCES = \
    ccan/time/time.c \
    ccan/talloc/talloc.c \
    ccan/str/str.c \
    ccan/str/debug.c \
    ccan/read_write_all/read_write_all.c \
    ccan/list/list.c \
    ccan/htable/htable.c \
    ccan/hash/hash.c \
    ccan/failtest/failtest.c \


##
# sbsign
#
include $(CLEAR_VARS)

LOCAL_MODULE := libccan
LOCAL_MODULE_TAGS := optional
LOCAL_C_INCLUDES := $(libccan_a_includes)
LOCAL_CFLAGS := $(libccan_a_cflags)
LOCAL_SRC_FILES := $(libccan_a_SOURCES)

include $(BUILD_HOST_STATIC_LIBRARY)

