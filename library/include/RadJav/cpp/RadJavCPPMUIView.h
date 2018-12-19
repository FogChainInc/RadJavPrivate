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
#ifndef _RADJAV_MUI_CPP_VIEW_H_
	#define _RADJAV_MUI_CPP_VIEW_H_
	#include "cpp/RadJavCPPGUIGObject.h"

	#ifdef USE_V8
		#include "v8/RadJavV8GUIGObject.h"
	#elif defined USE_JAVASCRIPTCORE
		#include "jscore/RadJavJSCGUIGObject.h"
	#endif
#ifdef USE_IOS
OBJC_CLASS(ViewDelegate);
#endif


	namespace RadJAV
	{
		namespace CPP
		{
			namespace MUI
			{
				class RADJAV_EXPORT ViewFrame : public GUI::GObjectWidget
												, public ChainedPtr
                {
                public:
                    ViewFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size);
					ViewFrame(const String &text, const Vector2 &pos, const Vector2 &size);
                    ~ViewFrame();

					#ifdef USE_ANDROID
						void addChild(GUI::GObjectWidget *child);
					#endif

					void setText(String text);
					String getText();

					bool bindEvent(const String& eventName, const GUI::Event* event);

					#ifdef USE_IOS
						UIView* getNativeWidget();
						ViewDelegate* widgetDelegate;
					#endif

				private:
					#ifdef USE_IOS
						UIView* widget;
					
					#elif defined USE_ANDROID
						static void initNatives();

						static jmethodID nativeConstructor;
						static jmethodID nativeAddView;
						static jmethodID nativeRemoveView;

						static jclass nativeLayoutClass;
					#endif
                };
                
                
				class RADJAV_EXPORT View : public CPP::GUI::GObject
				{
					public:
						#ifdef USE_V8
							View(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
                        #ifdef USE_JAVASCRIPTCORE
                            View(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                        #endif
						View(String name, String text = "", CPP::GUI::GObject *parent = NULL);

						void create();
						void createMainView();

						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
                        	/// Execute when an event is triggered.
                        	void on(String event, RJ_FUNC_TYPE func);
						#endif
					
						///???
						String icon;
				};
			}
		}
	}
#endif

