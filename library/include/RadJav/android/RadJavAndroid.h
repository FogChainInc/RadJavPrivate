#ifndef _RADJAV_ANDROID_H_
#define _RADJAV_ANDROID_H_

#include "RadJavPreprocessor.h"
#include "android/Jni.h"
#include "android/UiThreadDispatcher.h"
#include "android/UiThreadCallbackFunction.h"

namespace RadJAV
{
    class RadJavAndroid : public UiThreadDispatcher {
    public:
        RadJavAndroid();

        ~RadJavAndroid();

        RadJavAndroid(const RadJavAndroid &) = delete;

        RadJavAndroid(RadJavAndroid &&) = delete;

        RadJavAndroid &operator=(const RadJavAndroid &) = delete;

        static RadJavAndroid *instance();
        static bool registerNatives();

        ///Request function execution on Java UI thread
        void runOnUiThreadAsync(UiThreadCallbackFunctionType function, void *data);

        ///Terminate main loop
        void terminate(int exitCode);

        ///Get main java application instance
        jobject getJavaApplication() const;

        ///Get main java ViewGroup
        jobject getJavaViewGroup() const;

    private:
        //Java thread entry point
        static jint onRun(JNIEnv *env,
                          jobject thisObject,
                          jobject application,
                          jobject viewGroup,
                          jstring cachePath,
                          jstring appPath);

        //Ui thread callback called by request from runOnUiThreadAsynch
        static void onUiThread(JNIEnv *env,
                               jobject javaUiCallbackObject,
                               jobject data);

        //UI thread callback when UI event occur
        static jboolean onUiEvent(JNIEnv *env,
                                  jobject javaUiEventListenerObject,
                                  jobject data,
                                  jobjectArray arguments);

        bool isWaitingForUiThread();

        void uiThreadRequested();

        void uiThreadArrived();

        bool paused() const;

    private:
        jobject _java_application;
        jobject _java_view_group;
        bool _terminated;
        int _exit_code;
        unsigned long _ui_thread_request_counter;
        std::mutex _mutex;

        static RadJavAndroid *_instance;
    };
}

#endif