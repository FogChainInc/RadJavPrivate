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
#include "cpp/RadJavCPPGUIRadio.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				RadioFrame::RadioFrame(wxWindow *parent, const wxString &text, const wxPoint &pos, const wxSize &size)
					: wxRadioButton(parent, wxID_ANY, text, pos, size), GObjectBase()
				{
				}

				void RadioFrame::onChanged(wxCommandEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}

			#endif

			#ifdef USE_V8
				Radio::Radio(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
					_checked = V8_JAVASCRIPT_ENGINE->v8GetBool(args.This(), "_checked");
				}
			#endif

			Radio::Radio(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
				_checked = false;
			}

			void Radio::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					RadioFrame *object = RJNEW RadioFrame(parentWin, _text.towxString(),
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);
					object->SetValue(_checked);

					_appObj = object;

					setup();
				#endif
			}

			#ifdef USE_V8
				void Radio::on(String event, v8::Local<v8::Function> func)
				{
					CPP::GUI::RadioFrame *object = (CPP::GUI::RadioFrame *)_appObj;

					object->addNewEvent(event, object, func);

					if (event == "change")
					{
						#ifdef GUI_USE_WXWIDGETS
						object->Connect(wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler(RadioFrame::onChanged), object->createEvent(event, func));
						#endif
					}


				}
			#endif

			void Radio::setChecked(RJBOOL checked)
			{
				#ifdef GUI_USE_WXWIDGETS
					CPP::GUI::RadioFrame *object = (CPP::GUI::RadioFrame *)_appObj;

					object->SetValue(checked);
				#endif
			}

			RJBOOL Radio::isChecked()
			{
			  
				RJBOOL value = false;

				#ifdef GUI_USE_WXWIDGETS
					CPP::GUI::RadioFrame *object = (CPP::GUI::RadioFrame *)_appObj;
					value = object->GetValue();
				#endif

				return (value);
			}
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::RadioFrame, wxRadioButton)
	wxEND_EVENT_TABLE()
#endif

