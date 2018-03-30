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
#include "cpp/RadJavCPPGUIGObject.h"
#include "cpp/RadJavCPPFont.h"
#include "cpp/RadJavCPPColor.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef USE_V8
				GObject::GObject(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				{
					_zIndex = 0;
					_cursor = "default";
					_parent = NULL;
					_appObj = NULL;

					type = jsEngine->v8GetString(args.This(), "type");
					name = jsEngine->v8GetString(args.This(), "name");
					_text = jsEngine->v8GetString(args.This(), "_text");
					_visible = jsEngine->v8GetBool(args.This(), "_visible");
					_cursor = jsEngine->v8GetString(args.This(), "_cursor");

					v8::Handle<v8::Object> parent = jsEngine->v8GetObject(args.This(), "_parent");

					if (jsEngine->v8IsNull(parent) == false)
						_parent = (GObject *)jsEngine->v8GetExternal(parent, "_appObj");

					v8::Local<v8::Object> transform = jsEngine->v8GetObject(args.This(), "_transform");
					_transform = RJNEW Rectangle(jsEngine, transform);

					v8::Handle<v8::Object> objfont = jsEngine->v8GetObject(args.This(), "_font");
					_font = RJNEW Font(jsEngine, objfont);
				}
			#endif

			GObject::GObject(String name, String text, GObject *parent)
			{
				this->name = name;
				type = "";
				_transform = RJNEW Rectangle();
				_visible = true;
				_zIndex = 0;
				_text = text;
				_font = RJNEW Font();
				_cursor = "default";
				_parent = parent;
				_appObj = NULL;
			}

			GObject::~GObject()
			{
				DELETEOBJ(_font);
				DELETEOBJ(_transform);

				DELETEOBJ(_parent);
				DELETEOBJ(_appObj);
			}

			void GObject::create()
			{
			}

			void GObject::setFont(CPP::Font *font)
			{
				_font = font;

				#ifdef GUI_USE_WXWIDGETS
					wxFont wxfont;

					if (font->fontFamily != "")
						wxfont.SetFaceName(font->fontFamily.towxString());
					else
						wxfont = *wxSMALL_FONT;

					wxfont.SetPixelSize(wxSize(0, font->size));

					if (font->underline == true)
						wxfont.MakeUnderlined();

					if (font->bold == true)
						wxfont.MakeBold();

					if (font->italic == true)
						wxfont.MakeItalic();

					_appObj->SetFont(wxfont);
					_appObj->SetForegroundColour(wxColor(font->color.r * 255, 
						font->color.g * 255, font->color.b * 255, font->color.a * 255));
				#endif
			}

			CPP::Font *GObject::getFont()
			{
				return (_font);
			}

			void GObject::setPosition(RJINT x, RJINT y)
			{
				_transform->setPosition(x, y);

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						_appObj->SetPosition(wxPoint(x, y));
				#endif
			}

			void GObject::setPosition(CPP::Vector2 pos)
			{
				this->setPosition(pos.x, pos.y);
			}

			CPP::Vector2 GObject::getPosition()
			{
				CPP::Vector2 pos = _transform->getPosition();

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxPoint wxpos = _appObj->GetPosition();
						pos.x = wxpos.x;
						pos.y = wxpos.y;
					}
				#endif

				return (pos);
			}

			RJINT GObject::getX()
			{
				RJINT x = getPosition().x;

				return (x);
			}

			RJINT GObject::getY()
			{
				RJINT y = getPosition().y;

				return (y);
			}

			void GObject::setSize(RJINT width, RJINT height)
			{
				_transform->setSize(width, height);

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						_appObj->SetSize(width, height);
				#endif
			}

			void GObject::setSize(CPP::Vector2 size)
			{
				this->setSize(size.x, size.y);
			}

			CPP::Vector2 GObject::getSize()
			{
				CPP::Vector2 size = _transform->getSize();

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxSize wxsize = _appObj->GetSize();
						size.x = wxsize.GetWidth ();
						size.y = wxsize.GetHeight ();
					}
				#endif

				return (size);
			}

			RJINT GObject::getWidth()
			{
				RJINT width = getSize().x;

				return (width);
			}

			RJINT GObject::getHeight()
			{
				RJINT height = getSize().y;

				return (height);
			}

			void GObject::setText(String text)
			{
				_text = text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						_appObj->SetLabel(text.towxString());
				#endif
			}

			String GObject::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxString wxtext = _appObj->GetLabel();
						text = parsewxString (wxtext);
					}
				#endif

				return (text);
			}

			GObject *GObject::getParent()
			{
				return (_parent);
			}

			void GObject::setVisibility(RJBOOL visible)
			{
				_visible = visible;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						if (visible == true)
							_appObj->Show();
						else
							_appObj->Hide();
					}
				#endif
			}

			RJBOOL GObject::getVisibility()
			{
				RJBOOL visible = _visible;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						visible = _appObj->IsShown();
				#endif

				return (visible);
			}

			void GObject::setEnabled(RJBOOL enabled)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						if (enabled == true)
							_appObj->Enable();
						else
							_appObj->Disable();
					}
				#endif
			}

			RJBOOL GObject::getEnabled()
			{
				RJBOOL enabled = false;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						enabled = _appObj->IsEnabled();
				#endif

				return (enabled);
			}

			void GObject::setup()
			{
				setText(_text);
				setupCursor();
				setFont(_font);
			}

			void GObject::setupCursor()
			{
				if (_cursor == "hand")
					_appObj->SetCursor(wxCursor(wxCURSOR_HAND));
			}

			GObjectBase::GObjectBase()
			{
				events = RJNEW HashMap<std::string, v8::Persistent<v8::Value> *>();
			}

			GObjectBase::~GObjectBase()
			{
				HashMap<std::string, v8::Persistent<v8::Value> *>::iterator found = events->begin ();
				HashMap<std::string, v8::Persistent<v8::Value> *>::iterator end = events->end();

				while (found != end)
				{
					v8::Persistent<v8::Value> *evt = found->second;
					DELETE_OBJ(evt);

					found++;
				}

				DELETEOBJ(events);
			}

			v8::Persistent<v8::Value> *GObjectBase::createEvent(String event, v8::Local<v8::Function> function)
			{
				v8::Persistent<v8::Value> *persistent = RJNEW v8::Persistent<v8::Value>();
				persistent->Reset(V8_JAVASCRIPT_ENGINE->isolate, function);

				if (events->size() > 0)
				{
					auto found = events->find(event);
					auto end = events->end();

					if (found != end)
					{
						v8::Persistent<v8::Value> *evt = events->at(event);
						DELETE_OBJ(evt);

						events->erase(event);
					}
				}

				events->insert(HashMapPair<std::string, v8::Persistent<v8::Value> *>(event, persistent));

				return (persistent);
			}

			#ifdef GUI_USE_WXWIDGETS
				void GObjectBase::addNewEvent(String event, wxWindow *object, v8::Local<v8::Function> func)
				{
					if (event == "click")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_LEFT_UP, GObjectBase::onMouseEvent, -1, -1, (wxObject *)pevent);
					}

					if (event == "keyup")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_KEY_UP, GObjectBase::onKeyUp, -1, -1, (wxObject *)pevent);
					}
					
					if (event == "keydown")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_KEY_DOWN, GObjectBase::onKeyDown, -1, -1, (wxObject *)pevent);
					}

					if (event == "dblclick" || event == "doubleclick" )
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_LEFT_DCLICK, GObjectBase::onDoubleClick, -1, -1, (wxObject *)pevent);
					}

					if (event == "rightclick" || event == "contextmenu")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_RIGHT_UP, GObjectBase::onRightClick, -1, -1, (wxObject *)pevent);
					}
					
					if (event == "mousedown")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_AUX1_DOWN, GObjectBase::onMouseAux1Down, -1, -1, (wxObject *)pevent);
					}

					if (event == "mouseup")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_AUX1_UP, GObjectBase::onMouseAux1Up, -1, -1, (wxObject *)pevent);
					}

					if (event == "mouseenter")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_ENTER_WINDOW, GObjectBase::onMouseEnterWindow, -1, -1, (wxObject *)pevent);
					}

					if (event == "mouseleave")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_LEAVE_WINDOW, GObjectBase::onMouseLeaveWindow, -1, -1, (wxObject *)pevent);
					}

					if (event == "mousemove")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_MOTION, GObjectBase::onMouseMotion, -1, -1, (wxObject *)pevent);
					}

					if (event == "mousewheel")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_MOUSEWHEEL, GObjectBase::onMouseWheel, -1, -1, (wxObject *)pevent);
					}

					if (event == "rightdblclick" || event == "rightdoubleclick")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_RIGHT_DCLICK, GObjectBase::onRightDoubleClick, -1, -1, (wxObject *)pevent);
					}

					if (event == "rightdown")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_RIGHT_DOWN, GObjectBase::onRightDown, -1, -1, (wxObject *)pevent);
					}

					if (event == "middleclick")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_MIDDLE_UP, GObjectBase::onMiddleClick, -1, -1, (wxObject *)pevent);
					}

					if (event == "middledblclick" || event == "middledoubleclick")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_MIDDLE_UP, GObjectBase::onMiddleDoubleClick, -1, -1, (wxObject *)pevent);
					}

					if (event == "middledown")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_MIDDLE_DOWN, GObjectBase::onMiddleDown, -1, -1, (wxObject *)pevent);
					}

					if (event == "focus")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_SET_FOCUS, GObjectBase::onFocusSet, -1, -1, (wxObject *)pevent);
					}

					if (event == "focusout")
					{
						v8::Persistent<v8::Value> *pevent = createEvent(event, func);
						object->Bind(wxEVT_KILL_FOCUS, GObjectBase::onFocusOut, -1, -1, (wxObject *)pevent);
					}
				}

				void GObjectBase::onClick(wxCommandEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMouseEvent(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onKeyUp(wxKeyEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}
				
				void GObjectBase::onKeyDown(wxKeyEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onDoubleClick(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onRightClick(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMouseAux1Down(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMouseAux1Up(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMouseEnterWindow(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMouseLeaveWindow(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMouseMotion(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMouseWheel(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onRightDoubleClick(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onRightDown(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMiddleClick(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onMiddleDoubleClick(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}
				
				void GObjectBase::onMiddleDown(wxMouseEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onFocusSet(wxFocusEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void GObjectBase::onFocusOut(wxFocusEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}
			#endif

			v8::Local<v8::Value> GObjectBase::executeEvent(v8::Persistent<v8::Value> *pevent, RJINT numArgs, v8::Local<v8::Value> *args)
			{
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(pevent->Get(V8_JAVASCRIPT_ENGINE->isolate));
				v8::Local<v8::Value> result;

				if (V8_JAVASCRIPT_ENGINE->v8IsNull(func) == false)
					result = func->Call(V8_JAVASCRIPT_ENGINE->globalContext->Global(), numArgs, args);

				return (result);
			}
		}
	}
}

