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
#include "jscore/RadJavJSCMUIBottomNavigator.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUIBottomNavigator.h"
#include "cpp/RadJavCPPMUIView.h"
#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::BottomNavigator;
			using CppMuiViewObject = CPP::MUI::View;

			void BottomNavigator::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", BottomNavigator::init);
				JSC_CALLBACK(object, "_addTab", BottomNavigator::addTab);
			}

			JSValueRef BottomNavigator::init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef BottomNavigator::getSelectedTabIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject* appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if(!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "BottomNavigator not initialized");
					return undefined;
				}
				
				return JSValueMakeNumber(ctx, appObject->getSelectedTab());
			}

			JSValueRef BottomNavigator::addTab(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject* appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if(!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "BottomNavigator not initialized");
					return undefined;
				}

				//Extract view parameter
				CppMuiViewObject *view = nullptr;

				if(argumentCount)
				{
					JSValueRef viewJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
					if (!JSC_JAVASCRIPT_ENGINE->jscIsNull(viewJs) &&
						JSValueIsObject(ctx, viewJs))
					{
						//Extract view parameter
						JSObjectRef viewObjJs = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(viewJs);
						view = (CppMuiViewObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, viewObjJs, "_appObj");
					}
				}

				//Extract replace parameter
				String icon = "";
				if(argumentCount > 1)
				{
					JSValueRef iconJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
					if(!JSC_JAVASCRIPT_ENGINE->jscIsNull(iconJs) &&
					   JSValueIsString(ctx, iconJs))
					{
						icon = parseJSCValue(ctx, iconJs);
					}
				}

				//Sanity check
				if(!view)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "View parameter required");
					return undefined;
				}

				//Call native implementation
				appObject->addTab(view, icon);
				
				return undefined;
			}
		}
	}
}
