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
#include "android/Jni.h"
#include "android/ClassesCache.h"

thread_local JNIEnv* Jni::_env = nullptr;

Jni& Jni::instance()
{
    static Jni object;
    return object;
}

void Jni::storeJvm(JavaVM* jvm)
{
    Jni& jni = Jni::instance();

    jni.setJvm(jvm);
}

void Jni::storeJniEnvForThread(JNIEnv* env)
{
    Jni& jni = Jni::instance();

    jni.setJniEnv(env);
}

jclass Jni::findClass(const char* classPath)
{
    std::unique_lock<std::mutex> lock {_mutex};

    return _classes->get(classPath);
}

JNIEnv* Jni::getJniEnv()
{
    std::unique_lock<std::mutex> lock {_mutex};

    if (!_env)
    {
        JNIEnv* env;
        if (_jvm->GetEnv((void**)&env, JNI_VERSION_1_6))
            return nullptr;

        _env = env;
    }

    return _env;
}

JavaVM* Jni::getJavaVm()
{
    std::unique_lock<std::mutex> lock {_mutex};

    return _jvm;
}

Jni::~Jni()
{
    delete _classes;
}

Jni::Jni()
: _jvm(nullptr),
  _classes(nullptr)
{
}

void Jni::setJvm(JavaVM* jvm)
{
    if (jvm != _jvm)
        _jvm = jvm;

    if (!_classes)
        initClassesCache();

}
void Jni::setJniEnv(JNIEnv* env)
{
    std::unique_lock<std::mutex> lock {_mutex};

    if (env != _env)
        _env = env;
}

void Jni::initClassesCache()
{
    //Init with default classes
    //We can add more here to be loaded at startup
    _classes = new ClassesCache({"com/fogchain/radjavvm/RadJavApplication",
                                 "com/fogchain/radjavvm/UiCallback",
                                 "com/fogchain/radjavvm/UiEventListener",
                                 "com/fogchain/radjavvm/UiCallback",
                                 "com/fogchain/radjavvm/RadJavLayout",
                                 "android/widget/Button",
                                 "android/os/Handler",
                                 "android/os/Looper"});

}
