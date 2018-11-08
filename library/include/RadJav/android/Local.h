#ifndef _LOCAL_H
#define _LOCAL_H

#include <jni.h>
#include <memory>

template <class T>
class Local : public std::shared_ptr<T>
{
public:
    Local(JNIEnv* env, T* reference)
            : std::shared_ptr<T>(reference, [env](T* p){env->DeleteLocalRef(p);})
    {
        static_assert(std::is_base_of<_jobject, T>::value, "Type provided is not a JNI reference type");
    }

    operator T*()
    {
        return std::shared_ptr<T>::get();
    }
};

template <class T>
Local<T> wrap_local(JNIEnv* env, T* reference)
{
    return Local<T>(env, reference);
}

#endif //_LOCAL_H
