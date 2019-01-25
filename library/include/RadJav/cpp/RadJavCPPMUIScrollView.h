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
#ifndef _RADJAV_MUI_CPP_SCROLLVIEW_H_
#define _RADJAV_MUI_CPP_SCROLLVIEW_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIGObject.h"

	#ifdef USE_V8
		#include "v8/RadJavV8GUIGObject.h"
	#elif defined USE_JAVASCRIPTCORE
		#include "jscore/RadJavJSCGUIGObject.h"
    #endif

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{

			class RADJAV_EXPORT ScrollViewFrame : public GUI::GObjectWidget
					,public ChainedPtr
			{
			public:
				/**
				 * Constructor.
				 * @param String name. This goes to superclass constructor
				 * @param String text. Unused.
				 * @param GObjectWidget parent. Unused.
				 */
				ScrollViewFrame(GUI::GObjectWidget *parent, const Vector2 &pos, const Vector2 &size);
				~ScrollViewFrame();

				void setEnabled(RJBOOL enabled);
				RJBOOL getEnabled();

				#ifdef USE_ANDROID
					void addChild(GUI::GObjectWidget *child);
				#endif

				/** @method setContentSize
				 * Setter for content size. Contentsize is a measure of how much
				 * @param Vector2 size. New content size.
				 */
                //void setContentSize(const CPP::Vector2& size);

				/** @method getContentSize
				 * Getter for content size
				 * @return Vector2 contentSize.
				 */
                //CPP::Vector2 getContentSize() const;

				bool bindEvent(const String& eventName, const GUI::Event* event);

				#ifdef USE_IOS
					UIView* getNativeWidget();
				#elif defined USE_ANDROID
					jobject getNativeWidget();
				#endif

			private:
				#ifdef USE_IOS
					UIScrollView* widget;
					ScrollViewDelegate* widgetDelegate;
				#elif defined USE_ANDROID
					static void initNatives();
                    
                    static jmethodID nativeConstructor;
                    static jmethodID nativeAddView;
                    static jmethodID nativeRemoveView;	
					
					static jclass nativeLayoutClass;

                //    static jmethodID nativeSetContentSize;
                //    static jmethodID nativeGetContentSize;
				#endif
			};

			class RADJAV_EXPORT ScrollView : public CPP::GUI::GObject
			{
			public:
				#ifdef USE_V8
					ScrollView(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
				#elif defined USE_JAVASCRIPTCORE
					ScrollView(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
				#endif
				
				/**
				 * Constructor.
				 * @param String name. This goes to superclass constructor
				 * @param String text. Unused.
				 * @param GObjectWidget parent. Unused.
				 */
				ScrollView(String name, String text = "", CPP::GUI::GObject *parent = NULL);
				
				void create();

				/** @method setContentSize
				 * Setter for content size. Contentsize is a measure of how much
				 * @param Vector2 size. New content size.
				 */
                void setContentSize(const CPP::Vector2& size);

				/** @method getContentSize
				 * Getter for content size
				 * @return Vector2 contentSize.
				 */
                CPP::Vector2 getContentSize() const;

				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					/// Execute when an event is triggered.
					void on(String event, RJ_FUNC_TYPE func);
				#endif
			};

		}
	}
}

#endif
