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
#include "v8/RadJavV8MUIImage.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPMUIImage.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::Image;
			
			void Image::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Image::create);
				V8_CALLBACK(object, "setImage", Image::setImage);
				V8_CALLBACK(object, "setScaleMode", Image::setScaleMode);
				V8_CALLBACK(object, "getScaleMode", Image::getScaleMode);
			}
			
			void Image::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
			}
			
			void Image::setImage(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Image not initialized");

					return;
				}

				v8::Local<v8::String> pathJs = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

				if (V8_JAVASCRIPT_ENGINE->v8IsNull(pathJs))
				{
					V8_JAVASCRIPT_ENGINE->throwException("Path argument required");

					return;
				}

				appObject->setImage(parseV8Value(pathJs));
			}
			
			void Image::setScaleMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Image not initialized");

					return;
				}

				v8::Local<v8::Integer> scaleModeJs = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

				if (V8_JAVASCRIPT_ENGINE->v8IsNull(scaleModeJs))
				{
					V8_JAVASCRIPT_ENGINE->throwException("Scale mode argument required");

					return;
				}

				RJINT scaleMode = V8_JAVASCRIPT_ENGINE->v8ParseInt(scaleModeJs);

				switch (scaleMode)
				{
					case 1:
						appObject->setScaleMode(CPP::MUI::Image::ScaleMode::AspectFit);
						break;
					case 2:
						appObject->setScaleMode(CPP::MUI::Image::ScaleMode::AspectFill);
						break;
					default:
						V8_JAVASCRIPT_ENGINE->throwException("Unsupported ScaleMode");
				}
			}

			void Image::getScaleMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Image not initialized");

					return;
				}

				auto scaleMode = appObject->getScaleMode();

				switch (scaleMode)
				{
					case CPP::MUI::Image::ScaleMode::AspectFill:
						args.GetReturnValue().Set(2);
					case CPP::MUI::Image::ScaleMode::AspectFit:
					default:
						args.GetReturnValue().Set(1);
				}
			}
		}
	}
}
