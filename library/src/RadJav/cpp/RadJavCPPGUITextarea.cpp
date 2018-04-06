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
#include "cpp/RadJavCPPGUITextarea.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				TextareaFrame::TextareaFrame(wxWindow *parent, const wxString &text, const wxPoint &pos, const wxSize &size)
					: wxTextCtrl(parent, wxID_ANY, text, pos, size, wxTE_MULTILINE), GObjectBase()
				{
				}
		  
				void TextareaFrame::onText(wxCommandEvent &evt)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)evt.GetEventUserData();
					executeEvent(pevent);
				}
		  
				void TextareaFrame::onTextEnter(wxCommandEvent &evt)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)evt.GetEventUserData();
					executeEvent(pevent);
				}
		  
			#endif

			#ifdef USE_V8
				Textarea::Textarea(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
				}
			#endif

			Textarea::Textarea(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void Textarea::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					TextareaFrame *object = RJNEW TextareaFrame(parentWin, _text.towxString(),
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);

					_appObj = object;

					setup();
				#endif
			}

			void Textarea::setText(String text)
			{
				_text = text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						((TextareaFrame *)_appObj)->SetValue(text.towxString());
				#endif
			}

			String Textarea::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxString wxtext = ((TextareaFrame *)_appObj)->GetValue();
						text = parsewxString(wxtext);
					}
				#endif

				return (text);
			}

			#ifdef USE_V8
				void Textarea::on(String event, v8::Local<v8::Function> func)
				{
					CPP::GUI::TextareaFrame *obj = (CPP::GUI::TextareaFrame *)_appObj;

					obj->addNewEvent(event, obj, func);

					if (event == "onText")
					{
					        v8::Persistent<v8::Value> *pevent = obj->createEvent(event, func);
						obj->Connect(wxEVT_TEXT, wxCommandEventHandler(TextareaFrame::onText), (wxObject *)pevent);
					}

					if (event == "onTextEnter")
					{
					        v8::Persistent<v8::Value> *pevent = obj->createEvent(event, func);
						obj->Connect(wxEVT_TEXT_ENTER, wxCommandEventHandler(TextareaFrame::onTextEnter), (wxObject *)pevent);
					}
					
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::TextareaFrame, wxTextCtrl)
	wxEND_EVENT_TABLE()
#endif

