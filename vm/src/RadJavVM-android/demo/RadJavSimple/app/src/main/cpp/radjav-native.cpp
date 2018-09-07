#include <jni.h>
#include <string>
#include <RadJav/RadJav.h>
extern "C" JNIEXPORT jstring

JNICALL
Java_com_fogchain_aparkalau_radjavsimple_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "radjav-native string: ";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jint


JNICALL
Java_com_fogchain_aparkalau_radjavsimple_MainActivity_initializeVM(
        JNIEnv *env,
        jobject /* this */,
        jstring path_to_exec,
        jstring path_to_file,
        jstring parameters) {

    const char *native_path_to_exec = env->GetStringUTFChars(path_to_exec, JNI_FALSE);
    const char *native_path_to_file = env->GetStringUTFChars(path_to_file, JNI_FALSE);
    const char *native_parameters = env->GetStringUTFChars(parameters, JNI_FALSE);

    //TODO: implement
    RadJAV::Array<RadJAV::String> args;
    args.push_back(RadJAV::String(native_path_to_exec));
    args.push_back(RadJAV::String(native_path_to_file));

    RadJAV::String file;
    RadJAV::RadJavType type = RadJAV::RadJav::initialize(args, file);
    if (type == RadJAV::RadJavType::XRJ_NODE) return (EXIT_SUCCESS);

    try
    {
        if (args.size() < 2)
        {
            //showError("No files to execute or arguments specified!");
            //printf("No files to execute or arguments specified!\n");
            return (EXIT_FAILURE);
        }

        return (RadJAV::RadJav::runApplicationFromFile (file));
    }
    catch (RadJAV::Exception ex)
    {
        //showError (ex.getMessage());
        //printf("ex.getMessage() : %s", ex.getMessage().c_str());
        return (EXIT_FAILURE);
    }

    env->ReleaseStringUTFChars(path_to_exec, native_path_to_exec);
    env->ReleaseStringUTFChars(path_to_file, native_path_to_file);
    env->ReleaseStringUTFChars(parameters, native_parameters);

    return (int)type;
}


extern "C" JNIEXPORT void

JNICALL
Java_com_fogchain_aparkalau_radjavsimple_MainActivity_shutdownVM(
        JNIEnv *env,
        jobject /* this */) {
    //TODO: implement
    RadJAV::Static::shutdownVM();
    return;
}
extern "C" JNIEXPORT jint

JNICALL
Java_com_fogchain_aparkalau_radjavsimple_MainActivity_runApplication(
        JNIEnv *env,
        jobject /* this */) {
    //TODO: implement
    return 1;
}

extern "C" JNIEXPORT jint

JNICALL
Java_com_fogchain_aparkalau_radjavsimple_MainActivity_runApplicationFromFile(
        JNIEnv *env,
        jobject /* this */) {
    //TODO: implement
    return 2;
}
