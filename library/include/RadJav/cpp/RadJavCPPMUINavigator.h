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
#ifndef _RADJAV_MUI_CPP_NAVIGATOR_H_
	#define _RADJAV_MUI_CPP_NAVIGATOR_H_
	#include "cpp/RadJavCPPGUIGObject.h"

	#ifdef USE_V8
		#include "v8/RadJavV8GUIGObject.h"
	#elif defined USE_JAVASCRIPTCORE
		#include "jscore/RadJavJSCGUIGObject.h"
	#endif

	#include <stack>


	namespace RadJAV
	{
		namespace CPP
		{
			namespace MUI
			{
				class ViewFrame;

				class RADJAV_EXPORT NavigatorFrame : public ChainedPtr
                {
                public:
					NavigatorFrame(ViewFrame* view);
                    ~NavigatorFrame();

                    void push(ViewFrame* view, bool replace);

                    void pop(ViewFrame* view);

					void pop();

				private:
					#ifdef USE_IOS
						//UIView* widget;
						//TODO: do we need to handle events of the UIView?
						//ViewDelegate* widgetDelegate;
					#elif defined USE_ANDROID
						static jmethodID nativeConstructor;
						static jmethodID nativeAddView;
						static jmethodID nativeRemoveView;
						static jmethodID nativeSetLayoutTransition;

						static jclass nativeLayoutTransitionClass;
						static jclass nativeLayoutClass;

						jobject rootView;
						std::stack<ViewFrame*> viewStack;
					#endif
                };
                

				class View;

				class RADJAV_EXPORT Navigator : public ChainedPtr
				{
					public:
						#ifdef USE_V8
							Navigator(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
                        #ifdef USE_JAVASCRIPTCORE
							Navigator(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                        #endif
						Navigator(View* view);

						void create();

						void push(View* view, bool replace);
						void pop(View* view);
						void pop();

					private:
						NavigatorFrame* impl;
						View* rootView;
				};
			}
		}
	}
#endif

