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
