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
#include "v8/RadJavV8Testing.h"

#include "RadJav.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPTesting.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace Testing
		{
			void KeyboardSimulator::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "keyPress", KeyboardSimulator::keyPress);
			}

			void KeyboardSimulator::keyPress(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> arg = args[0];
				String key = parseV8Value (arg);

				CPP::Testing::KeyboardSimulator::keyPress(key);
			}

			void MouseSimulator::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "click", MouseSimulator::click);
				V8_CALLBACK(object, "doubleClick", MouseSimulator::doubleClick);
				V8_CALLBACK(object, "setPosition", MouseSimulator::setPosition);
				V8_CALLBACK(object, "wheel", MouseSimulator::wheel);
			}

			void MouseSimulator::click(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> arg = args[0];
				RJINT button = V8_JAVASCRIPT_ENGINE->v8ParseInt(arg);

				CPP::Testing::MouseSimulator::click(button);
			}

			void MouseSimulator::doubleClick(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> arg = args[0];
				RJINT button = V8_JAVASCRIPT_ENGINE->v8ParseInt(arg);
				
				CPP::Testing::MouseSimulator::doubleClick(button);
			}

			void MouseSimulator::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> arg = args[0];
				CPP::Vector2 pos(v8::Local<v8::Object>::Cast (arg));

				CPP::Testing::MouseSimulator::setPosition(pos);
			}

			void MouseSimulator::wheel(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJINT vertical = 0;
				RJINT horizontal = 0;
				
				if (args.Length() > 0
					&& args[0]->IsNumber())
				{
						vertical = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[0]);
				}
				
				if (args.Length() > 1
					&& args[1]->IsNumber())
				{
					horizontal = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[1]);
				}

				if (vertical != 0 ||
					horizontal != 0)
				{
					CPP::Testing::MouseSimulator::wheel(vertical, horizontal);
				}
			}
		}
	}
}
