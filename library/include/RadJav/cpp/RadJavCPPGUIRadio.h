/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#ifndef _RADJAV_GUI_CPP_RADIO_H_
	#define _RADJAV_GUI_CPP_RADIO_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "cpp/RadJavCPPGUIGObject.h"

	namespace RadJAV
	{
		namespace CPP
		{
			namespace GUI
			{
				/**
				 * @ingroup group_gui_cpp
				 * @brief Radio class.
				 */
				class RADJAV_EXPORT Radio : public CPP::GUI::GObject
				{
					public:
						#ifdef USE_V8
							Radio(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#elif defined USE_JAVASCRIPTCORE
							Radio(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[]);
						#endif
						Radio(String name, String text = "", CPP::GUI::GObject *parent = NULL);

						void create();

						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
							void on(String event, RJ_FUNC_TYPE func);
						#endif

						void setChecked(RJBOOL checked);
						RJBOOL isChecked();

						RJBOOL _checked;
				};
			}
		}
	}
#endif

