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
#include "jscore/RadJavJSCMUICheckbox.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUICheckbox.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::Checkbox;
			
			void Checkbox::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", Checkbox::create);

				JSC_CALLBACK(object, "setChecked", Checkbox::setChecked);
				JSC_CALLBACK(object, "isChecked", Checkbox::isChecked);
			}
			
			JSValueRef Checkbox::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

			JSValueRef Checkbox::setChecked(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject *appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Checkbox not initialized");
					return undefined;
				}

				JSValueRef checkedValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);

				if (checkedValue &&
					(JSValueIsBoolean(ctx, checkedValue) ||
					 JSValueIsNumber(ctx, checkedValue)))
				{
					RJBOOL checked = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(checkedValue);
					
					appObject->setChecked(checked);
					return undefined;
				}
				
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Parameter required");
				
				return undefined;
			}

			JSValueRef Checkbox::isChecked(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppMuiObject *appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Checkbox not initialized");
					return JSValueMakeUndefined(ctx);
				}
				
				return JSValueMakeBoolean(ctx, appObject->getChecked());
			}
		}
	}
}
