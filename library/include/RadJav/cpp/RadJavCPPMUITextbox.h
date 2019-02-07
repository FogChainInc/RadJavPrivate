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
#ifndef _RADJAV_MUI_CPP_TEXTBOX_H_
#define _RADJAV_MUI_CPP_TEXTBOX_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			/**
			 * @ingroup group_mui_cpp
			 * @brief Textbox class.
			 */
			class RADJAV_EXPORT Textbox : public CPP::GUI::GObject
			{
			public:
				enum class InputMode
				{
					Text,
					Number,
					Decimal,
					Phone,
					Email,
					Password
				};
				
			public:
				#ifdef USE_V8
					Textbox(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
				#elif defined USE_JAVASCRIPTCORE
					Textbox(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
				#endif
				/**
				 * Constructor.
				 * @param String name. This goes to superclass constructor.
				 * @param String text. Initial text.
				 * @param GObjectWidget parent.
				 */
				Textbox(String name, String text = "", CPP::GUI::GObject *parent = NULL);
				
				void create();
				
				/** @method setInputMode
				 * Setter for input mode. Viable options are Text, Number, Decimal, Phone, Email, Password
				 * @param InputMode input mode.
				 */
				void setInputMode(Textbox::InputMode mode);
				/** @method getInputMode
				 * Getter for input mode. Viable options are Text, Number, Decimal, Phone, Email, Password
				 * @return Font
				 */
				InputMode getInputMode() const;
				
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					/// Execute when an event is triggered.
					void on(String event, RJ_FUNC_TYPE func);
				#endif
			};
		}
	}
}
#endif
