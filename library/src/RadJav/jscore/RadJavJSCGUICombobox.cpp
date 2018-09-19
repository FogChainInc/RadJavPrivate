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
#include "jscore/RadJavJSCGUICombobox.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUICombobox.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			using CppGuiObject = CPP::GUI::Combobox;
			
			void Combobox::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", Combobox::create);

				JSC_CALLBACK(object, "addItem", Combobox::addItem);
				JSC_CALLBACK(object, "setItems", Combobox::setItems);
				JSC_CALLBACK(object, "deleteItem", Combobox::deleteItem);
				JSC_CALLBACK(object, "getItem", Combobox::getItem);
				JSC_CALLBACK(object, "getItems", Combobox::getItems);
				JSC_CALLBACK(object, "getNumItems", Combobox::getNumItems);
				JSC_CALLBACK(object, "clear", Combobox::clear);
				JSC_CALLBACK(object, "setSelectedItemIndex", Combobox::setSelectedItemIndex);
				JSC_CALLBACK(object, "getSelectedItemIndex", Combobox::getSelectedItemIndex);
			}

			JSValueRef Combobox::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppGuiObject *appObject = RJNEW CppGuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				return promise;
			}

			JSValueRef Combobox::addItem(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);

				String text;
				
				if (value &&
					JSValueIsString(ctx, value))
				{
					text = parseJSCValue(ctx, value);
				}
				else if (value &&
						 JSValueIsObject(ctx, value))
				{
					text = JSC_JAVASCRIPT_ENGINE->jscGetString(JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, value), "text");
				}
					
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->addItem(text);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Combobox::setItems(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				JSValueRef value = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!value ||
					!JSValueIsArray(ctx, value))
				{
					return undefined;
				}
				
				JSObjectRef arrayJs = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(value);
				JSValueRef arraySizeJs = JSC_JAVASCRIPT_ENGINE->jscGetValue(arrayJs, "length");
				
				const RJINT arraySize = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, arraySizeJs);
				
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
				{
					Array<CPP::GUI::Combobox::Item> *items = RJNEW Array<CPP::GUI::Combobox::Item>();

					for (RJINT iIdx = 0; iIdx < arraySize; iIdx++)
					{
						JSValueRef itemJs = JSObjectGetPropertyAtIndex(ctx, arrayJs, iIdx, nullptr);
						JSObjectRef itemObjectJs = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(itemJs);
						String text = JSC_JAVASCRIPT_ENGINE->jscGetString(itemObjectJs, "text");
						items->push_back(CPP::GUI::Combobox::Item(text));
					}

					DELETEOBJ(appObject->_items);
					appObject->setItems(items);
				}
				
				return undefined;
			}

			JSValueRef Combobox::deleteItem(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (value &&
					JSValueIsNumber(ctx, value))
				{
					RJINT index = JSC_JAVASCRIPT_ENGINE->jscValueToInt(value);

					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject != NULL)
						appObject->deleteItem(index);
				}
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Combobox::getItem(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (value &&
					JSValueIsNumber(ctx, value))
				{
					RJINT index = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, value);
					
					JSObjectRef obj = JSObjectMake(ctx, nullptr, nullptr);
					
					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject != NULL)
					{
						CPP::GUI::Combobox::Item item = appObject->getItem(index);
						JSC_JAVASCRIPT_ENGINE->jscSetString(obj, "name", item.name);
						JSC_JAVASCRIPT_ENGINE->jscSetString(obj, "text", item.text);
						
						return obj;
					}
				}
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Combobox::getItems(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					return CPP::GUI::Combobox::Item::toJSCArray(JSC_JAVASCRIPT_ENGINE, ctx, appObject->getItems());

				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Combobox::getNumItems(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJUINT size = 0;
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					size = appObject->getNumItems();

				return JSValueMakeNumber(ctx, size);
			}

			JSValueRef Combobox::clear(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->clear();
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Combobox::setSelectedItemIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (value &&
					JSValueIsNumber(ctx, value))
				{
					RJINT index = JSC_JAVASCRIPT_ENGINE->jscValueToInt(value);
					
					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject != NULL)
						appObject->setSelectedItemIndex(index);
				}
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Combobox::getSelectedItemIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJINT index = 0;
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					index = appObject->getSelectedItemIndex();

				return JSValueMakeNumber(ctx, index);
			}
		}
	}
}
#endif

