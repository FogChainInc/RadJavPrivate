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
#include "RadJavJSExternals.h"
#include "cpp/RadJavCPPChainedPtr.h"
#include "RadJav.h"

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#elif defined USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#if defined USE_V8 || defined USE_JAVASCRIPTCORE

namespace RadJAV
{
	ExternalsManager::ExternalsManager()
	{
#ifdef USE_V8
		impl = RJNEW ExternalsManagerV8Impl();
#elif defined USE_JAVASCRIPTCORE
		impl = RJNEW ExternalsManagerJscImpl();
#endif
	}
	
	ExternalsManager::~ExternalsManager()
	{
		RJDELETE(impl);
	}
	
#ifdef USE_V8
	void ExternalsManager::set(const v8::Local<v8::Object>& handle, const String& functionName, CPP::ChainedPtr* object)
	{
		impl->set(handle, functionName, object);
	}
	
	CPP::ChainedPtr* ExternalsManager::get(const v8::Local<v8::Object>& handle, const String& functionName)
	{
		return impl->get(handle, functionName);
	}
	
	void ExternalsManager::clear(const v8::Local<v8::Object>& handle, const String& functionName)
	{
		impl->clear(handle, functionName);
	}
#elif defined USE_JAVASCRIPTCORE
	void ExternalsManager::set(JSContextRef context, JSObjectRef handle, const String& functionName, CPP::ChainedPtr* object)
	{
		impl->set(context, handle, functionName, object);
	}
	
	CPP::ChainedPtr* ExternalsManager::get(JSContextRef context, JSObjectRef handle, const String& functionName)
	{
		return impl->get(context, handle, functionName);
	}
	
	void ExternalsManager::clear(JSContextRef context, JSObjectRef handle, const String& functionName)
	{
		impl->clear(context, handle, functionName);
	}
#endif
}

#endif

