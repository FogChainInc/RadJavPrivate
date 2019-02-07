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
#include "cpp/desktop/RadJavCPPGUIWindowFrame.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef __WXOSX__
	/// @todo Fix this later. This is throwing issues on OSX.
	//#import <AppKit/AppKit.h>
	//#import <AppKit/NSScreen.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				WindowFrame::WindowFrame(const wxString &text, const wxPoint &pos, const wxSize &size)
					: wxFrame(NULL, wxID_ANY, text, pos, size)
				{
				}

				void WindowFrame::onClose(wxCloseEvent &evt)
				{
					if (IsTopLevel() == true)
					{
						RadJav::javascriptEngine->exit(0);

						return;
					}

					Destroy();
				}

				void WindowFrame::onJSClose(wxCloseEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					
					#ifdef USE_V8
						v8::Local<v8::Value> result = executeEvent(pevent);

						if (result.IsEmpty() == false)
						{
							if ((result->IsNull() == false) && (result->IsUndefined() == false))
							{
								v8::Local<v8::Boolean> change = v8::Local<v8::Boolean>::Cast(result);

								if (change->Value() == false)
									evt.Veto();
							}
						}
					#endif
					
					#ifdef USE_JAVASCRIPTCORE
						JSValueRef result = executeEvent(pevent);
					
						if (result != NULL)
						{
							if ((JSValueIsNull (JSC_JAVASCRIPT_ENGINE->globalContext, result) == false) &&
								(JSValueIsUndefined (JSC_JAVASCRIPT_ENGINE->globalContext, result) == false))
							{
								RJBOOL change = JSC_JAVASCRIPT_ENGINE->jscParseBool (result);

								if (change == false)
									evt.Veto();
							}
						}
					#endif
				}

				void WindowFrame::onJSMinimized(wxIconizeEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}

				void WindowFrame::onJSMaximized(wxMaximizeEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}

				void WindowFrame::onClick(wxMouseEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}

				void WindowFrame::onMenuSelected(wxCommandEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::WindowFrame, wxFrame)
		EVT_CLOSE(RadJAV::CPP::GUI::WindowFrame::onClose)
	wxEND_EVENT_TABLE()
#endif
