#ifndef _CLASSESCACHE_H
#define _CLASSESCACHE_H

#include "android/Jni.h"
#include <string>
#include <unordered_map>

class ClassesCache
{
public:
    ClassesCache();
    ClassesCache(std::initializer_list<const char*> preloadClasses);
    ~ClassesCache();

    ClassesCache(const ClassesCache&) = delete;
    ClassesCache& operator =(const ClassesCache&) = delete;
    ClassesCache(ClassesCache&&) = delete;

    ///Function will return global reference to specified class
    jclass get(const char* classPath);

private:
    std::unordered_map<std::string, jclass> _classes;
};


#endif //_CLASSESCACHE_H
