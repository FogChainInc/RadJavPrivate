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
#include "android/Utils.h"
#include "android/Local.h"
#include "RadJavPreprocessor.h"

namespace RadJAV
{
	namespace Android
	{
		namespace Utils
		{
			jclass FindClass(const char* classPath)
			{
				JNIEnv* env = Jni::instance().getJniEnv();

				return FindClass(env, classPath);
			}

			jclass FindClass(JNIEnv* env, const char* classPath)
			{
				auto clazz = wrap_local(env, env->FindClass(classPath));
				return static_cast<jclass>(env->NewGlobalRef(clazz));
			}

			String EnumValueToString(const char* enumClass, jobject enumeration)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = jni.getJniEnv();

				auto clazz = jni.findClass(enumClass);
				jmethodID nameMethod = env->GetMethodID(clazz, "name", "()Ljava/lang/String;");

				auto valueJava = wrap_local(env, env->CallNonvirtualObjectMethod(enumeration, clazz, nameMethod));

				return RadJAV::parseJNIString(static_cast<jstring>(valueJava.get()));
			}

			Local<_jobject> StringToEnumValue(const char* enumClass, const char* enumerationValue)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = jni.getJniEnv();

				auto clazz = jni.findClass(enumClass);

				std::string signature = "L";
				signature.append(enumClass);
				signature.append(";");

				jfieldID enumFieldId = env->GetStaticFieldID(clazz, enumerationValue, signature.c_str());
				auto ret = wrap_local(env, env->GetStaticObjectField(clazz, enumFieldId));

				return ret;
			}

			bool IsNull(jobject obj)
			{
				return Jni::instance().getJniEnv()->IsSameObject(obj, nullptr);
			}

			jobject Cast(jobject fromClass, const char* toClass)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = jni.getJniEnv();

				jclass clazz = jni.findClass("java/lang/Class");
				jmethodID forName = env->GetStaticMethodID(clazz, "forName", "(Ljava/lang/String;)Ljava/lang/Class;");

				String destName(toClass);
				auto destNameJava = jni.wrapLocalRef(destName.toJNIString());

				auto classObject = jni.wrapLocalRef(env->CallStaticObjectMethod(clazz, forName, destNameJava.get()));

				jboolean exception = env->ExceptionCheck();
				if (exception)
				{
					env->ExceptionClear();
					return nullptr;
				}

				jmethodID cast = env->GetMethodID(clazz, "cast", "(Ljava/lang/Object;)Ljava/lang/Object;");

				jobject ret = env->CallObjectMethod(classObject, cast, fromClass);

				return ret;
			}

			Local<_jobject> NewInteger(int value)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = jni.getJniEnv();

				jclass intClass = jni.findClass("java/lang/Integer");
				jmethodID intConstructor = env->GetMethodID(intClass, "<init>", "(I)V");

				return wrap_local(env, env->NewObject(intClass, intConstructor, value));
			}

			Local<_jobject> NewLong(long value)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = jni.getJniEnv();

				jclass longClass = jni.findClass("java/lang/Long");
				jmethodID longConstructor = env->GetMethodID(longClass, "<init>", "(J)V");

				return wrap_local(env, env->NewObject(longClass, longConstructor, value));
			}

			Local<_jobject> NewBoolean(bool value)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = jni.getJniEnv();

				jclass boolClass = jni.findClass("java/lang/Boolean");
				jmethodID boolConstructor = env->GetMethodID(boolClass, "<init>", "(Z)V");

				return wrap_local(env, env->NewObject(boolClass, boolConstructor, value));
			}
		}
	}
}
