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
#ifndef _JNI_H
#define _JNI_H

#include <jni.h>
#include "android/Local.h"
#include <mutex>

namespace RadJAV
{
    class ClassesCache;

    class Jni {
    public:
        static Jni& instance();

        ///Call this during JNI_OnLoad
        static void storeJvm(JavaVM* jvm);

        ///Call this for each thread function callback because JNIEnv is valid per native call
        static void storeJniEnvForThread(JNIEnv* env);

        ///Call this to get java class, classes cached and stored as global references, thread safe
        jclass findClass(const char* classPath);

        ///Call this to get JNIEnv, thread safe
        JNIEnv* getJniEnv();

        ///Call this to get JavaVM, thread safe
        JavaVM* getJavaVm();

        ///Make auto release-able local reference
        template<class T>
        Local<T> wrapLocalRef(T* reference)
        {
            return wrap_local<T>(_env, reference);
        }

    public:
        ~Jni();

        Jni(const Jni&) = delete;
        Jni(Jni&&) = delete;
        Jni& operator =(const Jni&) = delete;

    private:
        Jni();

        void setJvm(JavaVM* jvm);
        void setJniEnv(JNIEnv* env);

        void initClassesCache();

    private:
        JavaVM* _jvm;
        thread_local static JNIEnv* _env;
        ClassesCache* _classes;
        std::mutex _mutex;
    };
}

#endif //_JNI_H
