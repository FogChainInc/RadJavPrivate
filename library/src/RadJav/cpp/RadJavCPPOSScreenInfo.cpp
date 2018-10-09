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

#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCJavascriptEngine.h"
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

                /// @todo Fill this out later using wxWidgets.

                return (numScreens);
            }

            ScreenInfo ScreenInfo::getScreenInfo (RJINT screenIndex)
            {
                ScreenInfo info;

                /// @todo Fill this out later using wxWidgets.
                #ifdef USE_IOS
                    UIScreen *screen = [UIScreen mainScreen];
                    CGRect screenRect = [screen bounds];

                    info.width = screenRect.size.width;
                    info.height = screenRect.size.height;
                    info.scale = screen.scale;
                #endif

                return (info);
            }
		}
	}
}

