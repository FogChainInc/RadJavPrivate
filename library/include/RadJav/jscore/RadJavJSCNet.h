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
#ifndef _RADJAV_JSC_NET_H_
#define _RADJAV_JSC_NET_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"
#include "RadJavThread.h"

#include <JavaScriptCore/JavaScriptCore.h>

#ifdef GUI_USE_WXWIDGETS
	#include <wx/wx.h>
#endif

namespace RadJAV
{
	namespace JSC
	{
		namespace Net
		{
			class RADJAV_EXPORT NetCallbacks
			{
			public:
				static void createV8Callbacks(JSContextRef context, JSObjectRef object);
				
				static JSValueRef httpRequest(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
				static JSValueRef httpPost(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
				static JSValueRef completeHttpRequest(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception);
				
				static RJINT curlWrite(RJCHAR *data, RJUINT size, RJUINT nmemb, String *output);
			};
			
			class RADJAV_EXPORT HttpThread : public Thread
			{
			public:
				HttpThread(String uri, RJBOOL post, RJLONG timeout, JSContextRef ctx, JSObjectRef resolvep);
				~HttpThread();
#ifdef GUI_USE_WXWIDGETS
				wxThread::ExitCode Entry();
#else
				RJINT Entry();
#endif
				
				RJLONG timeout;
				String uri;
				JSObjectRef resolvep;
				JSContextRef context;
				RJBOOL post;
			};
		}
	}
}

#endif
