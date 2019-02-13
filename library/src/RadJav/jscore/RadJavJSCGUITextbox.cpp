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
#include "jscore/RadJavJSCGUITextbox.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUITextbox.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			using CppGuiObject = CPP::GUI::Textbox;
			
			void Textbox::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", Textbox::create);
				
				JSC_CALLBACK(object, "_setInputMode", Textbox::setInputMode);
				JSC_CALLBACK(object, "_getInputMode", Textbox::getInputMode);
			}

			JSValueRef Textbox::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppGuiObject *appObject = RJNEW CppGuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				return promise;
			}
			
			JSValueRef Textbox::setInputMode(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppGuiObject *appObject = (CppGuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Textbox not initialized");
					return undefined;
				}
				
				JSValueRef inputModeValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!inputModeValue ||
					!JSValueIsNumber(ctx, inputModeValue))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "InputMode argument required");
					return undefined;
				}
				
				RJINT inputMode = JSC_JAVASCRIPT_ENGINE->jscValueToInt(inputModeValue);
				
				switch (inputMode)
				{
					case 1:
						appObject->setInputMode(CPP::GUI::Textbox::InputMode::Text);
						break;
					case 2:
						appObject->setInputMode(CPP::GUI::Textbox::InputMode::Number);
						break;
					case 3:
						appObject->setInputMode(CPP::GUI::Textbox::InputMode::Decimal);
						break;
					case 4:
						appObject->setInputMode(CPP::GUI::Textbox::InputMode::Phone);
						break;
					case 5:
						appObject->setInputMode(CPP::GUI::Textbox::InputMode::Email);
						break;
					case 6:
						appObject->setInputMode(CPP::GUI::Textbox::InputMode::Password);
						break;
					default:
						JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Unsupported InputMode");
				}
				return undefined;
			}
			
			JSValueRef Textbox::getInputMode(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppGuiObject *appObject = (CppGuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Textbox not initialized");
					return undefined;
				}
				
				auto inputMode = appObject->getInputMode();
				
				switch (inputMode)
				{
					case CPP::GUI::Textbox::InputMode::Number:
						return JSValueMakeNumber(ctx, 2.0);
					case CPP::GUI::Textbox::InputMode::Decimal:
						return JSValueMakeNumber(ctx, 3.0);
					case CPP::GUI::Textbox::InputMode::Phone:
						return JSValueMakeNumber(ctx, 4.0);
					case CPP::GUI::Textbox::InputMode::Email:
						return JSValueMakeNumber(ctx, 5.0);
					case CPP::GUI::Textbox::InputMode::Password:
						return JSValueMakeNumber(ctx, 6.0);
					case CPP::GUI::Textbox::InputMode::Text:
					default:
						return JSValueMakeNumber(ctx, 1.0);
				}
			}

		}
	}
}
