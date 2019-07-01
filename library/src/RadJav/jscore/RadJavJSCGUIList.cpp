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
#include "jscore/RadJavJSCGUIList.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIList.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			using CppGuiObject = CPP::GUI::List;
			
			void List::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", List::create);
				JSC_CALLBACK(object, "addColumn", List::addColumn);
				JSC_CALLBACK(object, "setColumns", List::setColumns);
				JSC_CALLBACK(object, "addRow", List::addRow);
				JSC_CALLBACK(object, "setRows", List::setRows);
				JSC_CALLBACK(object, "getSelectedRows", List::getSelectedRows);
				JSC_CALLBACK(object, "deleteRows", List::deleteRows);
			}

			JSValueRef List::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "List already created");
					return JSValueMakeUndefined(ctx);
				}
				
				appObject = RJNEW CppGuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				return promise;
			}

			JSValueRef List::addColumn(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				JSObjectRef column;
				
				if (value && JSValueIsString(ctx, value))
				{
					column = JSObjectMake(ctx, nullptr, nullptr);
					JSC_JAVASCRIPT_ENGINE->jscSetString(column, "text", parseJSCValue(ctx, value));
					
					if (argumentCount > 1)
					{
						RJINT width = JSC_JAVASCRIPT_ENGINE->jscValueToInt(arguments[1]);
						JSC_JAVASCRIPT_ENGINE->jscSetNumber(column, "width", width);
					}
					
					if (argumentCount > 2)
					{
						RJINT key = JSC_JAVASCRIPT_ENGINE->jscValueToInt(arguments[2]);
						JSC_JAVASCRIPT_ENGINE->jscSetNumber(column, "key", key);
					}
				}
				
				if (value && JSValueIsObject(ctx, value))
					column = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, value);
				
				JSObjectRef _columns = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_columns");
				RJINT _columnsSize = JSC_JAVASCRIPT_ENGINE->jscGetInt(_columns, "length");
				JSObjectSetPropertyAtIndex(ctx, _columns, _columnsSize, column, nullptr);
				
				String text = JSC_JAVASCRIPT_ENGINE->jscGetString(column, "text");
				RJINT width = JSC_JAVASCRIPT_ENGINE->jscGetInt(column, "width");
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (appObject != NULL)
					appObject->addColumn(text, width);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef List::setColumns(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (value && JSValueIsArray(ctx, value))
				{
					JSObjectRef array = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(value);
					JSC_JAVASCRIPT_ENGINE->jscSetObject(thisObject, "_columns", array);
					
					RJINT arraySize = JSC_JAVASCRIPT_ENGINE->jscGetInt(array, "length");
					
					Array<CPP::GUI::List::Column> *columns = RJNEW Array<CPP::GUI::List::Column> ();
					
					for (RJINT iIdx = 0; iIdx < arraySize; iIdx++)
					{
						JSValueRef columnValue = JSObjectGetPropertyAtIndex(ctx, array, iIdx, nullptr);
						JSObjectRef column = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(columnValue);
						
						String text = JSC_JAVASCRIPT_ENGINE->jscGetString(column, "text");
						RJINT width = JSC_JAVASCRIPT_ENGINE->jscGetInt(column, "width");
						CPP::GUI::List::Column col(text, width);
						
						columns->push_back(col);
					}
					
					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject != NULL)
					{
						DELETEOBJ (appObject->_columns);
						appObject->setColumns(columns);
					}
				}
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef List::addRow(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef itemValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (itemValue && JSValueIsObject(ctx, itemValue))
				{
					JSValueRef rowItemsValue = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(itemValue), "items");
					JSObjectRef rowItems = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(rowItemsValue);
					
					CPP::GUI::List::Row row;
					RJINT length = JSC_JAVASCRIPT_ENGINE->jscGetInt(rowItems, "length");
					
					for (RJUINT iIdx = 0; iIdx < length; iIdx++)
					{
						JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject( JSObjectGetPropertyAtIndex(ctx, rowItems, iIdx, nullptr));
						String str = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "text");
						
						row.items.push_back(CPP::GUI::List::Item (str));
					}
					
					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject != NULL)
						appObject->addRow(row);
				}
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef List::setRows(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef rowsValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (rowsValue && JSValueIsArray(ctx, rowsValue))
				{
					JSObjectRef rows = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(rowsValue);
					RJINT length = JSC_JAVASCRIPT_ENGINE->jscGetInt(rows, "length");

					Array<CPP::GUI::List::Row> *appRows = RJNEW Array<CPP::GUI::List::Row> ();
					
					for (RJUINT iIdx = 0; iIdx < length; iIdx++)
					{
						JSObjectRef item = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, JSObjectGetPropertyAtIndex(ctx, rows, iIdx, nullptr));
						JSObjectRef rowItems = JSC_JAVASCRIPT_ENGINE->jscGetObject(item, "items");
						RJINT rowItemsLength = JSC_JAVASCRIPT_ENGINE->jscGetInt(rowItems, "length");

						CPP::GUI::List::Row row;
						
						for (RJUINT iJdx = 0; iJdx < rowItemsLength; iJdx++)
						{
							String str = parseJSCValue(ctx, JSObjectGetPropertyAtIndex(ctx, rowItems, iJdx, nullptr));
							
							row.items.push_back(CPP::GUI::List::Item (str));
						}
						
						appRows->push_back(row);
					}
					
					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject != NULL)
						appObject->setRows(appRows);
					
					DELETEOBJ(appRows);
				}
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef List::getSelectedRows(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::GUI::List::Selection appSelection;
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				JSObjectRef result = nullptr;

				if (appObject != NULL)
				{
					appSelection = appObject->getSelectedRows();
					result = CPP::GUI::List::Selection::toJSCObject(JSC_JAVASCRIPT_ENGINE, ctx, &appSelection);
				}

				return result ? result : JSValueMakeUndefined(ctx);
			}

			JSValueRef List::deleteRows(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (value && JSValueIsObject(ctx, value))
				{
					JSObjectRef selection = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, value);

					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject != NULL)
					{
						CPP::GUI::List::Selection select = CPP::GUI::List::Selection::fromJSCObject(JSC_JAVASCRIPT_ENGINE, ctx, selection);
						appObject->deleteRows(select);
					}
				}
				
				return JSValueMakeUndefined(ctx);
			}
		}
	}
}
