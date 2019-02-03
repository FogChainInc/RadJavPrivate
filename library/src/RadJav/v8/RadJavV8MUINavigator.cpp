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
#include "v8/RadJavV8MUINavigator.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPMUINavigator.h"
#include "cpp/RadJavCPPMUIView.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::Navigator;
			using CppMuiViewObject = CPP::MUI::View;

			void Navigator::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Navigator::create);
				V8_CALLBACK(object, "_push", Navigator::push);
				V8_CALLBACK(object, "_pop", Navigator::pop);
			}

			void Navigator::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}

			void Navigator::push(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject* appObject = (CppMuiObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if(!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Navigator not initialized");
					return;
				}

				int argsLength = args.Length();

				//Extract view parameter
				CppMuiViewObject *view = nullptr;

				if(argsLength)
				{
					v8::Local<v8::Value> viewJs = V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0);
					if (!V8_JAVASCRIPT_ENGINE->v8IsNull(viewJs))
					{
						//Extract view parameter
						v8::Local<v8::Object> viewObjJs = v8::Local<v8::Object>::Cast(viewJs);
						view = (CppMuiViewObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(viewObjJs, "_appObj");
					}
				}

				//Extract replace parameter
				bool replace = false;
				if(argsLength > 1)
				{
					v8::Local<v8::Value> replaceJs = V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1);
					if(!V8_JAVASCRIPT_ENGINE->v8IsNull(replaceJs))
					{
						replace = v8::Local<v8::Boolean>::Cast(replaceJs)->Value();
					}
				}

				//Sanity check
				if(!view)
				{
					V8_JAVASCRIPT_ENGINE->throwException("View parameter required");
					return;
				}

				//Call native implementation
				appObject->push(view, replace);
			}

			void Navigator::pop(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject* appObject = (CppMuiObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if(!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Navigator not initialized");
					return;
				}

				int argsLength = args.Length();

				if(argsLength)
				{
					v8::Local<v8::Value> viewJs = V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0);
					if (V8_JAVASCRIPT_ENGINE->v8IsNull(viewJs))
					{
						//Call native implementation
						appObject->pop(nullptr);
					}
					else
					{
						//Extract view parameter
						v8::Local<v8::Object> viewObjJs = v8::Local<v8::Object>::Cast(viewJs);
						CppMuiViewObject *view = (CppMuiViewObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(viewObjJs, "_appObj");
						appObject->pop(view);
					}
				}
				else
				{
					//Call native implementation
					appObject->pop();
				}
			}
		}
	}
}
