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
#ifndef _RADJAV_MUI_CPP_BOTTOM_NAVIGATOR_H_
	#define _RADJAV_MUI_CPP_BOTTOM_NAVIGATOR_H_
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
				class View;
				class RADJAV_EXPORT BottomNavigatorFrame : public ChainedPtr
                {
                public:
					BottomNavigatorFrame(ViewFrame* view);
                    ~BottomNavigatorFrame();
					/** @method addTab
					 * Adds tab to tabbar. Title property of View will be used for icon caption.
					 * @param View this view will be wrapped in VIewController and displayed.
					 * @param String absolute path to icon to be displayed on tabbar
					 */
                    void addTab(View* view, String icon);
					/** @method getSelectedTab
					 * Returns index of currently selected tab.
					 * @return int currently selected tab
					 */
					int getSelectedTab();

				private:
					#ifdef USE_IOS
                        UIView *rootView;
						UITabBarController* widget;
	
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

				class RADJAV_EXPORT BottomNavigator : public ChainedPtr
				{
					public:
						#ifdef USE_V8
							BottomNavigator(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
                        #ifdef USE_JAVASCRIPTCORE
							BottomNavigator(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                        #endif
						BottomNavigator(View* view);

						void create();
						/** @method addTab
					 	* Adds tab to tabbar. Title property of View will be used for icon caption.
						* @param View this view will be wrapped in VIewController and displayed.
					 	* @param String absolute path to icon to be displayed on tabbar
					 	*/
						void addTab(View* view, String icon);
						/** @method getSelectedTab
					 	* Returns index of currently selected tab.
					 	* @return int currently selected tab
					 	*/
						int getSelectedTab();

					private:
						BottomNavigatorFrame* impl;
						View* rootView;
				};
			}
		}
	}
#endif

