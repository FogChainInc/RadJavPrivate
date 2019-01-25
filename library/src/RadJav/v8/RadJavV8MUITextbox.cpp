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
			}
			
			void Textbox::setInputMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Textbox not initialized");

					return;
				}

				v8::Local<v8::Integer> inputModeJs = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

				if (V8_JAVASCRIPT_ENGINE->v8IsNull(inputModeJs))
				{
					V8_JAVASCRIPT_ENGINE->throwException("Input mode argument required");

					return;
				}

				RJINT inputMode = V8_JAVASCRIPT_ENGINE->v8ParseInt(inputModeJs);

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
						V8_JAVASCRIPT_ENGINE->throwException("Unsupported InputMode");
				}
			}
			
			void Textbox::getInputMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = (CppMuiObject*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Textbox not initialized");

					return;
				}

				auto inputMode = appObject->getInputMode();

				switch (inputMode)
				{
					case CPP::MUI::Textbox::InputMode::Number:
						args.GetReturnValue().Set(2);
					case CPP::MUI::Textbox::InputMode::Decimal:
						args.GetReturnValue().Set(3);
					case CPP::MUI::Textbox::InputMode::Phone:
						args.GetReturnValue().Set(4);
					case CPP::MUI::Textbox::InputMode::Email:
						args.GetReturnValue().Set(5);
					case CPP::MUI::Textbox::InputMode::Password:
						args.GetReturnValue().Set(6);
					case CPP::MUI::Textbox::InputMode::Text:
					default:
						args.GetReturnValue().Set(1);
				}
			}
		}
	}
}
