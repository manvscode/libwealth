LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)


# Optional compiler flags.
LOCAL_LDLIBS   = -lm
LOCAL_CFLAGS   = -Wall -std=c99 -O2
#LOCAL_CFLAGS   = -Wall -std=c99 -g

SRC_PATH               := ../../src
LOCAL_STATIC_LIBRARIES := libcollections
LOCAL_MODULE           := wealth
#LOCAL_C_INCLUDES       := $(SRC_PATH)
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/$(SRC_PATH)
LOCAL_SRC_FILES        := \
    $(SRC_PATH)/item.c \
    $(SRC_PATH)/asset.c \
    $(SRC_PATH)/liability.c \
    $(SRC_PATH)/expense.c \
    $(SRC_PATH)/profile.c \
    $(SRC_PATH)/wealth.c

include $(BUILD_SHARED_LIBRARY)
