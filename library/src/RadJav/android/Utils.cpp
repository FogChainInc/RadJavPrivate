#include "android/Utils.h"
#include "android/Local.h"

namespace utils
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

}
