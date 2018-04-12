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
				MenuBarFrame::MenuBarFrame(const wxString &text, const wxPoint &pos, const wxSize &size)
				  : wxFrame(NULL, wxID_ANY, text, pos, size), GObjectBase ()
				{
				}

				void MenuBarFrame::onMenuOpen(wxMenuEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void MenuBarFrame::onMenuClose(wxMenuEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void MenuBarFrame::onMenuHighLight(wxMenuEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void MenuBarFrame::onMenuSelected(wxCommandEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

			#endif

			#ifdef USE_V8
				MenuBar::MenuBar(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
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
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					wxMenuBar *menuBar = RJNEW wxMenuBar();
					CPP::GUI::MenuBarFrame *window = (CPP::GUI::MenuBarFrame *)parentWin->GetParent();

					window->SetMenuBar(menuBar);

					_appObj = menuBar;

					setup();
				#endif
			}

			#ifdef USE_V8
				void MenuBar::on(String event, v8::Local<v8::Function> func)
				{
					CPP::GUI::MenuBarFrame *object = (CPP::GUI::MenuBarFrame *)_appObj;
					CPP::GUI::MenuBarFrame *obj = (CPP::GUI::MenuBarFrame *)object->GetParent();
					
					obj->addNewEvent(event, object, func);

					if (event == "menuopen")
					{
						v8::Persistent<v8::Value> *pevent = obj->createEvent(event, func);

						#ifdef GUI_USE_WXWIDGETS
						obj->Connect(wxEVT_MENU_OPEN, wxMenuEventHandler(MenuBarFrame::onMenuOpen), (wxObject *)pevent);
						#endif
						
					}
					if (event == "menuclose")
					{
						v8::Persistent<v8::Value> *pevent = obj->createEvent(event, func);

						#ifdef GUI_USE_WXWIDGETS
						obj->Connect(wxEVT_MENU_CLOSE, wxMenuEventHandler(MenuBarFrame::onMenuClose), (wxObject *)pevent);
						#endif
					}
					if (event == "menuhighlight")
					{
						v8::Persistent<v8::Value> *pevent = obj->createEvent(event, func);

						#ifdef GUI_USE_WXWIDGETS
						obj->Connect(wxEVT_MENU_HIGHLIGHT, wxMenuEventHandler(MenuBarFrame::onMenuHighLight), (wxObject *)pevent);
						#endif
					}
					if (event == "menuselected")
					{
						v8::Persistent<v8::Value> *pevent = obj->createEvent(event, func);

						#ifdef GUI_USE_WXWIDGETS
						obj->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(MenuBarFrame::onMenuSelected), (wxObject *)pevent);
						#endif
					}


				  
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::MenuBarFrame, wxFrame)
	wxEND_EVENT_TABLE()
#endif
