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
#include "cpp/RadJavCPPMUINavigationViewController.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
#ifdef USE_V8
            NavigationViewController::NavigationViewController(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
            : GObject (jsEngine, args)
            {
            }
#endif
#ifdef USE_JAVASCRIPTCORE
            NavigationViewController::NavigationViewController(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
            : GObject (jsEngine, thisObj, numArgs, args)
            {
                
            }
#endif
            
            NavigationViewController::NavigationViewController(String name, String text, CPP::GUI::GObject *parent)
            : GObject(name, text, parent)
            {
            }
            
                    
            void NavigationViewController::create()
            {
				GUI::GObjectWidget* parentWin = nullptr;
				
				if (_parent != nullptr)
					parentWin = _parent->_appObj;
				
				NavigationViewControllerFrame* object = RJNEW NavigationViewControllerFrame(_parent, _text,
													Vector2(_transform->x, _transform->y),
													Vector2(_transform->width, _transform->height));
				
				object->setVisibility(_visible);
				
				//TODO: Maybe we need another Base for such type of GUI classes?
				//TODO: Need to add implementation
				_appObject = object;
				_appObj = object;
				linkWith(object);
				setup();
            }
            void NavigationViewController::setRootViewController(CPP::GUI::GObject *presentedController){
                _appObject->setRootViewController(presentedController);
            }
            void NavigationViewController::pushViewController(CPP::GUI::GObject *presentedController){
                _appObject->pushViewController(presentedController);
            }
            void NavigationViewController::popViewController(){
                _appObject->popViewController();
            }
         
            
#if defined USE_V8 || defined USE_JAVASCRIPTCORE
            void NavigationViewController::on(String event, RJ_FUNC_TYPE func)
            {
                /// Do stuff here.
            }
#endif
        }
    }
}

