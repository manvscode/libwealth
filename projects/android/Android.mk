LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


# Optional compiler flags.
LOCAL_LDLIBS   = -lm
LOCAL_CFLAGS   = -Wall -pedantic -std=c99 -g

SRC_PATH               := ../../src
LOCAL_MODULE           := libwealth
LOCAL_STATIC_LIBRARIES := libcollections

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include
LOCAL_C_INCLUDES       := $(SRC_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)
LOCAL_SRC_FILES        := \
    $(SRC_PATH)/financial-item.c \
    $(SRC_PATH)/financial-profile.c \
    $(SRC_PATH)/wealth.c

include $(BUILD_SHARED_LIBRARY)
