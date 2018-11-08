#include <jni.h>
#include "RadJav.h"


extern "C" JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
//    TODO: update libRadJav code
//    if (!RadJAV::RadJav::initialize(vm))
//        return -1;

    return JNI_VERSION_1_6;
}
