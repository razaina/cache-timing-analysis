
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
#LOCAL_MODULE    := libaestest
#LOCAL_SRC_FILES := cta.cpp
#LOCAL_LDLIBS := -llog
#include $(BUILD_SHARED_LIBRARY)

LOCAL_MODULE    := ctatest
LOCAL_SRC_FILES := rijndael-alg-fst.c rijndael-api-fst.c ctatest.c v7_pmu.S
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -g
include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)
LOCAL_MODULE    := bench
LOCAL_SRC_FILES := bench.c v7_pmu.S
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -g -O0
include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_MODULE    := correlate
#LOCAL_SRC_FILES := correlate.c 
#LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS := -g
#include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := study
LOCAL_SRC_FILES := study.c 
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -g
include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_MODULE    := ciphertext
#LOCAL_SRC_FILES := ciphertext.c 
#LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS := -g
#include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_MODULE    := corr
#LOCAL_SRC_FILES := corr.c 
#LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS := -g
#include $(BUILD_EXECUTABLE)


include $(CLEAR_VARS)
LOCAL_MODULE := libssl-prebuilt
LOCAL_SRC_FILES := /usr/local/ssl/android-19/lib/libssl.a 
LOCAL_EXPORT_C_INCLUDES := /usr/local/ssl/android-19/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libcrypto-prebuilt
LOCAL_SRC_FILES := /usr/local/ssl/android-19/lib/libcrypto.a
LOCAL_EXPORT_C_INCLUDES := /usr/local/ssl/android-19/include
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE    := server 
LOCAL_SRC_FILES := server.c v7_pmu.S 
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -g
LOCAL_C_INCLUDES := /usr/local/ssl/android-19/include
LOCAL_LDLIBS := -L/usr/local/ssl/android-19/lib -llog
LOCAL_STATIC_LIBRARIES := libssl-prebuilt libcrypto-prebuilt
include $(BUILD_EXECUTABLE)

#include $(CLEAR_VARS)
#LOCAL_MODULE    := server-flush 
#LOCAL_SRC_FILES := server-flush.c 
#LOCAL_ARM_MODE := arm
#LOCAL_CFLAGS := -g
#LOCAL_C_INCLUDES := /usr/local/ssl/android-19/include
#LOCAL_LDLIBS := -L/usr/local/ssl/android-19/lib -llog
#LOCAL_STATIC_LIBRARIES := libssl-prebuilt libcrypto-prebuilt
#include $(BUILD_EXECUTABLE)

include $(CLEAR_VARS)
LOCAL_MODULE    := search
LOCAL_SRC_FILES := search.c 
LOCAL_ARM_MODE := arm
LOCAL_CFLAGS := -g
LOCAL_C_INCLUDES := /usr/local/ssl/android-19/include
LOCAL_LDLIBS := -L/usr/local/ssl/android-19/lib -llog
LOCAL_STATIC_LIBRARIES := libssl-prebuilt libcrypto-prebuilt
include $(BUILD_EXECUTABLE)
