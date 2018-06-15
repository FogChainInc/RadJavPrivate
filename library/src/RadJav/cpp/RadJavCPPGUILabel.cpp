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
#include "cpp/RadJavCPPGUILabel.h"

#include "RadJav.h"
#include "RadJavString.h"

#include <iostream>

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				LabelFrame::LabelFrame(wxWindow *parent, const wxString &text, const wxPoint &pos, const wxSize &size)
					: wxStaticText(parent, wxID_ANY, text, pos, size), GObjectBase()
				{
				}
		  
				void LabelFrame::onClick(wxMouseEvent &event)
				{
				  std::cout << "Label clicked" << std::endl;
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
			#endif

			#ifdef USE_V8
				Label::Label(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
				}
			#endif

			Label::Label(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void Label::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					LabelFrame *object = RJNEW LabelFrame(parentWin, _text.towxString(),
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);

					_appObj = object;

					setup();
				#endif
			}

			#ifdef USE_V8
				void Label::on(String event, v8::Local<v8::Function> func)
				{
					CPP::GUI::LabelFrame *object = (CPP::GUI::LabelFrame *)_appObj;

					object->addNewEvent(event, object, func);

					  std::cout << "Entering Label::on" << std::endl;
					if (event == "click")
					{
					  std::cout << "Connecting Label click event" << std::endl;
						object->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(LabelFrame::onClick), object->createEvent(event, func));
					}

					
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::LabelFrame, wxStaticText)
	wxEND_EVENT_TABLE()
#endif

