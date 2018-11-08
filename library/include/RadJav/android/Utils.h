#ifndef RADJAVVM_UTILS_H
#define RADJAVVM_UTILS_H


#include "android/Jni.h"
#include <android/log.h>

#define LOG_TAG "libRadJav"

#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace utils
{
    ///Return global reference to java class
    jclass FindClass(const char* classPath);

    ///Return global reference to java class
    jclass FindClass(JNIEnv* env, const char* classPath);
}

#endif //RADJAVVM_UTILS_H
