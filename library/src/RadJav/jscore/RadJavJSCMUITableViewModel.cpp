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
#include "jscore/RadJavJSCMUITableViewModel.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUITableViewModel.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::TableViewModel;
			
            void TableViewModel::createJSCCallbacks(JSContextRef context, JSObjectRef object)
            {
                JSC_CALLBACK(object, "create", TableViewModel::create);
                JSC_CALLBACK(object, "setCellModels", TableViewModel::setCellModels);
            }
            
            JSValueRef TableViewModel::setCellModels(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
       
                if (argumentCount)
                {
                    std::vector<CPP::MUI::TableCellModel*> *arrayData = new std::vector<CPP::MUI::TableCellModel*>();
                    
                    if (JSValueIsArray(ctx, arguments[0]))
                    {
                        JSObjectRef array = JSValueToObject(ctx, arguments[0], nullptr);
                        JSStringRef arrayLengthProperty = JSStringCreateWithUTF8CString("length");
                        
                        JSValueRef lengthValue = JSObjectGetProperty(ctx, array, arrayLengthProperty, nullptr);
                        JSStringRelease(arrayLengthProperty);
                        
                        size_t arraySize = JSValueToNumber(ctx, lengthValue, nullptr);
                        
                        for (size_t i = 0; i < arraySize; i++)
                        {
                            JSObjectRef item = JSValueToObject(ctx,JSObjectGetPropertyAtIndex(ctx, array, i, nullptr),nullptr);
                            CPP::MUI::TableCellModel * cell = (CPP::MUI::TableCellModel*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, item, "_appObj");
                            if (cell != NULL)arrayData->push_back(cell);
                        }
                    }
                    
                    CppMuiObject *appObject = (CppMuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                    appObject->setCellModels(arrayData);
                }
                
                return JSValueMakeUndefined(ctx);
            }
            
            
        
            
			JSValueRef TableViewModel::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();
				
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
				
				return promise;
			}
           
		}
	}
}
