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
#include "jscore/RadJavJSCGlobal.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCJavascriptEngine.h"

	#include "cpp/RadJavCPPIO.h"

	#ifdef GUI_USE_WXWIDGETS
		#include <wx/wx.h>
	#endif

namespace RadJAV
{
	namespace JSC
	{
		void Global::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
            JSC_CALLBACK(object, "alert", Global::alert);
		}

		JSValueRef Global::alert(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
		{
			String message = "";
			String title = "";

            message = parseJSCValue (context, args[0]);

            if (numArgs > 1)
                title = parseJSCValue (context, args[1]);

			RadJav::showMessageBox(message, title);

            return (JSValueMakeUndefined(context));
		}
	}
}
#endif

