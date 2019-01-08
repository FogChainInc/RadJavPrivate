/*
 MIT-LICENSE
 Copyright (c) 2018 Higher Edge Software, LLC

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
 subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or substantial
 portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
 LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef _RADJAV_UTILS_H
#define _RADJAV_UTILS_H


#include "android/Jni.h"
#include <android/log.h>

#include "RadJavString.h"


#define LOG_TAG "libRadJav"

#define  LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define  LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define  LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define  LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace RadJAV
{
	namespace Android
	{
		namespace Utils
		{
			///Return global reference to java class
			jclass FindClass(const char* classPath);

			///Return global reference to java class
			jclass FindClass(JNIEnv* env, const char* classPath);

			///Return string representation of Java enum class value
			String EnumValueToString(const char* enumClass, jobject enumeration);

			//Return JNI enum object value from string representation
			Local<_jobject> StringToEnumValue(const char* enumClass, const char* enumerationValue);

			///Check if Java object is null
			bool IsNull(jobject obj);

			///Cast one java object class to another java object class if possible, use dots instead of slashes in toClass parameter
			jobject Cast(jobject fromClass, const char* toClass);

			///Create new java Integer instance with specific value
			Local<_jobject> NewInteger(int value);

			///Create new java Long instance with specific value
			Local<_jobject> NewLong(long value);

			///Create new java Boolean instance with specific value
			Local<_jobject> NewBoolean(bool value);
		}
	}
}

#endif //_RADJAV_UTILS_H
