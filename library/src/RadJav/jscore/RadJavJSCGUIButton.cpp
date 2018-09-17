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
#include "jscore/RadJavJSCGUIButton.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIButton.h"

#define UITYPE CPP::GUI::Button

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			void Button::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
                JSC_CALLBACK(object, "create", Button::create);
                JSC_CALLBACK(object, "setFont", GObject<UITYPE>::setFont);
                JSC_CALLBACK(object, "getFont", GObject<UITYPE>::getFont);
                JSC_CALLBACK(object, "setPosition", GObject<UITYPE>::setPosition);
                JSC_CALLBACK(object, "getPosition", GObject<UITYPE>::getPosition);
                JSC_CALLBACK(object, "getX", GObject<UITYPE>::getX);
                JSC_CALLBACK(object, "getY", GObject<UITYPE>::getY);
                JSC_CALLBACK(object, "setSize", GObject<UITYPE>::setSize);
                JSC_CALLBACK(object, "getSize", GObject<UITYPE>::getSize);
                JSC_CALLBACK(object, "getWidth", GObject<UITYPE>::getWidth);
                JSC_CALLBACK(object, "getHeight", GObject<UITYPE>::getHeight);
                JSC_CALLBACK(object, "setText", GObject<UITYPE>::setText);
                JSC_CALLBACK(object, "getText", GObject<UITYPE>::getText);
                JSC_CALLBACK(object, "getParent", GObject<UITYPE>::getParent);
                JSC_CALLBACK(object, "getAppObj", GObject<UITYPE>::getAppObj);
                JSC_CALLBACK(object, "setVisibility", GObject<UITYPE>::setVisibility);
                JSC_CALLBACK(object, "getVisibility", GObject<UITYPE>::getVisibility);
                JSC_CALLBACK(object, "setEnabled", GObject<UITYPE>::setEnabled);
                JSC_CALLBACK(object, "getEnabled", GObject<UITYPE>::getEnabled);
                JSC_CALLBACK(object, "on", GObject<UITYPE>::on);
                JSC_CALLBACK(object, "destroy", GObject<UITYPE>::destroy);
			}

			JSValueRef Button::create(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                UITYPE *appObject = RJNEW UITYPE(JSC_JAVASCRIPT_ENGINE, thisObj, numArgs, args);
                appObject->create();
                
                JSC_JAVASCRIPT_ENGINE->jscSetExternal(context, thisObj, "_appObj", appObject);
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObj, _guiFinishedCreatingGObject);
                
                return (promise);
			}
		}
	}
}
#endif
