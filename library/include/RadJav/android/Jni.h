#ifndef _JNI_H
#define _JNI_H

#include <jni.h>
#include "android/Local.h"
#include <mutex>

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


#endif //_JNI_H
