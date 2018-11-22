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
#include "v8/RadJavV8GUIImage.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPGUIImage.h"

#define UITYPE CPP::GUI::Image

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
			void Image::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Image::create);
				V8_CALLBACK(object, "setImage", Image::setImage);
			}

			void Image::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = RJNEW UITYPE(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}

			void Image::setImage(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> value = v8::Local<v8::Value>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				args.This()->Set(String("_image").toV8String(args.GetIsolate()), value);
				String src = "";
				RJINT width = 0;
				RJINT height = 0;

				if (value->IsString() == true)
					src = parseV8Value(value);

				if (value->IsObject() == true)
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(value);
					src = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "src");
					width = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "width");
					height = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "height");
				}

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setImage(src);
			}
		}
	}
}
