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
#ifndef _RADJAV_MUI_CPP_TableViewController_H_
#define _RADJAV_MUI_CPP_TableViewController_H_
#include "cpp/RadJavCPPGUIGObject.h"
#include "jscore/RadJavJSCGUIGObject.h"
#include "RadJavCPPMUIViewController.h"
#ifdef USE_IOS
#ifdef __OBJC__
#define OBJC_CLASS(name) @class name
#else
#define OBJC_CLASS(name) typedef struct objc_object name
#endif

OBJC_CLASS(UINavigationController);
#elif defined USE_ANDROID
#warning Add Button implementation for Android platform
#endif


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            
            //TODO: Add some base class here with common UI controls interface
            class RADJAV_EXPORT TableViewControllerFrame : public ViewControllerFrame
            {
            public:
                //TODO: Add correct parent type here, usually some base C++ container class (which still not created)
                TableViewControllerFrame();
                TableViewControllerFrame(void *parent, const String &text, const Vector2 &pos, const Vector2 &size);
                ~TableViewControllerFrame();
                void create();
                void setRootViewController(CPP::GUI::GObject *presentedController);
                void pushViewController(CPP::GUI::GObject *presentedController);
                void popViewController();
                
#ifdef USE_IOS
                UIView* getNativeWidget();
                
#elif defined USE_ANDROID
                //TODO: Add correct type here for Android
                void* getNativeWidget();
#endif
                
            private:
#ifdef USE_IOS
                UINavigationController* widget;
#elif defined USE_ANDROID
                //TODO: Wrap Android specific type here
#endif
            };
            
            
            class RADJAV_EXPORT TableViewController : public CPP::GUI::GObject
            {
            public:
#ifdef USE_V8
                TableViewController(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
#endif
#ifdef USE_JAVASCRIPTCORE
                TableViewController(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
#endif
                TableViewController(String name, String text = "", CPP::GUI::GObject *parent = NULL);
                
#if defined USE_V8 || defined USE_JAVASCRIPTCORE
                /// Execute when an event is triggered.
                void on(String event, RJ_FUNC_TYPE func);
#endif
                void create();
                void setRootViewController(CPP::GUI::GObject *presentedController);
                void pushViewController(CPP::GUI::GObject *presentedController);
                void popViewController();                
                
                TableViewControllerFrame* _appObject;
                String icon;
            };
        }
    }
}
#endif

