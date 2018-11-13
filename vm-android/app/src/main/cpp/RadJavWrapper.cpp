#include <jni.h>
#include "RadJavC.h"
#include <cstdlib>


extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    if (RadJav_initialize(vm) == EXIT_FAILURE)
        return -1;

    return JNI_VERSION_1_6;
}
