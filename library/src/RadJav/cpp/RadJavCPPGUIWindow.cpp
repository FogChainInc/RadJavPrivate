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
#include "cpp/RadJavCPPGUIWindow.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				WindowFrame::WindowFrame(const wxString &text, const wxPoint &pos, const wxSize &size)
					: wxFrame(NULL, wxID_ANY, text, pos, size), GObjectBase ()
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
                    /// @todo Fill this out.
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

			#ifdef USE_V8
			Window::Window(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args)
			{
				icon = jsEngine->v8GetString(args.This(), "_icon");
			}
			#endif

			Window::Window(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void Window::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					WindowFrame *object = RJNEW WindowFrame(_text, 
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
				
					linkWith(object);
				
					wxPanel *panel = RJNEW wxPanel(object, wxID_ANY);
					/*wxBoxSizer *sizer = RJNEW wxBoxSizer(wxVERTICAL);
					panel->SetSizer(sizer);
					sizer->SetSizeHints(object);*/
					RadJav::app->SetTopWindow(object);
					object->Show(_visible);
					RadJav::app->SetActive(_visible, object);

					_appObj = panel;

					if (icon != "")
						setIcon(icon);

					setup();
				#endif
			}

			void Window::setPosition(RJINT x, RJINT y)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						CPP::GUI::WindowFrame *obj = (CPP::GUI::WindowFrame *)_appObj->GetParent();
						obj->SetPosition(wxPoint(x, y));
					}
				#endif
			}

			CPP::Vector2 Window::getPosition()
			{
				CPP::Vector2 newpos;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxPoint pos = _appObj->GetParent ()->GetPosition();
						newpos.x = pos.x;
						newpos.y = pos.y;
					}
				#endif

				return (newpos);
			}

			void Window::setSize(RJINT width, RJINT height)
			{
				_transform->setSize(width, height);

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						_appObj->GetParent ()->SetSize(width, height);
				#endif
			}

			CPP::Vector2 Window::getSize()
			{
				CPP::Vector2 size = _transform->getSize();

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxSize wxsize = _appObj->GetParent ()->GetSize();
						size.x = wxsize.GetWidth();
						size.y = wxsize.GetHeight();
					}
				#endif

				return (size);
			}

			void Window::setText(String text)
			{
				_text = text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						((WindowFrame *)_appObj->GetParent())->SetTitle(text.towxString());
				#endif
			}

			String Window::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxString wxtext = ((WindowFrame *)_appObj->GetParent ())->GetTitle ();
						text = parsewxString(wxtext);
					}
				#endif

				return (text);
			}

			void Window::setVisibility(RJBOOL visible)
			{
				_visible = visible;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						if (visible == true)
							_appObj->GetParent()->Show();
						else
							_appObj->GetParent()->Hide();
					}
				#endif
			}

			RJBOOL Window::getVisibility()
			{
				RJBOOL visible = _visible;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						visible = _appObj->GetParent ()->IsShown();
				#endif

				return (visible);
			}

			void Window::setEnabled(RJBOOL enabled)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						if (enabled == true)
							((WindowFrame *)_appObj->GetParent())->Enable();
						else
							((WindowFrame *)_appObj->GetParent())->Disable();
					}
				#endif
			}

			RJBOOL Window::getEnabled()
			{
				RJBOOL enabled = false;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						enabled = ((WindowFrame *)_appObj->GetParent())->IsEnabled();
				#endif

				return (enabled);
			}

			void Window::setIcon(String newIcon)
			{
				icon = newIcon;

				#ifdef GUI_USE_WXWIDGETS
                    #ifdef __WINDOWS__
                        wxIcon wxicon = wxICON(newIcon.c_str ());
                
                        if (wxicon.IsOk() == false)
                            throw RadJAV::Exception("Invalid icon");
                
                        ((WindowFrame *)_appObj->GetParent())->SetIcon(wxicon);
                    #else
                        #warning "TODO: Add Window Icon support for non Windows platforms"
                    #endif
				#endif
			}

			#ifdef USE_V8
			void Window::on(String event, v8::Local<v8::Function> func)
			{
				#ifdef GUI_USE_WXWIDGETS
					CPP::GUI::WindowFrame *object = (CPP::GUI::WindowFrame *)_appObj;
					CPP::GUI::WindowFrame *obj = (CPP::GUI::WindowFrame *)object->GetParent();

					obj->addNewEvent(event, obj, func);

					if (event == "close")
					{
						obj->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(WindowFrame::onJSClose), obj->createEvent(event, func));
					}

					if (event == "minimize")
					{
						obj->Connect(wxEVT_ICONIZE, wxIconizeEventHandler(WindowFrame::onJSMinimized), obj->createEvent(event, func));
					}
					if (event == "maximize")
					{
						obj->Connect(wxEVT_MAXIMIZE, wxMaximizeEventHandler(WindowFrame::onJSMaximized), obj->createEvent(event, func));
					}
				
					if (event == "menuselected")
					{
						obj->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(WindowFrame::onMenuSelected), obj->createEvent(event, func));
					}
				#endif
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

