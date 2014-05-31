ifeq ($(USE_ULP_AUDIO),true)

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

# Set up the OpenCore variables.
#include frameworks/base/media/libstagefright/codecs/common/Config.mk
LOCAL_C_INCLUDES := \
    $(TOP)/device/samsung/sec_mm/sec_codecs/s5pc210/audio/srp/include \
    $(TOP)/frameworks/base/include/media/stagefright/openmax \
    $(TOP)/frameworks/base/media/libstagefright

LOCAL_SRC_FILES := \
    ULPAudioPlayer.cpp

# do not prelink
LOCAL_PRELINK_MODULE := false

LOCAL_MODULE := libaudiohw

LOCAL_SHARED_LIBRARIES := \
    libbinder \
    libutils \
    libcutils \
    libui \
    libhardware \
    libandroid_runtime \
    libmedia \
    libicuuc \
    libstagefright

LOCAL_STATIC_LIBRARIES := libsrpapi

LOCAL_LDLIBS +=

LOCAL_MODULE_TAGS := optional

include $(BUILD_SHARED_LIBRARY)

endif
