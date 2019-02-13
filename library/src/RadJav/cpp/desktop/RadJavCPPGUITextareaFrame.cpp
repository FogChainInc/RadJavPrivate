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
#include "cpp/desktop/RadJavCPPGUITextareaFrame.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				TextareaFrame::TextareaFrame(GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
				: wxTextCtrl(parent ? parent->getNativeWidget() : nullptr, wxID_ANY, text.towxString(), wxPoint(pos.x, pos.y), wxSize(size.x, size.y), wxTE_MULTILINE)
				{
				}

				void TextareaFrame::setText(String text)
				{
					SetValue(text.towxString());
				}

				String TextareaFrame::getText()
				{
					wxString text = GetValue();
					return parsewxString(text);
				}

				wxWindow* TextareaFrame::getNativeWidget()
				{
					return this;
				}

				void TextareaFrame::onText(wxCommandEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}

				void TextareaFrame::onTextEnter(wxCommandEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::TextareaFrame, wxTextCtrl)
	wxEND_EVENT_TABLE()
#endif
