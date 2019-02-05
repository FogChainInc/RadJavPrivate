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
				#ifdef RADJAV_MOBILE
					class ViewFrame;

					class RADJAV_EXPORT NavigatorFrame : public ChainedPtr
					{
					public:
						/**
						 * Constructor.
						 * @param ViewFrame view. Initial view to be placed in stack.
						 */
						NavigatorFrame(ViewFrame* view);
						~NavigatorFrame();

						/** @method push. Wraps view in controller. Adds controller to stack with default animation
						 *
						 * @param View view to be added on top of navigation stack.
						 * @param Bool replace. If true - replaces topmost controller on stack
						 */
						void push(ViewFrame* view, bool replace);

						/** @method pop.
						 *
						 * @param ViewFrame view. If view is on navigation stack - pop back all the way to it, otherwise pop to root
						 */
						void pop(ViewFrame* view);
						/** @method pop. Removes last controller in stack with default animation
						 *
						 */
						void pop();

					private:
						#ifdef USE_IOS
							UIView *rootView;
							UINavigationController* widget;
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
				#endif

				#ifdef GUI_USE_WXWIDGETS
					/// The wxWidgets button to use.
					class RADJAV_EXPORT NavigatorFrame : public wxStaticText, public CPP::GUI::GObjectEvents, public ChainedPtr
					{
						public:
							NavigatorFrame(wxWindow *parent, const wxString &text, const wxPoint &pos, const wxSize &size);

							void onClick(wxMouseEvent &event);

						protected:
							wxDECLARE_EVENT_TABLE();
					};
				#endif

				class View;

				class RADJAV_EXPORT Navigator : public CPP::GUI::GObject
				{
					public:
						#ifdef USE_V8
							Navigator(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
                        #ifdef USE_JAVASCRIPTCORE
							Navigator(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                        #endif
					
						/**
					 	* Constructor.
					 	* @param ViewFrame view. Initial view to be placed in stack.
					 	*/
						Navigator(View* view, String name = "", String text = "", CPP::GUI::GObject *parent = NULL);

						void create();

						void setText(String text);

						/** @method push. Wraps view in controller. Adds controller to stack with default animation
					 	*
					 	* @param View view to be added on top of navigation stack.
					 	* @param Bool replace. If true - replaces topmost controller on stack
						 */
						void push(View* view, bool replace);
					
						/** @method pop.
						*
					 	* @param ViewFrame view. If view is on navigation stack - pop back all the way to it, otherwise pop to root
					 	*/
						void pop(View* view);
						/** @method pop. Removes last controller in stack with default animation
						 *
						 */
						void pop();

						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
							/// Execute when an event is triggered.
							void on(String event, RJ_FUNC_TYPE func);
						#endif

					private:
						View* rootView;
						Array<View *> views;

						#ifdef RADJAV_MOBILE
							NavigatorFrame* impl;
						#endif
				};
			}
		}
	}
#endif

