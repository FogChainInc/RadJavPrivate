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
#ifndef _RADJAV_MUI_CPP_VIEWCONTROLLER_H_
#define _RADJAV_MUI_CPP_VIEWCONTROLLER_H_

#include "cpp/RadJavCPPGUIGObject.h"

#ifdef USE_V8
	#include "v8/RadJavV8GUIGObject.h"
#elif defined USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCGUIGObject.h"
#endif

#ifdef USE_IOS
    OBJC_CLASS(UIViewController);
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
            class RADJAV_EXPORT ViewControllerFrame : public GUI::GObjectWidget
                                                    , public ChainedPtr
            {
            public:
                //TODO: Add correct parent type here, usually some base C++ container class (which still not created)
                ViewControllerFrame();
                ViewControllerFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size);
                ~ViewControllerFrame();

                void makeRootViewController();
                void makeRootViewControllerIfRootIsEmpty();
                void addChild(GUI::GObjectWidget *child);
                void presentViewControllerAnimated(CPP::GUI::GObject *presentedController);
                void dismissViewControllerAnimated();
                void setFont(CPP::Font *font);
                CPP::Font *getFont();
                void setPosition(RJINT x, RJINT y);
                void setPosition(CPP::Vector2 pos);
                CPP::Vector2 getPosition();
                RJINT getX();
                RJINT getY();
                void setSize(RJINT width, RJINT height);
                void setSize(CPP::Vector2 size);
                CPP::Vector2 getSize();
                RJINT getWidth();
                RJINT getHeight();
                void setText(String text);
                String getText();
                GUI::GObjectWidget *getParent();
                void setVisibility(RJBOOL visible);
                RJBOOL getVisibility();
                void setEnabled(RJBOOL enabled);
                RJBOOL getEnabled();
				
				bool bindEvent(const String& eventName, const GUI::Event* event);
                
                #ifdef USE_IOS
                    UIView* getNativeWidget();
                #elif defined USE_ANDROID
                    jobject getNativeWidget();
                #endif

            private:
                #ifdef USE_IOS
                    UIViewController* widget;
                #elif defined USE_ANDROID
                    jobject widget;
                #endif
            };
            
            
            class RADJAV_EXPORT ViewController : public CPP::GUI::GObject
            {
            public:
                #ifdef USE_V8
                    ViewController(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
                #endif
                #ifdef USE_JAVASCRIPTCORE
                    ViewController(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                #endif
                ViewController(String name, String text = "", CPP::GUI::GObject *parent = NULL);
                void makeRootViewController();
                void create();
                void addChild(CPP::GUI::GObject *child);
                void presentViewControllerAnimated(CPP::GUI::GObject *presentedController);
                void dismissViewControllerAnimated();
                void setPosition(RJINT x, RJINT y);
                CPP::Vector2 getPosition();
                void setSize(RJINT x, RJINT y);
                CPP::Vector2 getSize();
                void setText(String text);
                String getText();
                void setVisibility(RJBOOL visible);
                RJBOOL getVisibility();
                void setEnabled(RJBOOL enabled);
                RJBOOL getEnabled();
                
                #if defined USE_V8 || defined USE_JAVASCRIPTCORE
                    /// Execute when an event is triggered.
                    void on(String event, RJ_FUNC_TYPE func);
				#endif

                ViewControllerFrame* _appObject;
                String icon;
            };
        }
    }
}
#endif

