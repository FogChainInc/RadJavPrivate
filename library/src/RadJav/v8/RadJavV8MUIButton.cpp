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
#include "v8/RadJavV8MUIButton.h"

#include "RadJav.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPMUIButton.h"

namespace RadJAV
{
	namespace V8
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::Button;
			
			void Button::createJSCCallbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Button::create);
			}

			void Button::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = V8_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}
		}
	}
}
