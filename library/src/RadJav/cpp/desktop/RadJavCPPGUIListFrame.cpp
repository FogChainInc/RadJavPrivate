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
#include "cpp/desktop/RadJavCPPGUIListFrame.h"

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
					: wxListView(parent, wxID_ANY, pos, size)
				{
				}

				void ListFrame::onRowClick(wxListEvent &event)
				{
					#ifdef USE_V8

					Event *pevent = (Event *)event.GetEventUserData();
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

				void ListFrame::onRowFocused(wxListEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}

				void ListFrame::onRowRightClick(wxListEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}

				void ListFrame::onRowMiddleClick(wxListEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::ListFrame, wxListView)
	wxEND_EVENT_TABLE()
#endif
