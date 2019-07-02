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
#include "v8/RadJavV8MUITableViewModel.h"

#include "RadJav.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPMUITableViewModel.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::TableViewModel;
			
            void TableViewModel::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
            {
				V8_CALLBACK(object, "_init", TableViewModel::init);
				V8_CALLBACK(object, "_itemPushed", TableViewModel::itemPushed);
				V8_CALLBACK(object, "_itemRemoved", TableViewModel::itemRemoved);
				V8_CALLBACK(object, "_itemsCleared", TableViewModel::itemsCleared);
            }
			
			void TableViewModel::init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("TableViewModel already created");
					return;
				}

				appObject = RJNEW CppMuiObject(V8_JAVASCRIPT_ENGINE, args);
				
				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
			}
			
			void TableViewModel::itemPushed(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("TableViewModel not initialized");
					return;
				}
				
				if (args.Length() && args[0]->IsNumber())
				{
					appObject->itemAdded(V8_JAVASCRIPT_ENGINE->v8ParseInt(args[0]));
				}
			}
			
			void TableViewModel::itemRemoved(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("TableViewModel not initialized");
					return;
				}

				if (args.Length() && args[0]->IsNumber())
				{
					appObject->itemRemoved(V8_JAVASCRIPT_ENGINE->v8ParseInt(args[0]));
				}
			}
			
			void TableViewModel::itemsCleared(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("TableViewModel not initialized");
					return;
				}
				
				appObject->itemsCleared();
			}
		}
	}
}
