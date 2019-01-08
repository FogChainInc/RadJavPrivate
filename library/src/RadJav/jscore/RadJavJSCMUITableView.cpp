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
#include "jscore/RadJavJSCMUITableView.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUITableView.h"
#include "cpp/RadJavCPPMUITableViewModel.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::TableView;
			
            void TableView::createJSCCallbacks(JSContextRef context, JSObjectRef object)
            {
                JSC_CALLBACK(object, "create", TableView::create);
                JSC_CALLBACK(object, "setModel", TableView::setModel);
				JSC_CALLBACK(object, "setDelegate", TableView::setDelegate);
            }
			JSValueRef TableView::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);

				//TODO: this is temporary for TableView second version
				/*
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
				
				return promise;
				 */

				return JSValueMakeUndefined(ctx);
			}
            
            JSValueRef TableView::setModel(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                if (argumentCount > 0){
                    JSObjectRef argument =  JSValueToObject(ctx, arguments[0], exception);
                    CPP::MUI::TableViewModel *model = (CPP::MUI::TableViewModel *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, argument, "_appObj");
                    if (model != NULL){
                        
                        appObject->setModel(model);
                    }
                    
                }
				
				/*
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
                
                return promise;
				 */
				
				return JSValueMakeUndefined(ctx);
            }

			JSValueRef TableView::setDelegate(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "TableView not initialized");
					return undefined;
				}
				
				JSValueRef delegateJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (delegateJs)
				{
					JSObjectRef delegateJsObj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(delegateJs);
					if(delegateJsObj && JSObjectIsFunction(ctx, delegateJsObj))
					{
						appObject->setDelegate(delegateJsObj);
					}
				}
				
				return undefined;
			}
		}
	}
}
