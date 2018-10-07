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
#include "jscore/RadJavJSCOSScreenInfo.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPOSScreenInfo.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace OS
		{
            using CppMuiObject = CPP::OS::ScreenInfo;

			void ScreenInfo::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
                JSC_CALLBACK(object, "getNumberOfScreens", ScreenInfo::getNumberOfScreens);
                JSC_CALLBACK(object, "getScreenInfo", ScreenInfo::getScreenInfo);
			}

			JSValueRef ScreenInfo::getNumberOfScreens(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeNumber(context, CppMuiObject::getNumberOfScreens ()));
			}

			JSValueRef ScreenInfo::getScreenInfo(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                RJINT screenIndex = JSC_JAVASCRIPT_ENGINE->jscParseInt(args[0]);
                CppMuiObject screenInfo = CppMuiObject::getScreenInfo (screenIndex);
                JSObjectRef obj = screenInfo.toJSCObject ();

				return (obj);
			}
		}
	}
}
