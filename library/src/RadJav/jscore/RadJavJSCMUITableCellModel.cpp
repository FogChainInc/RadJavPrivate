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
#include "jscore/RadJavJSCMUITableCellModel.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUITableCellModel.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::TableCellModel;
			
            void TableCellModel::createJSCCallbacks(JSContextRef context, JSObjectRef object)
            {
                JSC_CALLBACK(object, "create", TableCellModel::create);
                
                JSC_CALLBACK(object, "setUsesAccessoryButton", TableCellModel::setUsesAccessoryButton);
                JSC_CALLBACK(object, "getUsesAccessoryButton", TableCellModel::getUsesAccessoryButton);
                
                JSC_CALLBACK(object, "setUsesCheckmark", TableCellModel::setUsesCheckmark);
                JSC_CALLBACK(object, "getUsesCheckmark", TableCellModel::getUsesCheckmark);
                
                JSC_CALLBACK(object, "setIsSelected", TableCellModel::setIsSelected);
                JSC_CALLBACK(object, "getIsSelected", TableCellModel::getIsSelected);
                
                JSC_CALLBACK(object, "setIsDeletable", TableCellModel::setIsDeletable);
                JSC_CALLBACK(object, "getIsDeletable", TableCellModel::getIsDeletable);
                
                JSC_CALLBACK(object, "setIsHeader", TableCellModel::setIsHeader);
                JSC_CALLBACK(object, "getIsHeader", TableCellModel::getIsHeader);
                
                JSC_CALLBACK(object, "setIsFooter", TableCellModel::setIsFooter);
                JSC_CALLBACK(object, "getIsFooter", TableCellModel::getIsFooter);
            }
			JSValueRef TableCellModel::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();
				
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
				
				return promise;
			}
            
            JSValueRef TableCellModel::getUsesAccessoryButton(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_usesAccessoryButton");
                
                if (appObject != NULL)
                    value = appObject->getUsesAccessoryButton();
                
                return JSValueMakeBoolean(ctx, value);
            }
            
            
            JSValueRef TableCellModel::setUsesAccessoryButton(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_usesAccessoryButton", value);
                
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                if (appObject != NULL)
                    appObject->setUsesAccessoryButton(value);
                
                return JSValueMakeUndefined(ctx);
            }
            
            
            JSValueRef TableCellModel::getUsesCheckmark(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_usesCheckmark");
                
                if (appObject != NULL)
                    value = appObject->getUsesCheckmark();
                
                return JSValueMakeBoolean(ctx, value);
            }
            
            
            JSValueRef TableCellModel::setUsesCheckmark(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_usesCheckmark", value);
                
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                if (appObject != NULL)
                    appObject->setUsesCheckmark(value);
                
                return JSValueMakeUndefined(ctx);
            }
            
            JSValueRef TableCellModel::getIsDeletable(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_isDeletable");
                
                if (appObject != NULL)
                    value = appObject->getIsDeletable();
                
                return JSValueMakeBoolean(ctx, value);
            }
            
            
            JSValueRef TableCellModel::setIsDeletable(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_isDeletable", value);
                
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                if (appObject != NULL)
                    appObject->setIsDeletable(value);
                
                return JSValueMakeUndefined(ctx);
            }
            
            JSValueRef TableCellModel::getIsSelected(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_isSelected");
                
                if (appObject != NULL)
                    value = appObject->getIsSelected();
                
                return JSValueMakeBoolean(ctx, value);
            }
            
            
            JSValueRef TableCellModel::setIsSelected(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_isSelected", value);
                
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                if (appObject != NULL)
                    appObject->setIsSelected(value);
                
                return JSValueMakeUndefined(ctx);
            }
            
            
            JSValueRef TableCellModel::getIsHeader(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_isHeader");
                
                if (appObject != NULL)
                    value = appObject->getIsHeader();
                
                return JSValueMakeBoolean(ctx, value);
            }
            
            
            JSValueRef TableCellModel::setIsHeader(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_isHeader", value);
                
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                if (appObject != NULL)
                    appObject->setIsHeader(value);
                
                return JSValueMakeUndefined(ctx);
            }
            
            JSValueRef TableCellModel::getIsFooter(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_isFooter");
                
                if (appObject != NULL)
                    value = appObject->getIsFooter();
                
                return JSValueMakeBoolean(ctx, value);
            }
            
            
            JSValueRef TableCellModel::setIsFooter(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_isFooter", value);
                
                CppMuiObject *appObject = (CppMuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
                
                if (appObject != NULL)
                    appObject->setIsFooter(value);
                
                return JSValueMakeUndefined(ctx);
            }
           
		}
	}
}
