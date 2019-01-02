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
#ifndef _RADJAV_MUI_CPP_NAVIGATIONVIEWCONTROLLER_H_
#define _RADJAV_MUI_CPP_NAVIGATIONVIEWCONTROLLER_H_

#include "cpp/RadJavCPPGUIGObject.h"

#ifdef USE_V8
	#include "v8/RadJavV8GUIGObject.h"
#elif defined USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCGUIGObject.h"
#endif

#include "RadJavCPPMUIViewController.h"

#ifdef USE_IOS
	OBJC_CLASS(UINavigationController);
#elif defined USE_ANDROID
	JNI_CLASS(jobject);
#endif


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            
            //TODO: Add some base class here with common UI controls interface
            class RADJAV_EXPORT NavigationViewControllerFrame : public ViewControllerFrame
            {
            public:
                //TODO: Add correct parent type here, usually some base C++ container class (which still not created)
                NavigationViewControllerFrame();
				/**
				 * Constructor. If parent is not provided will install itself as rootviewcontroller to key window.
				 * @param GObjectWidget parent. Constructed object will be added to view hierarchy of parent
				 * @param String text. Unused.
				 * @param Vector2 pos Initial position.
				 * @param Vector2 size Initial size.
				 */
                NavigationViewControllerFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size);
                ~NavigationViewControllerFrame();
                void create();
				/** @method setRootViewController
				 *
				 * @param Gobject controller to be set as root of navigation stack
				 */
                void setRootViewController(CPP::GUI::GObject *presentedController);
				/** @method pushViewController. Adds controller to stack with default animation
				 *
				 * @param Gobject controller to be added on top of navigation stack.
				 */
                void pushViewController(CPP::GUI::GObject *presentedController);
				/** @method popViewController. Removes last controller in stack with default animation
				 *
				 */
                void popViewController();
                
				#ifdef USE_IOS
                	UIView* getNativeWidget();
				#elif defined USE_ANDROID
                	jobject getNativeWidget();
				#endif
                
            private:
				#ifdef USE_IOS
                	UINavigationController* widget;
				#elif defined USE_ANDROID
                	jobject widget;
				#endif
            };
            
            
            class RADJAV_EXPORT NavigationViewController : public CPP::GUI::GObject
            {
            public:
				#ifdef USE_V8
                	NavigationViewController(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
				#endif
				#ifdef USE_JAVASCRIPTCORE
                	NavigationViewController(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
				#endif
				/**
				 * Constructor.
				 * @param String name. This goes to superclass constructor
				 * @param String text. Unused.
				 * @param GObjectWidget parent. Unused.
				 */
                NavigationViewController(String name, String text = "", CPP::GUI::GObject *parent = NULL);
                
                #if defined USE_V8 || defined USE_JAVASCRIPTCORE
                	/// Execute when an event is triggered.
                	void on(String event, RJ_FUNC_TYPE func);
				#endif
                void create();
				/** @method setRootViewController
				 *
				 * @param Gobject controller to be set as root of navigation stack
				 */
				void setRootViewController(CPP::GUI::GObject *presentedController);
				/** @method pushViewController. Adds controller to stack with default animation
				 *
				 * @param Gobject controller to be added on top of navigation stack.
				 */
				void pushViewController(CPP::GUI::GObject *presentedController);
				/** @method popViewController. Removes last controller in stack with default animation
				 *
				 */
				void popViewController();
                
                NavigationViewControllerFrame* _appObject;
            };
        }
    }
}
#endif

