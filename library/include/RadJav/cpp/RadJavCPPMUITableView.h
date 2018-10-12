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
#ifndef _RADJAV_MUI_CPP_TableView_H_
#define _RADJAV_MUI_CPP_TableView_H_

#include "cpp/RadJavCPPGUIGObject.h"

#ifdef USE_V8
	#include "v8/RadJavV8GUIGObject.h"
#elif defined USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCGUIGObject.h"
#endif

#ifdef USE_IOS
#ifdef __OBJC__
#define OBJC_CLASS(name) @class name
#else
#define OBJC_CLASS(name) typedef struct objc_object name
#endif

OBJC_CLASS(UITableView);
#elif defined USE_ANDROID
#warning Add forward declaration of Android specific class/type
#endif


	namespace RadJAV
	{
		namespace CPP
		{
			namespace MUI
			{
				class RADJAV_EXPORT TableViewFrame : public GUI::GObjectWidget
												, public ChainedPtr
                {
                public:
                    TableViewFrame(GUI::GObject *parent, const String &text, const Vector2 &pos, const Vector2 &size);
                    ~TableViewFrame();

					void setText(String text);
					String getText();

					bool bindEvent(const String& eventName, const GUI::Event* event);

					#ifdef USE_IOS
						UIView* getNativeWidget();
					#elif defined USE_ANDROID
						void* getNativeWidget();
					#endif

				private:
					#ifdef USE_IOS
						UITableView* widget;
						//TODO: do we need to handle events of the UITableView?
						//TableViewDelegate* widgetDelegate;
					#elif defined USE_ANDROID
                    	//TODO: Wrap Android specific type here
					#endif
                };
                
                
				class RADJAV_EXPORT TableView : public CPP::GUI::GObject
				{
					public:
						#ifdef USE_V8
							TableView(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
                        #ifdef USE_JAVASCRIPTCORE
                            TableView(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                        #endif
						TableView(String name, String text = "", CPP::GUI::GObject *parent = NULL);

						void create();

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

