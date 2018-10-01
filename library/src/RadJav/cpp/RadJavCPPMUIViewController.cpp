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
#include "cpp/RadJavCPPMUIViewController.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
#ifdef USE_V8
            ViewController::ViewController(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
            : GObject (jsEngine, args)
            {
            }
#endif
#ifdef USE_JAVASCRIPTCORE
            ViewController::ViewController(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
            : GObject (jsEngine, thisObj, numArgs, args)
            {
            }
#endif
            
            ViewController::ViewController(String name, String text, CPP::GUI::GObject *parent)
            : GObject(name, text, parent)
            {
            }
            
            void ViewController::addChild(GUI::GObject *child){
                _appObject->addChild(child);
            }
            
            void ViewController::create()
            {
				GUI::GObjectWidget* parentWin = nullptr;
				
				if (_parent != nullptr)
					parentWin = _parent->_appObj;
				
				ViewControllerFrame* object = RJNEW ViewControllerFrame(_parent, _text,
													Vector2(_transform->x, _transform->y),
													Vector2(_transform->width, _transform->height));
				
				object->setVisibility(_visible);
				
				//TODO: Maybe we need another Base for such type of GUI classes?
				//TODO: Need to add implementation
				//_appObj = object;
				
				linkWith(object);
				setup();
            }
            
            void ViewController::setPosition(RJINT x, RJINT y)
            {
                _appObject->setPosition(x, y);
            }
            
            CPP::Vector2 ViewController::getPosition()
            {
                CPP::Vector2 newpos;
                
                /// Do stuff here.
                
                return (newpos);
            }
            
            void ViewController::setSize(RJINT width, RJINT height)
            {
                _transform->setSize(width, height);
                
                _appObject->setSize(width, height);
            }
            
            CPP::Vector2 ViewController::getSize()
            {
                CPP::Vector2 size = _transform->getSize();
                
                /// Do stuff here.
                
                return (size);
            }
            
            void ViewController::setText(String text)
            {
                _text = text;
                
                /// Do stuff here.
            }
            
            String ViewController::getText()
            {
                String text = _text;
                
                /// Do stuff here.
                
                return (text);
            }
            
            void ViewController::setVisibility(RJBOOL visible)
            {
                _visible = visible;
                
                /// Do stuff here.
            }
            
            RJBOOL ViewController::getVisibility()
            {
                RJBOOL visible = _visible;
                
                /// Do stuff here.
                
                return (visible);
            }
            
            void ViewController::setEnabled(RJBOOL enabled)
            {
                /// Do stuff here.
            }
            
            RJBOOL ViewController::getEnabled()
            {
                RJBOOL enabled = false;
                
                /// Do stuff here.
                
                return (enabled);
            }
            
#if defined USE_V8 || defined USE_JAVASCRIPTCORE
            void ViewController::on(String event, RJ_FUNC_TYPE func)
            {
                /// Do stuff here.
            }
#endif
        }
    }
}

