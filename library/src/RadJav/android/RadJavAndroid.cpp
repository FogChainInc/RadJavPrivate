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
#include <RadJav.h>
#include "android/RadJavAndroid.h"
#include "RadJavPreprocessor.h"
#include "android/Utils.h"

namespace RadJAV
{
	namespace Android
	{
		RadJavAndroid *RadJavAndroid::_instance = nullptr;

		RadJavAndroid *RadJavAndroid::instance() {
			return _instance;
		}

		bool RadJavAndroid::registerNatives()
		{
			Jni& jni = Jni::instance();
			JNIEnv* env = jni.getJniEnv();

			if (!env)
				return false;

			JNINativeMethod runCallback[1];
			JNINativeMethod uiCallback[1];
			JNINativeMethod uiEventCallback[1];
			JNINativeMethod nativeCallback[1];

			runCallback[0].name = "RadJavRun";
			runCallback[0].signature = "(Lcom/fogchain/radjavvm/RadJavApplication;Landroid/view/ViewGroup;Ljava/lang/String;Ljava/lang/String;)I";
			runCallback[0].fnPtr = reinterpret_cast<void*>(RadJavAndroid::onRun);

			uiCallback[0].name = "RadJavUiCallback";
			uiCallback[0].signature = "(Ljava/nio/ByteBuffer;)V";
			uiCallback[0].fnPtr = reinterpret_cast<void*>(RadJavAndroid::onUiThread);

			uiEventCallback[0].name = "RadJavUiEvent";
			uiEventCallback[0].signature = "(Ljava/nio/ByteBuffer;[Ljava/lang/Object;)Z";
			uiEventCallback[0].fnPtr = reinterpret_cast<void*>(RadJavAndroid::onUiEvent);

			nativeCallback[0].name = "RadJavNativeCallback";
			nativeCallback[0].signature = "(Ljava/nio/ByteBuffer;[Ljava/lang/Object;)Ljava/lang/Object;";
			nativeCallback[0].fnPtr = reinterpret_cast<void*>(RadJavAndroid::onUiCall);

			jclass radJavApplicationClass = jni.findClass("com/fogchain/radjavvm/RadJavApplication");
			jclass uiCallbackClass = jni.findClass("com/fogchain/radjavvm/UiCallback");
			jclass uiEventCallbackClass = jni.findClass("com/fogchain/radjavvm/UiEventListener");
			jclass nativeCallbackClass = jni.findClass("com/fogchain/radjavvm/NativeCallback");

			jint result = 0;
			result |= env->RegisterNatives(radJavApplicationClass, runCallback, 1);
			result |= env->RegisterNatives(uiCallbackClass, uiCallback, 1);
			result |= env->RegisterNatives(uiEventCallbackClass, uiEventCallback, 1);
			result |= env->RegisterNatives(nativeCallbackClass, nativeCallback, 1);

			return result == 0;
		}

		void RadJavAndroid::runOnUiThreadAsync(UiThreadCallbackFunctionType function, void *data)
		{
			//LOGI(__FUNCTION__);

			runOnUiThread(function, data);
		}

		void RadJavAndroid::runOnUiThread(UiThreadCallbackFunctionType function, void *data)
		{
			//LOGI(__FUNCTION__);

//        Handler handler = new Handler(Looper.getMainLooper());
//        handler.post(new Runnable() {
//            public void run() {
//                // UI code goes here
//            }
//        });

			Jni &jni = Jni::instance();
			JNIEnv *env = Jni::instance().getJniEnv();

			jclass handlerClass = jni.findClass("android/os/Handler");
			jclass looperClass = jni.findClass("android/os/Looper");
			jclass uiCallbackClass = jni.findClass("com/fogchain/radjavvm/UiCallback");

			jmethodID getMainLooper = env->GetStaticMethodID(looperClass, "getMainLooper",
															 "()Landroid/os/Looper;");
			jmethodID handlerConstructor = env->GetMethodID(handlerClass, "<init>",
															"(Landroid/os/Looper;)V");
			jmethodID uiCallbackConstructor = env->GetMethodID(uiCallbackClass, "<init>",
															   "(Ljava/nio/ByteBuffer;)V");

			jmethodID post = env->GetMethodID(handlerClass, "post", "(Ljava/lang/Runnable;)Z");

			auto mainLooper = jni.wrapLocalRef(env->CallStaticObjectMethod(looperClass, getMainLooper));
			auto handler = jni.wrapLocalRef(
					env->NewObject(handlerClass, handlerConstructor, mainLooper.get()));

			//Wrap data
			UiThreadCallbackFunction *func = new UiThreadCallbackFunction(this, function, data);

			auto functionObject = jni.wrapLocalRef(env->NewDirectByteBuffer(func, sizeof(func)));

			auto uiCallback = jni.wrapLocalRef(
					env->NewObject(uiCallbackClass, uiCallbackConstructor, functionObject.get()));

			jboolean result = env->CallBooleanMethod(handler, post, uiCallback.get());

			uiThreadRequested();
		}

		void RadJavAndroid::terminate(int exitCode)
		{
			_terminated = true;
			_exit_code = exitCode;

			Jni &jni = Jni::instance();
			JNIEnv *env = jni.getJniEnv();

			auto appClass = jni.wrapLocalRef(env->GetObjectClass(_java_application));

			jmethodID shutdownMethodId = env->GetMethodID(appClass, "shutdown", "()V");

			env->CallVoidMethod(_java_application, shutdownMethodId);
		}

		jobject RadJavAndroid::getJavaApplication() const
		{
			return _java_application;
		}

		jobject RadJavAndroid::getJavaViewGroup() const
		{
			return _java_view_group;
		}

		RadJavAndroid::~RadJavAndroid()
		{
			terminate(0);
		}

		RadJavAndroid::RadJavAndroid()
		{
			_java_application = nullptr;
			_java_view_group = nullptr;
			_terminated = false;
			_exit_code = EXIT_SUCCESS;

			_ui_thread_request_counter = 0;

			_native_callback_result = nullptr;

			_instance = this;
		}

		bool RadJavAndroid::isWaitingForUiThread()
		{
			std::unique_lock<std::mutex> lock{_counter_protector_mutex};

			return _ui_thread_request_counter != 0;
		}

		void RadJavAndroid::uiThreadRequested()
		{
			std::unique_lock<std::mutex> lock{_counter_protector_mutex};

			_ui_thread_request_counter++;
		}

		void RadJavAndroid::uiThreadArrived()
		{
			std::unique_lock<std::mutex> lock{_counter_protector_mutex};

			_ui_thread_request_counter--;
		}

		bool RadJavAndroid::isPaused() const
		{
			Jni &jni = Jni::instance();
			JNIEnv *env = jni.getJniEnv();

			auto appClass = jni.wrapLocalRef(env->GetObjectClass(_java_application));

			jfieldID pausedFieldId = env->GetStaticFieldID(appClass, "paused", "Z");

			jboolean paused = env->GetStaticBooleanField(appClass, pausedFieldId);

			return paused;
		}

		jint RadJavAndroid::onRun(JNIEnv *env,
								  jobject thisObject,
								  jobject application,
								  jobject viewGroup,
								  jstring cachePath,
								  jstring appPath)
		{
			LOGI(__FUNCTION__);

			Jni::instance().storeJniEnvForThread(env);
			RadJavAndroid *app = RadJavAndroid::instance();

			//Store Java Application in global reference
			app->_java_application = env->NewGlobalRef(application);

			//Store Java ViewGroup in global reference
			app->_java_view_group = env->NewGlobalRef(viewGroup);

			const char* cachePathUtf = env->GetStringUTFChars(cachePath, nullptr);
			String cacheDirPath(cachePathUtf);
			env->ReleaseStringUTFChars(appPath, cachePathUtf);

			const char* appPathUtf = env->GetStringUTFChars(appPath, nullptr);
			String appFilePath(appPathUtf);
			env->ReleaseStringUTFChars(appPath, appPathUtf);

			//Initialize RadJav
			Array<String> params;

			params.push_back(cacheDirPath);
			params.push_back(appFilePath);
			String file = "";

			RadJav::initialize(params, file);

			//Enter main loop
			int result = RadJav::runApplicationFromFile(file);

			return result;
		}

		void RadJavAndroid::onUiThread(JNIEnv *env,
									   jobject javaUiCallbackObject,
									   jobject data)
		{
			//LOGI(__FUNCTION__);

			Jni::instance().storeJniEnvForThread(env);

			//Get data
			UiThreadCallbackFunction *callback = (UiThreadCallbackFunction *) env->GetDirectBufferAddress(
					data);

			//Execute callback function
			(*callback)(env);

			//Notify about received UI callback
			callback->dispatch();

			delete callback;
		}

		jboolean RadJavAndroid::onUiEvent(JNIEnv *env,
										  jobject javaUiEventListenerObject,
										  jobject data,
										  jobjectArray arguments)
		{
			LOGI(__FUNCTION__);

			Jni::instance().storeJniEnvForThread(env);
			RadJavAndroid* app = RadJavAndroid::instance();
			if (!app)
				return JNI_FALSE;

			CPP::GUI::EventData* eventData = (CPP::GUI::EventData*)env->GetDirectBufferAddress(data);

			jboolean result = JNI_FALSE;

			if (eventData != nullptr)
			{
				auto retVal = eventData->_widget->executeEvent(static_cast<CPP::GUI::Event*>(eventData->_event));

				//check for boolean return value, if exists - return it to the UiEventListener
				if (!V8_JAVASCRIPT_ENGINE->v8IsNull(retVal) &&
					retVal->IsBoolean())
				{
					// true from event means it is consumed
					bool consumeEvent = retVal->IsTrue();

					result = consumeEvent ? JNI_TRUE : JNI_FALSE;
				}
			}

			return result;
		}

		jobject RadJavAndroid::onUiCall(JNIEnv *env,
										jobject javaNativeCallbackObject,
										jobject data,
										jobjectArray arguments)
		{
			LOGI(__FUNCTION__);

			Jni::instance().storeJniEnvForThread(env);
			RadJavAndroid* app = RadJavAndroid::instance();

			if (!app)
			{
				LOGI("RadJavAndroid is null!");
				return nullptr;
			}

			if (app->_native_callback_result)
			{
				env->DeleteGlobalRef(app->_native_callback_result);
			}

			app->_native_callback_result = nullptr;
			NativeCallbackFunction* nativeCallback = static_cast<NativeCallbackFunction*>(env->GetDirectBufferAddress(data));

			if (nativeCallback &&
				arguments)
			{
				app->_native_callback_result = (*nativeCallback)(Jni::instance().getJniEnv(), arguments);
			}

			if (!app->_native_callback_result)
			{
				LOGI("Returning null result!");
			}

			return app->_native_callback_result;
		}
	}
}
