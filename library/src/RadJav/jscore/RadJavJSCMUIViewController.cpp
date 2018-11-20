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
#include "jscore/RadJavJSCMUIViewController.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUIViewController.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::ViewController;
			
			void ViewController::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", ViewController::create);
                JSC_CALLBACK(object, "makeRootViewController", ViewController::makeRootViewController);
                JSC_CALLBACK(object, "presentViewControllerAnimated", ViewController::presentViewControllerAnimated);
                JSC_CALLBACK(object, "dismissViewControllerAnimated", ViewController::dismissViewControllerAnimated);
			}

			JSValueRef ViewController::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();
				
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
				
				return promise;
			}
            
            JSValueRef ViewController::makeRootViewController(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
 
                appObject->makeRootViewController();
                
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
            JSValueRef ViewController::presentViewControllerAnimated(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                if (argumentCount > 0){
                    JSObjectRef argument =  JSValueToObject(ctx, arguments[0], exception);
                    CppMuiObject *childAppObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, argument, "_appObj");
                    if (childAppObject != NULL){
                        
                        appObject->presentViewControllerAnimated(childAppObject);
                    }
                    
                }
                
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
            
            JSValueRef ViewController::dismissViewControllerAnimated(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
        
                appObject->dismissViewControllerAnimated();
                
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
            }
		}
	}
}
