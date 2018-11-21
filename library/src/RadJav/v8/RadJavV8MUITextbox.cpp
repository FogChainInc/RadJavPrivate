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
#include "v8/RadJavV8MUITextbox.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPMUITextbox.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::Textbox;
			
			void Textbox::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Textbox::create);
				
				V8_CALLBACK(object, "setInputMode", Textbox::setInputMode);
				V8_CALLBACK(object, "getInputMode", Textbox::getInputMode);

			}
			
			void Textbox::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();
				
				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);

				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}
			
			void Textbox::setInputMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				/* TODO: add implementation
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject *appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Textbox not initialized");
					return undefined;
				}
				
				JSValueRef inputModeValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!inputModeValue ||
					!JSValueIsNumber(ctx, inputModeValue))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "InputMode argument required");
					return undefined;
				}
				
				RJINT inputMode = JSC_JAVASCRIPT_ENGINE->jscValueToInt(inputModeValue);
				
				switch (inputMode)
				{
					case 1:
						appObject->setInputMode(CPP::MUI::Textbox::InputMode::Text);
						break;
					case 2:
						appObject->setInputMode(CPP::MUI::Textbox::InputMode::Number);
						break;
					case 3:
						appObject->setInputMode(CPP::MUI::Textbox::InputMode::Decimal);
						break;
					case 4:
						appObject->setInputMode(CPP::MUI::Textbox::InputMode::Phone);
						break;
					case 5:
						appObject->setInputMode(CPP::MUI::Textbox::InputMode::Email);
						break;
					case 6:
						appObject->setInputMode(CPP::MUI::Textbox::InputMode::Password);
						break;
					default:
						JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Unsupported InputMode");
				}
				return undefined;
				 */
			}
			
			void Textbox::getInputMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				/* TODO: add implementation
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject *appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Textbox not initialized");
					return undefined;
				}
				
				auto inputMode = appObject->getInputMode();
				
				switch (inputMode)
				{
					case CPP::MUI::Textbox::InputMode::Number:
						return JSValueMakeNumber(ctx, 2.0);
					case CPP::MUI::Textbox::InputMode::Decimal:
						return JSValueMakeNumber(ctx, 3.0);
					case CPP::MUI::Textbox::InputMode::Phone:
						return JSValueMakeNumber(ctx, 4.0);
					case CPP::MUI::Textbox::InputMode::Email:
						return JSValueMakeNumber(ctx, 5.0);
					case CPP::MUI::Textbox::InputMode::Password:
						return JSValueMakeNumber(ctx, 6.0);
					case CPP::MUI::Textbox::InputMode::Text:
					default:
						return JSValueMakeNumber(ctx, 1.0);
				}
				 */
			}
		}
	}
}
