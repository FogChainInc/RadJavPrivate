#include "android/ClassesCache.h"
#include "android/Utils.h"

ClassesCache::ClassesCache()
{
}

ClassesCache::ClassesCache(std::initializer_list<const char*> preloadClasses)
{
    JNIEnv* env = Jni::instance().getJniEnv();

    for (auto classPath : preloadClasses)
    {
        auto clazz = utils::FindClass(classPath);

        _classes[classPath] = clazz;
    }
}

ClassesCache::~ClassesCache()
{
    JNIEnv* env = Jni::instance().getJniEnv();

    for (auto& clazz : _classes)
    {
        env->DeleteGlobalRef(clazz.second);
    }
}

jclass ClassesCache::get(const char* classPath)
{
    auto it = _classes.find(classPath);

    if ( it != _classes.end())
    {
        return it->second;
    } else{
        jclass clazz = utils::FindClass(classPath);
        _classes[classPath] = clazz;
    }

    return nullptr;
}
