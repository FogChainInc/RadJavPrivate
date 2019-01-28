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
#ifndef _RADJAV_MUI_CPP_LABEL_H_
#define _RADJAV_MUI_CPP_LABEL_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIGObject.h"

#ifdef USE_IOS
	OBJC_CLASS(UILabel);
	OBJC_CLASS(LabelDelegate);
#elif defined USE_ANDROID
	JNI_CLASS(jobject);
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			class RADJAV_EXPORT LabelFrame : public GUI::GObjectWidget
											,public ChainedPtr
			{
			public:
				/**
				 * Constructor.
				 * @param GObjectWidget parent. Constructed object will be added to view hierarchy of parent
				 * @param String text. Display text, has default value of empty string.
				 * @param Vector2 pos Initial position.
				 * @param Vector2 size Initial size.
				 */
				LabelFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size);
				~LabelFrame();

				/** @method setText
				 * Setter for button text.
				 * @param String text
				 */
				void setText(String text);
				
				/** @method getText
				 * Getter for buttonText.
				 * @return String text
				 */
				String getText();
				
				/** @method setFont
				 * Setter for button font.
				 * @param Font font
				 */
				void setFont(CPP::Font *font);
				/** @method getFont
				 * Getter for button font.
				 * @return Font
				 */
				CPP::Font *getFont();
				#ifdef USE_IOS
					/** @method setEnabled
				 	* Setter for enabled.
				 	* @param BOOL enabled
				 	*/
					void setEnabled(RJBOOL enabled);
					/** @method getEnabled
				 	* Getter for enabled.
				 	* @return Bool enabled
				 	*/
					RJBOOL getEnabled();
				#endif

				bool bindEvent(const String& eventName, const GUI::Event* event);

				#ifdef USE_IOS
					UIView* getNativeWidget();
				#endif

			private:
				#ifdef USE_IOS
					UILabel* widget;
					//TODO: do we need to handle events of the UILabel?
					//LabelDelegate* widgetDelegate;
				#elif defined USE_ANDROID
					static jclass nativeTextViewClass;

					static jmethodID nativeConstructor;
					static jmethodID nativeSetText;
					static jmethodID nativeGetText;
				#endif
			};
			
			/**
			 * @ingroup group_mui_cpp
			 * @brief Label class.
			 */
			class RADJAV_EXPORT Label : public CPP::GUI::GObject
			{
			public:
				#ifdef USE_V8
					Label(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
				#elif defined USE_JAVASCRIPTCORE
					Label(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
				#endif
				/**
				 * Constructor.
				 * @param String name. This goes to superclass constructor
				 * @param String text. Display text, has default value of empty string.
				 * @param GObjectWidget parent. Constructed object will be added to view hierarchy of parent.
				 */
				Label(String name, String text = "", CPP::GUI::GObject *parent = NULL);
				
				void create();
				
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					/// Execute when an event is triggered.
					void on(String event, RJ_FUNC_TYPE func);
				#endif
			};
		}
	}
}
#endif

