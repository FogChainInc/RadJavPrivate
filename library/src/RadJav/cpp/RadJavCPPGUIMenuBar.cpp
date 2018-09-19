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
#include "cpp/RadJavCPPGUIMenuBar.h"

#include "RadJav.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIWindow.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
  			#ifdef GUI_USE_WXWIDGETS
				MenuBarFrame::MenuBarFrame(const wxString &text)
				  : wxMenuBar(), GObjectBase ()
				{
				}

				void MenuBarFrame::onMenuOpen(wxMenuEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void MenuBarFrame::onMenuClose(wxMenuEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void MenuBarFrame::onMenuHighLight(wxMenuEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void MenuBarFrame::onMenuSelected(wxCommandEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}

			#endif

			#ifdef USE_V8
				MenuBar::MenuBar(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
				}
			#elif defined USE_JAVASCRIPTCORE
				MenuBar::MenuBar(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
					: GObject (jsEngine, thisObject, argumentCount, arguments)
				{
				}
			#endif

			MenuBar::MenuBar(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void MenuBar::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxFrame *parentWin = NULL;

					if (_parent != NULL)
						parentWin = dynamic_cast<wxFrame*>(_parent->_appObj->GetParent());

					MenuBarFrame *object = RJNEW MenuBarFrame("");
					parentWin->SetMenuBar(object);

					_appObj = object;
				
					setup();
				#endif
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void MenuBar::on(String event, RJ_FUNC_TYPE func)
				{
					#ifdef GUI_USE_WXWIDGETS

						CPP::GUI::MenuBarFrame *object = (CPP::GUI::MenuBarFrame *)_appObj;
						CPP::GUI::MenuBarFrame *obj = (CPP::GUI::MenuBarFrame *)object->GetParent();
					
						obj->addNewEvent(event, object, func);

						if (event == "menuopen")
						{
							#ifdef GUI_USE_WXWIDGETS
							obj->Connect(wxEVT_MENU_OPEN, wxMenuEventHandler(MenuBarFrame::onMenuOpen), object->createEvent(event, func));
							#endif
						
						}
						if (event == "menuclose")
						{
							#ifdef GUI_USE_WXWIDGETS
							obj->Connect(wxEVT_MENU_CLOSE, wxMenuEventHandler(MenuBarFrame::onMenuClose), object->createEvent(event, func));
							#endif
						}
						if (event == "menuhighlight")
						{
							#ifdef GUI_USE_WXWIDGETS
							obj->Connect(wxEVT_MENU_HIGHLIGHT, wxMenuEventHandler(MenuBarFrame::onMenuHighLight), object->createEvent(event, func));
							#endif
						}
						if (event == "menuselected")
						{
							#ifdef GUI_USE_WXWIDGETS
							obj->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBarFrame::onMenuSelected), object->createEvent(event, func));
							#endif
						}

					#endif
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::MenuBarFrame, wxMenuBar)
	wxEND_EVENT_TABLE()
#endif
