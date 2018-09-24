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
#ifndef _RADJAV_MUI_CPP_BUTTON_H_
	#define _RADJAV_MUI_CPP_BUTTON_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "cpp/RadJavCPPGUIGObject.h"

	namespace RadJAV
	{
		namespace CPP
		{
			namespace MUI
			{
				class RADJAV_EXPORT Button : public CPP::GUI::GObject
				{
					public:
						#ifdef USE_V8
							Button(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
                        #ifdef USE_JAVASCRIPTCORE
                            Button(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                        #endif
						Button(String name, String text = "", CPP::GUI::GObject *parent = NULL);

						void create();
						void setPosition(RJINT x, RJINT y);
						CPP::Vector2 getPosition();
						void setSize(RJINT x, RJINT y);
						CPP::Vector2 getSize();
						void setText(String text);
						String getText();
						void setVisibility(RJBOOL visible);
						RJBOOL getVisibility();
						void setEnabled(RJBOOL enabled);
						RJBOOL getEnabled();

						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
                        	/// Execute when an event is triggered.
                        	void on(String event, RJ_FUNC_TYPE func);
						#endif

						String icon;
				};
			}
		}
	}
#endif

