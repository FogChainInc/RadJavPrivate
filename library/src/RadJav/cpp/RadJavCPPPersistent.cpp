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

#include "cpp/RadJavCPPPersistent.h"
#include "RadJav.h"

namespace RadJAV
{
	namespace CPP
	{
		#ifdef USE_V8
			Persistent::Persistent(v8::Local<v8::Object> jsObject)
			{
				persistent = RJNEW v8::Persistent<v8::Object>();
				persistent->Reset(jsObject->GetIsolate(), jsObject);
			}

		#elif defined USE_JAVASCRIPTCORE
			Persistent::Persistent(JSObjectRef object)
			{
				persistent = object;
				JSValueProtect(JSC_JAVASCRIPT_ENGINE->globalContext, persistent);
			}
		#endif

		Persistent::~Persistent()
		{
			#ifdef USE_V8
				persistent->Reset();
				RJDELETE persistent;
			#elif defined USE_JAVASCRIPTCORE
				JSValueUnprotect(JSC_JAVASCRIPT_ENGINE->globalContext, persistent);
			#endif
		}

		#ifdef USE_V8
			v8::Local<v8::Object> Persistent::get() const
			{
				return persistent->Get(V8_JAVASCRIPT_ENGINE->isolate);
			}
		#elif defined USE_JAVASCRIPTCORE
			JSObjectRef Persistent::get() const
			{
				return persistent;
			}
		#endif

	}
}
