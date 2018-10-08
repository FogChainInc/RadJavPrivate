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
#include "v8/RadJavV8OSScreenInfo.h"

#include "RadJav.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPOSScreenInfo.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace OS
		{
			using CppMuiObject = CPP::OS::ScreenInfo;
			
			void ScreenInfo::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
                V8_CALLBACK(object, "getWidth", ScreenInfo::getWidth);
                V8_CALLBACK(object, "getHeight", ScreenInfo::getHeight);
                V8_CALLBACK(object, "getScale", ScreenInfo::getScale);
			}

			void ScreenInfo::getWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), CppMuiObject::getWidth()));
			}

			void ScreenInfo::getHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), CppMuiObject::getHeight()));
			}

			void ScreenInfo::getScale(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), CppMuiObject::getScale()));
			}
		}
	}
}
