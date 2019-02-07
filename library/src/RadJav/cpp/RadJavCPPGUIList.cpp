/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#include "cpp/RadJavCPPGUIList.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef GUI_USE_WXWIDGETS
	#include "cpp/desktop/RadJavCPPGUIListFrame.h"
#elif defined USE_ANDROID
	#include "cpp/android/RadJavCPPGUIListFrame.h"
#elif defined USE_IOS
	#include "cpp/ios/RadJavCPPGUIListFrame.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef USE_V8
				List::List(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
					_canSort = true;
					_hasCheckBoxes = false;
					_columns = RJNEW Array<Column>();

					v8::Local<v8::Array> columns = v8::Local<v8::Array>::Cast(jsEngine->v8GetValue(args.This(), "_columns"));
					_canSort = jsEngine->v8GetBool(args.This(), "_canSort");
					RJUINT length = 0;

					if (jsEngine->v8IsNull(columns) == false)
						length = columns->Length();

					for (RJUINT iIdx = 0; iIdx < length; iIdx++)
					{
						v8::Local<v8::Object> column = v8::Local<v8::Object>::Cast(columns->Get(iIdx));
						String text = jsEngine->v8GetString(column, "text");
						RJINT width = jsEngine->v8GetInt(column, "width");

						if (width == 0)
							width = -1;

						_columns->push_back(List::Column (text, width));
					}
				}
			#elif defined USE_JAVASCRIPTCORE
				List::List(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
					: GObject (jsEngine, thisObject, argumentCount, arguments)
				{
					_canSort = true;
					_hasCheckBoxes = false;
					_columns = RJNEW Array<Column>();
					
					JSObjectRef columns = jsEngine->jscGetObject(thisObject, "_columns");

					_canSort = jsEngine->jscGetBool(thisObject, "_canSort");
					RJUINT length = 0;
					
					if (jsEngine->jscIsNull(columns) == false)
						length = jsEngine->jscGetInt(columns, "length");
					
					for (RJUINT iIdx = 0; iIdx < length; iIdx++)
					{
						JSObjectRef column = jsEngine->jscCastValueToObject( JSObjectGetPropertyAtIndex(jsEngine->globalContext, columns, iIdx, nullptr));
						String text = jsEngine->jscGetString(column, "text");
						RJINT width = jsEngine->jscGetInt(column, "width");
						
						if (width == 0)
							width = -1;
						
						_columns->push_back(List::Column (text, width));
					}
				}
			#endif

			List::List(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
				_canSort = true;
				_hasCheckBoxes = false;
				_columns = RJNEW Array<Column> ();
			}

			void List::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					ListFrame *object = RJNEW ListFrame(parentWin, 
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);

					_appObj = object;
				
					linkWith(object);

					setColumns(_columns);
					setup();
				#endif
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void List::on(String event, RJ_FUNC_TYPE func)
				{
					#ifdef GUI_USE_WXWIDGETS
						CPP::GUI::ListFrame *object = (CPP::GUI::ListFrame *)_appObj;

						object->addNewEvent(event, object, func);

						if (event == "rowClick")
						{
						
								object->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(ListFrame::onRowClick), object->createEvent(event, func));
						
						}
					
						if (event == "rowFocused")
						{
							#ifdef GUI_USE_WXWIDGETS
								object->Connect(wxEVT_COMMAND_LIST_ITEM_FOCUSED, wxListEventHandler(ListFrame::onRowFocused), object->createEvent(event, func));
							#endif
						}
						if (event == "rowRightClick")
						{
							#ifdef GUI_USE_WXWIDGETS
								object->Connect(wxEVT_COMMAND_LIST_ITEM_RIGHT_CLICK, wxListEventHandler(ListFrame::onRowRightClick), object->createEvent(event, func));
							#endif
						}
						if (event == "rowMiddleClick")
						{
							#ifdef GUI_USE_WXWIDGETS
								object->Connect(wxEVT_COMMAND_LIST_ITEM_MIDDLE_CLICK, wxListEventHandler(ListFrame::onRowMiddleClick), object->createEvent(event, func));
							#endif
						}
					#endif
				}
			#endif

			void List::addColumn(String column, RJINT width)
			{
				addColumn(List::Column (column, width));
			}

			void List::addColumn(Column column)
			{
				#ifdef GUI_USE_WXWIDGETS
					ListFrame *object = (ListFrame *)_appObj;

					if (column.width == 0)
						column.width = -1;

					object->AppendColumn(column.text.towxString(), wxLIST_FORMAT_LEFT, column.width);
				#endif

				_columns->push_back(column);
			}

			void List::setColumns(Array<Column> *columns)
			{
				#ifdef GUI_USE_WXWIDGETS
					ListFrame *object = (ListFrame *)_appObj;

					if (object->GetColumnCount () > 0)
						object->DeleteAllColumns();

					for (RJUINT iIdx = 0; iIdx < columns->size (); iIdx++)
					{
						Column column = columns->at(iIdx);

						object->AppendColumn(column.text.towxString(), wxLIST_FORMAT_LEFT, column.width);
					}
				#endif

				_columns = columns;
			}

			void List::addRow(Row row)
			{
				#ifdef GUI_USE_WXWIDGETS
					ListFrame *object = (ListFrame *)_appObj;

					RJINT rowId = object->GetItemCount();
					wxListItem listItem;
					listItem.SetId(rowId);
					listItem.SetText(String("test").towxString());
					object->InsertItem(listItem);

					for (RJUINT iIdx = 0; iIdx < row.items.size (); iIdx++)
					{
						List::Item item = row.items.at(iIdx);
						String str = item.text;

						object->SetItem(rowId, iIdx, str.towxString());
					}
				#endif
			}

			void List::setRows(Array<Row> *rows)
			{
				#ifdef GUI_USE_WXWIDGETS
					ListFrame *object = (ListFrame *)_appObj;

					for (RJUINT iIdx = 0; iIdx < rows->size (); iIdx++)
					{
						Row row = rows->at(iIdx);
						RJUINT length = row.items.size ();

						object->DeleteAllItems();

						for (RJUINT iJdx = 0; iJdx < length; iJdx++)
						{
							String str = row.items.at (iJdx).text;

							wxListItem listItem;
							listItem.SetText(str.towxString());
							object->InsertItem(listItem);
						}
					}
				#endif
			}

			List::Selection List::getSelectedRows()
			{
				List::Selection selection;
				selection._appObj = this;

				#ifdef GUI_USE_WXWIDGETS
					ListFrame *object = (ListFrame *)_appObj;
					RJINT count = object->GetSelectedItemCount();

					for (RJINT iIdx = 0; iIdx < count; iIdx++)
					{
						RJLONG index = object->GetNextSelected(iIdx);

						if (index == -1)
							break;

						selection.indicies.push_back(index);
					}
				#endif

				return (selection);
			}

			void List::deleteRows(Selection selection)
			{
				#ifdef GUI_USE_WXWIDGETS
					ListFrame *object = (ListFrame *)_appObj;
					RJUINT length = selection.indicies.size ();

					for (RJUINT iIdx = 0; iIdx < length; iIdx++)
					{
						RJINT index = selection.indicies.at (iIdx);

						object->DeleteItem(index);
					}
				#endif
			}

			#ifdef USE_V8
			List::Selection List::Selection::fromV8Object(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> selection)
			{
				v8::Local<v8::Array> rows = v8::Local<v8::Array>::Cast(V8_JAVASCRIPT_ENGINE->v8GetValue(selection, "_appObj"));
				RJUINT length = rows->Length();
				List::Selection newSelection;

				for (RJUINT iIdx = 0; iIdx < length; iIdx++)
				{
					v8::Local<v8::Integer> value = v8::Local<v8::Integer>::Cast(rows->Get(iIdx));
					RJINT index = value->Value();

					newSelection.indicies.push_back(index);
				}

				return (newSelection);
			}

			v8::Local<v8::Object> List::Selection::toV8Object(V8JavascriptEngine *jsEngine, List::Selection *selection)
			{
				v8::Local<v8::Object> gui = jsEngine->v8GetObject(V8_RADJAV, "GUI");
				v8::Local<v8::Object> list = jsEngine->v8GetObject(gui, "List");
				v8::Local<v8::Object> selectionV8 = jsEngine->v8GetObject(list, "Selection");
				v8::Local<v8::Object> result = jsEngine->v8CallAsConstructor(selectionV8, 0, NULL);
				v8::Local<v8::Array> selected = v8::Array::New(jsEngine->isolate);
				RJINT count = selection->indicies.size ();

				for (RJINT iIdx = 0; iIdx < count; iIdx++)
				{
					RJLONG index = selection->indicies.at (iIdx);

					if (index == -1)
						break;

					selected->Set(iIdx, v8::Integer::New(jsEngine->isolate, index));
				}

				result->Set(String("_appObj").toV8String(jsEngine->isolate), selected);

				return (result);
			}
			#elif defined USE_JAVASCRIPTCORE
			List::Selection List::Selection::fromJSCObject(JSCJavascriptEngine *jsEngine, JSContextRef ctx, JSObjectRef selection)
			{
				JSObjectRef rows = jsEngine->jscGetObject(selection, "_appObj");
				RJUINT length = jsEngine->jscGetInt(rows, "length");
				List::Selection newSelection;
				
				for (RJUINT iIdx = 0; iIdx < length; iIdx++)
				{
					JSValueRef value = JSObjectGetPropertyAtIndex(ctx, rows, iIdx, nullptr);
					RJINT index = jsEngine->jscValueToInt(value);
					
					newSelection.indicies.push_back(index);
				}
				
				return newSelection;
			}
			
			JSObjectRef List::Selection::toJSCObject(JSCJavascriptEngine *jsEngine, JSContextRef ctx, List::Selection *selection)
			{
				JSObjectRef gui = jsEngine->jscGetObject(JSC_RADJAV, "GUI");
				JSObjectRef list = jsEngine->jscGetObject(gui, "List");
				JSObjectRef selectionV8 = jsEngine->jscGetObject(list, "Selection");
				JSObjectRef result = jsEngine->jscCallAsConstructor(selectionV8, 0, NULL);
				
				RJINT count = selection->indicies.size ();
				JSValueRef values[count];
				
				for (RJINT iIdx = 0; iIdx < count; iIdx++)
				{
					RJLONG index = selection->indicies.at (iIdx);
					
					if (index == -1)
					{
						values[iIdx] = JSValueMakeUndefined(ctx);
						break;
					}
					
					values[iIdx] = JSValueMakeNumber(ctx, index);
				}
				
				JSObjectRef selected = JSObjectMakeArray(ctx, count, values, nullptr);

				jsEngine->jscSetObject(result, "_appObj", selected);
				
				return result;
			}
			#endif
		}
	}
}
