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
#ifndef _RADJAV_MUI_CPP_CHECKBOX_H_
#define _RADJAV_MUI_CPP_CHECKBOX_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIGObject.h"

#ifdef USE_IOS
	OBJC_CLASS(UISwitch);
	OBJC_CLASS(SwitchDelegate);
#elif defined USE_ANDROID
	#warning Add CheckBox implementation for Android platform
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			class RADJAV_EXPORT CheckBoxFrame : public GUI::GObjectWidget
												,public ChainedPtr
			{
			public:
				CheckBoxFrame(GUI::GObject *parent, RJBOOL checked, const Vector2 &pos, const Vector2 &size);
				~CheckBoxFrame();
				
				void setChecked(RJBOOL checked);
				RJBOOL getChecked() const;

				void setEnabled(RJBOOL enabled);
				RJBOOL getEnabled();
				
				bool bindEvent(const String& eventName, const GUI::Event* event);
				
				#ifdef USE_IOS
					UIView* getNativeWidget();
				#elif defined USE_ANDROID
					void* getNativeWidget();
				#endif
				
			private:
				#ifdef USE_IOS
					UISwitch* widget;
					SwitchDelegate* widgetDelegate;
				#elif defined USE_ANDROID
					//TODO: Wrap Android specific type here
					void* widget;
				#endif
			};
			
			class RADJAV_EXPORT CheckBox : public CPP::GUI::GObject
			{
			public:
				#ifdef USE_V8
					CheckBox(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
				#elif defined USE_JAVASCRIPTCORE
					CheckBox(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
				#endif
				
				CheckBox(String name, String text = "", CPP::GUI::GObject *parent = NULL);
				
				void create();
				void setChecked(RJBOOL checked);
				RJBOOL getChecked() const;
				
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					/// Execute when an event is triggered.
					void on(String event, RJ_FUNC_TYPE func);
				#endif
			};
		}
	}
}
#endif

