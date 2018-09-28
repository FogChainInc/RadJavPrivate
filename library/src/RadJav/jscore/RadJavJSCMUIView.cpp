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
#include "jscore/RadJavJSCMUIView.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUIView.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::View;
			
			void View::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", View::create);
                JSC_CALLBACK(object, "setSize", View::setSize);
                JSC_CALLBACK(object, "getSize", View::getSize);
                JSC_CALLBACK(object, "setPosition", View::setPosition);
                JSC_CALLBACK(object, "getPosition", View::getPosition);
                JSC_CALLBACK(object, "addChild", View::addChild);
                
                //JSC_CALLBACK(object, "setParent", View::setParent);
                //JSC_CALLBACK(object, "getParent", View::getParent);

                
//                JSC_CALLBACK(object, "getX", View::getX);
//                JSC_CALLBACK(object, "getY", View::getY);
//
//                JSC_CALLBACK(object, "getWidth", View::getWidth);
//                JSC_CALLBACK(object, "getHeight", View::getHeight);
               
//                JSC_CALLBACK(object, "setFont", View::setFont);
//                JSC_CALLBACK(object, "getFont", View::getFont);

//
//                JSC_CALLBACK(object, "setText", View::setText);
//                JSC_CALLBACK(object, "getText", View::getText);
//                JSC_CALLBACK(object, "getParent", View::getParent);
//                JSC_CALLBACK(object, "getAppObj", View::getAppObj);
//                JSC_CALLBACK(object, "setVisibility", View::setVisibility);
//                JSC_CALLBACK(object, "getVisibility", View::getVisibility);
//                JSC_CALLBACK(object, "setEnabled", View::setEnabled);
//                JSC_CALLBACK(object, "getEnabled", View::getEnabled);
//                JSC_CALLBACK(object, "on", View::on);
//                JSC_CALLBACK(object, "destroy", View::destroy);

			}

			JSValueRef View::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();
				
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
				
				return promise;
			}
            
            JSValueRef View::setSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                int width = 0;
                int height = 0;

                
                if (argumentCount >= 2){
                    width =  JSValueToNumber(ctx, arguments[0], exception);
                    height =  JSValueToNumber(ctx, arguments[1], exception);
                }
                
                appObject->setSize(width, height);
                
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
            
            
            JSValueRef View::getSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                 CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
            }
            
            JSValueRef View::setPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                int x = 0;
                int y = 0;
                
                
                if (argumentCount >= 2){
                    x =  JSValueToNumber(ctx, arguments[0], exception);
                    y =  JSValueToNumber(ctx, arguments[1], exception);
                }
                
                appObject->setPosition(x, y);
                
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
            
            
            JSValueRef View::getPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                
            }
            
            
            
            JSValueRef View::addChild(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
 
                if (argumentCount > 0){
                   JSObjectRef argument =  JSValueToObject(ctx, arguments[0], exception);
                    CppMuiObject *childAppObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, argument, "_appObj");
                    if (childAppObject != NULL){
                        
                        appObject->addChild(childAppObject);
                    }
                    
                }

                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
            
		}
	}
}
