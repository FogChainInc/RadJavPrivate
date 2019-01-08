/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#include "cpp/RadJavCPPOSScreenInfo.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef USE_IOS
    #import <UIKit/UIKit.h>
#endif

#ifdef USE_ANDROID
	#include "android/RadJavAndroid.h"
	#include "android/Utils.h"
#endif

#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#ifdef GUI_USE_WXWIDGETS
	#include <wx/window.h>
	#include <wx/display.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace OS
		{
			ScreenInfo::ScreenInfo(RJINT width, RJINT height, RJNUMBER scale)
			{
				this->width = width;
				this->height = height;
				this->scale = scale;
			}

			#ifdef USE_V8
				ScreenInfo::ScreenInfo(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> obj)
				{
					width = 0;
					height = 0;
					scale = 1;

					width = jsEngine->v8GetInt (obj, "width");
					height = jsEngine->v8GetInt (obj, "height");
					scale = jsEngine->v8GetDecimal (obj, "scale");
				}

				v8::Local<v8::Object> ScreenInfo::toV8Object()
				{
					v8::Handle<v8::Function> func = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "OS");
					v8::Handle<v8::Function> func2 = V8_JAVASCRIPT_ENGINE->v8GetFunction(func, "ScreenInfo");
					v8::Local<v8::Object> ScreenInfoObj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(func2, 0, NULL);

					V8_JAVASCRIPT_ENGINE->v8SetNumber(ScreenInfoObj, "width", width);
					V8_JAVASCRIPT_ENGINE->v8SetNumber(ScreenInfoObj, "height", height);
					V8_JAVASCRIPT_ENGINE->v8SetNumber(ScreenInfoObj, "scale", scale);

					return (ScreenInfoObj);
				}
			#endif

            #ifdef USE_JAVASCRIPTCORE
				ScreenInfo::ScreenInfo(JSCJavascriptEngine *jsEngine, JSObjectRef obj)
				{
					width = 0;
					height = 0;
					scale = 1;

					width = jsEngine->jscGetInt (obj, "width");
					height = jsEngine->jscGetInt (obj, "height");
					scale = jsEngine->jscGetDecimal (obj, "scale");
				}

				JSObjectRef ScreenInfo::toJSCObject()
				{
					JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "OS");
					JSObjectRef func2 = JSC_JAVASCRIPT_ENGINE->jscGetFunction(func, "ScreenInfo");
					JSObjectRef objJSC = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(func2, 0, NULL);

					JSC_JAVASCRIPT_ENGINE->jscSetNumber(objJSC, "width", width);
					JSC_JAVASCRIPT_ENGINE->jscSetNumber(objJSC, "height", height);
					JSC_JAVASCRIPT_ENGINE->jscSetNumber(objJSC, "scale", scale);

					return (objJSC);
				}
			#endif

			RJUINT ScreenInfo::getWidth ()
			{
				return (width);
			}

			RJUINT ScreenInfo::getHeight ()
			{
				return (height);
			}

			RJNUMBER ScreenInfo::getScale ()
			{
				return (scale);
			}

            RJINT ScreenInfo::getNumberOfScreens ()
            {
                RJINT numScreens = 1;

				#ifdef GUI_USE_WXWIDGETS
					numScreens = wxDisplay::GetCount();
				#endif

                return (numScreens);
            }

            ScreenInfo ScreenInfo::getScreenInfo (RJINT screenIndex)
            {
                ScreenInfo info;

				#ifdef GUI_USE_WXWIDGETS
					RJINT screensCount = getNumberOfScreens();
				
					if (screenIndex < screensCount &&
						screenIndex >= 0)
					{
						wxDisplay display(screenIndex);

						wxRect displayBounds = display.GetGeometry();
						info.width = displayBounds.width;
						info.height = displayBounds.height;

						//Get scale factor for main display
						if (screenIndex == 0)
						{
							wxApp* app = wxTheApp;
							if (app)
							{
								wxWindow* mainWindow = app->GetTopWindow();
								if (mainWindow)
								{
									info.scale = mainWindow->GetContentScaleFactor();
								}
							}
						}
					}
				#elif defined USE_IOS
                    UIScreen *screen = [UIScreen mainScreen];
                    CGRect screenRect = [screen bounds];

                    info.width = screenRect.size.width;
                    info.height = screenRect.size.height;
                    info.scale = screen.scale;
				#elif defined USE_ANDROID
                    RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
                    	/* Java example
						Application app = getApplication();

						WindowManager win_manager = (WindowManager) app.getSystemService(Context.WINDOW_SERVICE);

						Display display = win_manager.getDefaultDisplay();

						DisplayMetrics metrics = new DisplayMetrics();

						display.getMetrics(metrics);

                    	//Actual values
                    	metrics.widthPixels;
                    	metrics.heightPixels;
                    	 */

                    	using namespace Android;

                    	Jni& jni = Jni::instance();
                    	RadJavAndroid* radJavApp = RadJavAndroid::instance();
                    	jobject appJava = radJavApp->getJavaApplication();

                    	auto appClass = jni.wrapLocalRef(env->GetObjectClass(appJava));

                    	jmethodID getSystemService = env->GetMethodID(appClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");

                    	jclass contextClass = jni.findClass("android/content/Context");
                    	jfieldID window_service = env->GetStaticFieldID(contextClass, "WINDOW_SERVICE", "Ljava/lang/String;");
                    	auto window_service_str = jni.wrapLocalRef(env->GetStaticObjectField(contextClass, window_service));

                    	auto object = jni.wrapLocalRef(env->CallObjectMethod(appJava, getSystemService, window_service_str.get()));

						auto windowManager = jni.wrapLocalRef(Utils::Cast(object, "android.view.WindowManager"));

						jclass windowManagerClass = jni.findClass("android/view/WindowManager");

						jmethodID getDefaultDisplay = env->GetMethodID(windowManagerClass, "getDefaultDisplay", "()Landroid/view/Display;");

						auto display = jni.wrapLocalRef(env->CallObjectMethod(windowManager, getDefaultDisplay));

						jclass displayMetricsClass = jni.findClass("android/util/DisplayMetrics");
						jmethodID displayMetricsConstructor = env->GetMethodID(displayMetricsClass, "<init>", "()V");

						auto displayMetrics = jni.wrapLocalRef(env->NewObject(displayMetricsClass, displayMetricsConstructor));

						auto displayClass = jni.wrapLocalRef(env->GetObjectClass(display));

						jmethodID getMetrics = env->GetMethodID(displayClass, "getMetrics", "(Landroid/util/DisplayMetrics;)V");

						env->CallVoidMethod(display, getMetrics, displayMetrics.get());

						jfieldID widthPixels = env->GetFieldID(displayMetricsClass, "widthPixels", "I");
						jfieldID heightPixels = env->GetFieldID(displayMetricsClass, "heightPixels", "I");

						info.width = env->GetIntField(displayMetrics, widthPixels);
						info.height = env->GetIntField(displayMetrics, heightPixels);
					});
				#else
					#warning Add ScreenInfo support
				#endif

                return (info);
            }
		}
	}
}

