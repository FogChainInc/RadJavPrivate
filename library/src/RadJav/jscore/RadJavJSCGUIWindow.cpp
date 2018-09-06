/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#include "jscore/RadJavJSCGUIWindow.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIWindow.h"

#define UITYPE CPP::GUI::Window

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			void Window::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
                JSC_CALLBACK(object, "create", Window::create);
                JSC_CALLBACK(object, "setFont", Window::setFont);
                JSC_CALLBACK(object, "getFont", Window::getFont);
                JSC_CALLBACK(object, "setPosition", Window::setPosition);
                JSC_CALLBACK(object, "getPosition", Window::getPosition);
                JSC_CALLBACK(object, "getX", Window::getX);
                JSC_CALLBACK(object, "getY", Window::getY);
                JSC_CALLBACK(object, "setSize", Window::setSize);
                JSC_CALLBACK(object, "getSize", Window::getSize);
                JSC_CALLBACK(object, "getWidth", Window::getWidth);
                JSC_CALLBACK(object, "getHeight", Window::getHeight);
                JSC_CALLBACK(object, "setText", Window::setText);
                JSC_CALLBACK(object, "getText", Window::getText);
                JSC_CALLBACK(object, "getParent", Window::getParent);
                JSC_CALLBACK(object, "getAppObj", Window::getAppObj);
                JSC_CALLBACK(object, "setVisibility", Window::setVisibility);
                JSC_CALLBACK(object, "getVisibility", Window::getVisibility);
                JSC_CALLBACK(object, "setEnabled", Window::setEnabled);
                JSC_CALLBACK(object, "getEnabled", Window::getEnabled);
                JSC_CALLBACK(object, "setIcon", Window::setIcon);
                JSC_CALLBACK(object, "on", Window::on);
                JSC_CALLBACK(object, "destroy", Window::destroy);
			}

			JSValueRef Window::create(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                UITYPE *appObject = RJNEW UITYPE(JSC_JAVASCRIPT_ENGINE, thisObj, numArgs, args);
                appObject->create();
                
                JSC_JAVASCRIPT_ENGINE->jscSetExternal(context, thisObj, "_appObj", appObject);
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObj, _guiFinishedCreatingGObject);
                
                return (promise);
			}

			JSValueRef Window::addChild(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::setFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::setPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getX(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getY(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::setSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getWidth(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getHeight(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::setText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getParent(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::setVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::setEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}
            
            JSValueRef Window::setIcon(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                return (JSValueMakeUndefined(context));
            }

			JSValueRef Window::on(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}
            
            JSValueRef Window::destroy(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                return (JSValueMakeUndefined(context));
            }
		}
	}
}
#endif
