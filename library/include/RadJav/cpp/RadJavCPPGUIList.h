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
#ifndef _RADJAV_GUI_CPP_LIST_H_
	#define _RADJAV_GUI_CPP_LIST_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "cpp/RadJavCPPGUIGObject.h"

	#ifdef GUI_USE_WXWIDGETS
		#include <wx/wx.h>
		#include <wx/listctrl.h>
	#endif

	namespace RadJAV
	{
		namespace CPP
		{
			namespace GUI
			{
				#ifdef GUI_USE_WXWIDGETS
					/// The wxWidgets button to use.
					class RADJAV_EXPORT ListFrame : public wxListView, public GObjectBase, public ChainedPtr
					{
						public:
							ListFrame(wxWindow *parent, const wxPoint &pos, const wxSize &size);

							void onRowClick(wxListEvent &event);
							void onRowRightClick(wxListEvent &event);
							void onRowMiddleClick(wxListEvent &event);
							void onRowFocused(wxListEvent &event);

						protected:
							wxDECLARE_EVENT_TABLE();
					};
				#endif

				class RADJAV_EXPORT List : public CPP::GUI::GObject
				{
					public:
						#ifdef USE_V8
							List(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
						List(String name, String text = "", CPP::GUI::GObject *parent = NULL);

						void create();

						#ifdef USE_V8
							void on(String event, v8::Local<v8::Function> func);
						#endif

						class RADJAV_EXPORT Item
						{
							public:
								inline Item()
								{
									name = "";
									text = "";
								}

								inline Item(String text)
								{
									name = "";
									this->text = text;
								}

								String name;
								String text;
						};

						class RADJAV_EXPORT Row
						{
							public:
								inline Row()
								{
								}

								inline Row(const Row &row)
								{
									items = row.items;
								}

								Array<Item> items;
						};

						class RADJAV_EXPORT Column
						{
							public:
								inline Column()
								{
									text = "";
									width = 0;
									key = NULL;
								}

								inline Column(String text, RJINT width = 0)
								{
									this->text = text;
									this->width = width;
									key = NULL;
								}

								String text;
								RJINT width;
								void *key;
						};

						class RADJAV_EXPORT Selection
						{
							public:
								inline Selection()
								{
									_appObj = NULL;
								}

								#ifdef USE_V8
									static Selection fromV8Object(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> selection);
									static v8::Local<v8::Object> toV8Object(V8JavascriptEngine *jsEngine, List::Selection *selection);
								#endif

								GObject *_appObj;
								Array<RJLONG> indicies;
						};

						void addColumn(String column, RJINT width = 0);
						void addColumn(Column column);
						void setColumns(Array<Column> *columns);

						void addRow(Row row);
						void setRows(Array<Row> *rows);
						Selection getSelectedRows();
						void deleteRows(Selection selection);

						RJBOOL _hasCheckBoxes;
						RJBOOL _canSort;
						Array<Column> *_columns;
				};
			}
		}
	}
#endif

