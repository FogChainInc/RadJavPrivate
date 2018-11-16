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

        ///Request function execution on Java UI thread synchronously
        void runOnUiThread(UiThreadCallbackFunctionType function, void *data = nullptr);

        ///Request function execution on Java UI thread
        void runOnUiThreadAsync(UiThreadCallbackFunctionType function, void *data = nullptr);

        ///Terminate main loop
        void terminate(int exitCode);

        ///Get main java application instance
        jobject getJavaApplication() const;

        ///Get main java ViewGroup
        jobject getJavaViewGroup() const;

        ///Determine if there is some unfinished job requested on Android UI thread
        bool isWaitingForUiThread();

        ///Check if Android application was paused (Activity is in a background or destroyed)
        bool isPaused() const;

    private:
        void uiThreadRequested();

        void uiThreadArrived(bool async);

        void runOnUiThread(UiThreadCallbackFunctionType function, void *data, bool asynchronously);

        ///Java thread entry point
        static jint onRun(JNIEnv *env,
                          jobject thisObject,
                          jobject application,
                          jobject viewGroup,
                          jstring cachePath,
                          jstring appPath);

        ///Ui thread callback called by request from runOnUiThreadAsynch
        static void onUiThread(JNIEnv *env,
                               jobject javaUiCallbackObject,
                               jobject data);

        ///UI thread callback when UI event occur
        static jboolean onUiEvent(JNIEnv *env,
                                  jobject javaUiEventListenerObject,
                                  jobject data,
                                  jobjectArray arguments);


    private:
        jobject _java_application;
        jobject _java_view_group;
        bool _terminated;
        int _exit_code;
        unsigned long _ui_thread_request_counter;
        std::mutex _counter_protector_mutex;
        std::mutex _sync_ui_call_mutex;

        static RadJavAndroid *_instance;
    };
}

#endif