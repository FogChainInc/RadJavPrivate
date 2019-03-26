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
					panel = RJNEW wxPanel(this, wxID_ANY);
					/*wxBoxSizer *sizer = RJNEW wxBoxSizer(wxVERTICAL);
					panel->SetSizer(sizer);
					sizer->SetSizeHints(object);*/
				}

				void WindowFrame::addChild(GObjectWidget *child)
				{
					if (child)
					{
						panel->AddChild(child->getNativeWidget());
						child->parent = this;
					}
				}

				void WindowFrame::setFont(CPP::Font *font)
				{
					wxFont wxfont;

					if (font->fontFamily != "")
					{
						if (!wxfont.SetFaceName(font->fontFamily.towxString()))
							return;
					}
					else
					{
						wxfont = *wxSMALL_FONT;
					}

					wxfont.SetPixelSize(wxSize(0, font->size));

					if (font->underline == true)
						wxfont.MakeUnderlined();

					if (font->bold == true)
						wxfont.MakeBold();

					if (font->italic == true)
						wxfont.MakeItalic();

					wxWindow* widget = this;
					if (widget)
					{
						widget->SetFont(wxfont);
						widget->SetForegroundColour(wxColor(font->color.r * 255,
							font->color.g * 255, font->color.b * 255, font->color.a * 255));
					}
				}

				CPP::Font* WindowFrame::getFont()
				{
					//TODO: convert native Font to CPP::Font here
					return nullptr;
				}

				void WindowFrame::setPosition(RJINT x, RJINT y)
				{
					this->SetPosition(wxPoint(x, y));
				}

				void WindowFrame::setPosition(CPP::Vector2 pos)
				{
					setPosition(pos.x, pos.y);
				}

				CPP::Vector2 WindowFrame::getPosition()
				{
					wxPoint pos = this->GetPosition();
					return Vector2(pos.x, pos.y);
				}

				RJINT WindowFrame::getX()
				{
					return this->GetPosition().x;
				}

				RJINT WindowFrame::getY()
				{
					return this->GetPosition().y;
				}

				void WindowFrame::setSize(RJINT width, RJINT height)
				{
					this->SetSize(width, height);
				}

				void WindowFrame::setSize(CPP::Vector2 size)
				{
					setSize(size.x, size.y);
				}

				CPP::Vector2 WindowFrame::getSize()
				{
					wxSize size = this->GetSize();
					return Vector2(size.GetWidth(), size.GetHeight());
				}

				RJINT WindowFrame::getWidth()
				{
					return this->GetSize().GetWidth();
				}

				RJINT WindowFrame::getHeight()
				{
					return this->GetSize().GetHeight();
				}

				void WindowFrame::setText(String text)
				{
					SetTitle(text.towxString());
				}

				String WindowFrame::getText()
				{
					return parsewxString(GetTitle());
				}

				void WindowFrame::setVisibility(RJBOOL visible)
				{
					if (visible)
						this->Show();
					else
						this->Hide();
				}

				RJBOOL WindowFrame::getVisibility()
				{
					return this->IsShown();
				}

				void WindowFrame::setEnabled(RJBOOL enabled)
				{
					if (enabled)
						this->Enable();
					else
						this->Disable();
				}

				RJBOOL WindowFrame::getEnabled()
				{
					return getNativeWidget()->IsEnabled();
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
			
				wxWindow* WindowFrame::getNativeWidget()
				{
					return panel;
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
