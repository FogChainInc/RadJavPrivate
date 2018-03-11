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

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				ListFrame::ListFrame(wxWindow *parent, const wxPoint &pos, const wxSize &size)
					: wxListView(parent, wxID_ANY, pos, size), GObjectBase()
				{
				}

				void ListFrame::onRowClick(wxListEvent &event)
				{
					#ifdef USE_V8
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					v8::Local<v8::Array> selectedRows = v8::Array::New(V8_JAVASCRIPT_ENGINE->isolate);

					ListFrame *object = (ListFrame *)event.GetEventObject();
					RJINT count = object->GetSelectedItemCount();

					for (RJINT iIdx = 0; iIdx < count; iIdx++)
					{
						RJLONG index = object->GetNextSelected(iIdx);

						if (index == -1)
							break;

						RJINT numCols = object->GetColumnCount();
						v8::Local<v8::Object> items = v8::Object::New(V8_JAVASCRIPT_ENGINE->isolate);
						v8::Local<v8::Array> itemsArray = v8::Array::New(V8_JAVASCRIPT_ENGINE->isolate);

						for (RJINT iJdx = 0; iJdx < numCols; iJdx++)
						{
							wxString itemText = object->GetItemText(index, iJdx);
							String temp = parsewxString(itemText);
							v8::Local<v8::Object> item = v8::Object::New(V8_JAVASCRIPT_ENGINE->isolate);

							item->Set(String("text").toV8String(V8_JAVASCRIPT_ENGINE->isolate), temp.toV8String(V8_JAVASCRIPT_ENGINE->isolate));
							itemsArray->Set(iJdx, item);
						}

						items->Set(String("items").toV8String(V8_JAVASCRIPT_ENGINE->isolate), itemsArray);
						selectedRows->Set(iIdx, items);
					}

					v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[1];
					args[0] = selectedRows;
					//args[0] = v8::Number::New (V8_JAVASCRIPT_ENGINE->isolate, temp);

					executeEvent(pevent, 1, args);
					DELETE_ARRAY(args);
					#endif
				}
			#endif

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

					setColumns(_columns);
					setup();
				#endif
			}

			#ifdef USE_V8
				void List::on(String event, v8::Local<v8::Function> func)
				{
					CPP::GUI::ListFrame *object = (CPP::GUI::ListFrame *)_appObj;

					object->addNewEvent(event, object, func);

					if (event == "rowClick")
					{
						v8::Persistent<v8::Value> *pevent = object->createEvent(event, func);

						#ifdef GUI_USE_WXWIDGETS
							object->Connect(wxEVT_COMMAND_LIST_ITEM_SELECTED, wxListEventHandler(ListFrame::onRowClick), (wxObject *)pevent);
						#endif
					}
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
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::ListFrame, wxListView)
	wxEND_EVENT_TABLE()
#endif

