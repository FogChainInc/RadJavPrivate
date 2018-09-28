/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#include "v8/RadJavV8MUIView.h"

#include "RadJav.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPMUIView.h"

namespace RadJAV
{
	namespace V8
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::View;
			
			void View::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				V8_CALLBACK(object, "create", View::create);
                V8_CALLBACK(object, "setSize", View::setSize);
                V8_CALLBACK(object, "getSize", View::getSize);
                V8_CALLBACK(object, "setPosition", View::setPosition);
                V8_CALLBACK(object, "getPosition", View::getPosition);
                
                
                //V8_CALLBACK(object, "setParent", View::setParent);
                //V8_CALLBACK(object, "getParent", View::getParent);

                
//                V8_CALLBACK(object, "getX", View::getX);
//                V8_CALLBACK(object, "getY", View::getY);
//
//                V8_CALLBACK(object, "getWidth", View::getWidth);
//                V8_CALLBACK(object, "getHeight", View::getHeight);
               
//                V8_CALLBACK(object, "setFont", View::setFont);
//                V8_CALLBACK(object, "getFont", View::getFont);

//
//                V8_CALLBACK(object, "setText", View::setText);
//                V8_CALLBACK(object, "getText", View::getText);
//                V8_CALLBACK(object, "getParent", View::getParent);
//                V8_CALLBACK(object, "getAppObj", View::getAppObj);
//                V8_CALLBACK(object, "setVisibility", View::setVisibility);
//                V8_CALLBACK(object, "getVisibility", View::getVisibility);
//                V8_CALLBACK(object, "setEnabled", View::setEnabled);
//                V8_CALLBACK(object, "getEnabled", View::getEnabled);
//                V8_CALLBACK(object, "on", View::on);
//                V8_CALLBACK(object, "destroy", View::destroy);

			}

			JSValueRef View::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = V8_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
				
				return promise;
			}
            
            JSValueRef View::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(thisObject, "_appObj");
                
                int width = 0;
                int height = 0;

                
                if (argumentCount >= 2){
                    width =  JSValueToNumber(arguments[0], exception);
                    height =  JSValueToNumber(arguments[1], exception);
                }
                
                appObject->setSize(width, height);
                
                JSObjectRef _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = V8_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
            
            
            JSValueRef View::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                 CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(thisObject, "_appObj");
            }
            
            JSValueRef View::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(thisObject, "_appObj");
                
                int x = 0;
                int y = 0;
                
                
                if (argumentCount >= 2){
                    x =  JSValueToNumber(arguments[0], exception);
                    y =  JSValueToNumber(arguments[1], exception);
                }
                
                appObject->setPosition(x, y);
                
                JSObjectRef _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = V8_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
            
            
            JSValueRef View::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                
            }
            
            
            
            JSValueRef View::setParent(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(thisObject, "_appObj");
 
                if (argumentCount > 0){
                   JSObjectRef argument =  JSValueToObject(arguments[0], exception);
                  
                }
                //TODO: implement lookup and addSubview: according to it
                //appObject->setParent(x, y);
                
                JSObjectRef _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = V8_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
            
		}
	}
}
