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
					{
						_parent = (GObject *)jsEngine->v8GetExternal(parent, "_appObj");
						_parent->addChild(this);
					}

					v8::Local<v8::Object> transform = jsEngine->v8GetObject(args.This(), "_transform");
					_transform = RJNEW Rectangle(jsEngine, transform);

					v8::Handle<v8::Object> objfont = jsEngine->v8GetObject(args.This(), "_font");
					_font = RJNEW Font(jsEngine, objfont);
				}
			#elif defined USE_JAVASCRIPTCORE
                GObject::GObject(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
                {
                    _zIndex = 0;
                    _cursor = "default";
                    _parent = NULL;
					_appObj = NULL;

                    type = jsEngine->jscGetString(thisObj, "type");
                    name = jsEngine->jscGetString(thisObj, "name");
                    _text = jsEngine->jscGetString(thisObj, "_text");
                    _visible = jsEngine->jscGetBool(thisObj, "_visible");
                    _cursor = jsEngine->jscGetString(thisObj, "_cursor");

                    JSObjectRef parent = jsEngine->jscGetObject(thisObj, "_parent");

					if (jsEngine->jscIsNull(parent) == false)
					{
						_parent = (GObject *)jsEngine->jscGetExternal(jsEngine->globalContext, parent, "_appObj");
						_parent->addChild(this);
					}

                    JSObjectRef transform = jsEngine->jscGetObject(thisObj, "_transform");
                    _transform = RJNEW Rectangle(jsEngine, transform);

                    JSObjectRef objfont = jsEngine->jscGetObject(thisObj, "_font");
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

				if (_parent != NULL)
					_parent->addChild(this);
			}

			GObject::~GObject()
			{
				DELETEOBJ(_font);
				DELETEOBJ(_transform);
			}

			void GObject::create()
			{
			}

			void GObject::addChild(GObject *child)
			{
				if (_appObj &&
					child &&
					child->_appObj)
				{
					_appObj->addChild(child->_appObj);
				}

				child->_parent = this;
				_children.push_back (child);
			}

			void GObject::setFont(CPP::Font *font)
			{
				_font = font;

				if (_appObj)
					_appObj->setFont(font);
			}

			CPP::Font *GObject::getFont()
			{
				return _font;
			}

			void GObject::setPosition(RJINT x, RJINT y)
			{
				_transform->setPosition(x, y);

				if (_appObj)
				{
					_appObj->setPosition(x, y);
				}
			}

			void GObject::setPosition(CPP::Vector2 pos)
			{
				setPosition(pos.x, pos.y);
			}

			CPP::Vector2 GObject::getPosition()
			{
				CPP::Vector2 pos = _transform->getPosition();

				if (_appObj)
				{
					pos = _appObj->getPosition();
				}

				return pos;
			}

			RJINT GObject::getX()
			{
				RJINT x = getPosition().x;

				return x;
			}

			RJINT GObject::getY()
			{
				RJINT y = getPosition().y;

				return y;
			}

			void GObject::setSize(RJINT width, RJINT height)
			{
				_transform->setSize(width, height);

				if (_appObj)
				{
					_appObj->setSize(width, height);
				}
			}

			void GObject::setSize(CPP::Vector2 size)
			{
				setSize(size.x, size.y);
			}

			CPP::Vector2 GObject::getSize()
			{
				CPP::Vector2 size = _transform->getSize();

				if (_appObj)
				{
					size = _appObj->getSize();
				}

				return size;
			}

			RJINT GObject::getWidth()
			{
				RJINT width = getSize().x;

				return width;
			}

			RJINT GObject::getHeight()
			{
				RJINT height = getSize().y;

				return height;
			}

			void GObject::setText(String text)
			{
				_text = text;

				if (_appObj)
				{
					_appObj->setText(text);
				}
			}

			String GObject::getText()
			{
				String text = _text;

				if (_appObj)
				{
					text = _appObj->getText();
				}

				return text;
			}

			GObject *GObject::getParent()
			{
				return _parent;
			}

			void GObject::setVisibility(RJBOOL visible)
			{
				_visible = visible;

				if (_appObj)
				{
					_appObj->setVisibility(visible);
				}
			}

			RJBOOL GObject::getVisibility()
			{
				RJBOOL visible = _visible;

				if (_appObj)
				{
					visible = _appObj->getVisibility();
				}

				return visible;
			}

			void GObject::setEnabled(RJBOOL enabled)
			{
				if (_appObj)
				{
					_appObj->setEnabled(enabled);
				}
			}

			RJBOOL GObject::getEnabled()
			{
				RJBOOL enabled = false;

				if (_appObj)
				{
					enabled = _appObj->getEnabled();
				}

				return enabled;
			}

			void GObject::_callChildCreated(GObject *child)
			{
				if (onChildCreated != NULL)
					onChildCreated(child);
			}

			void GObject::setup()
			{
				setText(_text);
				setupCursor();
				setFont(_font);

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						_appObj->Raise();
				#endif
			}

			void GObject::setupCursor()
			{
				if (_appObj)
				{
					#ifdef GUI_USE_WXWIDGETS
						if (_cursor == "hand")
							_appObj->SetCursor(wxCursor(wxCURSOR_HAND));
					#elif defined USE_IOS || defined USE_ANDROID
						#warning No cursor support in MUI
					#endif
				}
			}

			GObjectEvents::GObjectEvents()
			{
				events = RJNEW HashMap<std::string, Event* >();
			}

			GObjectEvents::~GObjectEvents()
			{
				DELETEOBJ(events);
			}

			#ifdef GUI_USE_WXWIDGETS
                #ifdef USE_V8
					Event* GObjectEvents::createEvent(String event, v8::Local<v8::Function> function)
					{
						// Create a persistent function to execute asych later.
						v8::Persistent<v8::Value> *persistent = RJNEW v8::Persistent<v8::Value>();
						persistent->Reset(function->GetIsolate(), function);
						
						Event* evt = RJNEW Event(persistent);
						
						if (events->size() > 0)
						{
							auto found = events->find(event);
							auto end = events->end();
							
							if (found != end)
							{
								Event *evtToRemove = events->at(event);
								DELETEOBJ(evtToRemove);
								
								events->erase(event);
							}
						}
						
						events->insert(HashMapPair<std::string, Event *>(event, evt));
						
						return evt;
					}
                #elif defined USE_JAVASCRIPTCORE
                    Event* GObjectEvents::createEvent(String event, JSObjectRef function)
                    {
                        // Create a persistent function to execute asych later.
                        Event* evt = RJNEW Event(function);
                        
                        if (events->size() > 0)
                        {
                            auto found = events->find(event);
                            auto end = events->end();
                            
                            if (found != end)
                            {
                                Event *evtToRemove = events->at(event);
                                DELETEOBJ(evtToRemove);
                                
                                events->erase(event);
                            }
                        }
                        
                        events->insert(HashMapPair<std::string, Event *>(event, evt));
                        
                        return evt;
                    }
                #endif
			
				void GObjectEvents::addNewEvent(String event, wxWindow *object,
                                            #ifdef USE_V8
                                              v8::Local<v8::Function> func
                                            #endif
                                            #ifdef USE_JAVASCRIPTCORE
                                              JSObjectRef func
                                            #endif
                                              )
				{
					if (event == "click")
					{
						object->Bind(wxEVT_LEFT_UP, GObjectEvents::onClick, -1, -1, createEvent(event, func));
					}

					if (event == "keyup")
					{
						object->Bind(wxEVT_KEY_UP, GObjectEvents::onKeyUp, -1, -1, createEvent(event, func));
					}
					
					if (event == "keydown")
					{
						object->Bind(wxEVT_KEY_DOWN, GObjectEvents::onKeyDown, -1, -1, createEvent(event, func));
					}

					if (event == "dblclick" || event == "doubleclick" )
					{
						object->Bind(wxEVT_LEFT_DCLICK, GObjectEvents::onDoubleClick, -1, -1, createEvent(event, func));
					}

					if (event == "rightclick" || event == "contextmenu")
					{
						object->Bind(wxEVT_RIGHT_UP, GObjectEvents::onRightClick, -1, -1, createEvent(event, func));
					}
					
					if (event == "mousedown")
					{
						object->Bind(wxEVT_AUX1_DOWN, GObjectEvents::onMouseAux1Down, -1, -1, createEvent(event, func));
					}

					if (event == "mouseup")
					{
						object->Bind(wxEVT_AUX1_UP, GObjectEvents::onMouseAux1Up, -1, -1, createEvent(event, func));
					}

					if (event == "mouseenter")
					{
						object->Bind(wxEVT_ENTER_WINDOW, GObjectEvents::onMouseEnterWindow, -1, -1, createEvent(event, func));
					}

					if (event == "mouseleave")
					{
						object->Bind(wxEVT_LEAVE_WINDOW, GObjectEvents::onMouseLeaveWindow, -1, -1, createEvent(event, func));
					}

					if (event == "mousemove")
					{
						object->Bind(wxEVT_MOTION, GObjectEvents::onMouseMotion, -1, -1, createEvent(event, func));
					}

					if (event == "mousewheel")
					{
						object->Bind(wxEVT_MOUSEWHEEL, GObjectEvents::onMouseWheel, -1, -1, createEvent(event, func));
					}

					if (event == "rightdblclick" || event == "rightdoubleclick")
					{
						object->Bind(wxEVT_RIGHT_DCLICK, GObjectEvents::onRightDoubleClick, -1, -1, createEvent(event, func));
					}

					if (event == "rightdown")
					{
						object->Bind(wxEVT_RIGHT_DOWN, GObjectEvents::onRightDown, -1, -1, createEvent(event, func));
					}

					if (event == "middleclick")
					{
						object->Bind(wxEVT_MIDDLE_UP, GObjectEvents::onMiddleClick, -1, -1, createEvent(event, func));
					}

					if (event == "middledblclick" || event == "middledoubleclick")
					{
						object->Bind(wxEVT_MIDDLE_UP, GObjectEvents::onMiddleDoubleClick, -1, -1, createEvent(event, func));
					}

					if (event == "middledown")
					{
						object->Bind(wxEVT_MIDDLE_DOWN, GObjectEvents::onMiddleDown, -1, -1, createEvent(event, func));
					}

					if (event == "focus")
					{
						object->Bind(wxEVT_SET_FOCUS, GObjectEvents::onFocusSet, -1, -1, createEvent(event, func));
					}

					if (event == "focusout")
					{
						object->Bind(wxEVT_KILL_FOCUS, GObjectEvents::onFocusOut, -1, -1, createEvent(event, func));
					}
				}

				void GObjectEvents::onClick(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);	/// @fixme Is this needed? Seems to be necessary on Windows.
				}

				void GObjectEvents::onKeyUp(wxKeyEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}
				
				void GObjectEvents::onKeyDown(wxKeyEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onDoubleClick(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onRightClick(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onMouseAux1Down(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onMouseAux1Up(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onMouseEnterWindow(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onMouseLeaveWindow(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onMouseMotion(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onMouseWheel(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onRightDoubleClick(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onRightDown(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onMiddleClick(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onMiddleDoubleClick(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}
				
				void GObjectEvents::onMiddleDown(wxMouseEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onFocusSet(wxFocusEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}

				void GObjectEvents::onFocusOut(wxFocusEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);

					event.Skip(true);
				}
			#elif defined USE_IOS || defined USE_ANDROID
				#ifdef USE_V8
					Event* GObjectEvents::createEvent(String event, v8::Local<v8::Function> function)
					{
						// Create a persistent function to execute async later.
						v8::Persistent<v8::Value> *persistent = RJNEW v8::Persistent<v8::Value>();
						persistent->Reset(function->GetIsolate(), function);
						
						Event* evt = RJNEW Event(persistent);
						
						if (events->size() > 0)
						{
							auto found = events->find(event);
							auto end = events->end();
							
							if (found != end)
							{
								Event *evtToRemove = events->at(event);
								DELETEOBJ(evtToRemove);
								
								events->erase(event);
							}
						}
						
						events->insert(HashMapPair<std::string, Event *>(event, evt));
						
						return evt;
					}
				#elif defined USE_JAVASCRIPTCORE
					Event* GObjectEvents::createEvent(String event, JSObjectRef function)
					{
						// Create a persistent function to execute asych later.
						Event* evt = RJNEW Event(function);
						
						if (events->size() > 0)
						{
							auto found = events->find(event);
							auto end = events->end();
							
							if (found != end)
							{
								Event *evtToRemove = events->at(event);
								DELETEOBJ(evtToRemove);
								
								events->erase(event);
							}
						}
						
						events->insert(HashMapPair<std::string, Event *>(event, evt));
						
						return evt;
					}
				#endif
			
				void GObjectEvents::addNewEvent(String event,
												#ifdef USE_V8
													v8::Local<v8::Function> func
												#elif defined USE_JAVASCRIPTCORE
													JSObjectRef func
												#endif
												)
				{
					Event* evt = createEvent(event, func);
					
					bindEvent(event, evt);
					
					/* TODO: Add relevant implementation
					if (event == "click")
					{
						object->Bind(wxEVT_LEFT_UP, GObjectEvents::onClick, -1, -1, createEvent(event, func));
					}
					
					if (event == "keyup")
					{
						object->Bind(wxEVT_KEY_UP, GObjectEvents::onKeyUp, -1, -1, createEvent(event, func));
					}
					
					if (event == "keydown")
					{
						object->Bind(wxEVT_KEY_DOWN, GObjectEvents::onKeyDown, -1, -1, createEvent(event, func));
					}
					
					if (event == "dblclick" || event == "doubleclick" )
					{
						object->Bind(wxEVT_LEFT_DCLICK, GObjectEvents::onDoubleClick, -1, -1, createEvent(event, func));
					}
					
					if (event == "rightclick" || event == "contextmenu")
					{
						object->Bind(wxEVT_RIGHT_UP, GObjectEvents::onRightClick, -1, -1, createEvent(event, func));
					}
					
					if (event == "mousedown")
					{
						object->Bind(wxEVT_AUX1_DOWN, GObjectEvents::onMouseAux1Down, -1, -1, createEvent(event, func));
					}
					
					if (event == "mouseup")
					{
						object->Bind(wxEVT_AUX1_UP, GObjectEvents::onMouseAux1Up, -1, -1, createEvent(event, func));
					}
					
					if (event == "mouseenter")
					{
						object->Bind(wxEVT_ENTER_WINDOW, GObjectEvents::onMouseEnterWindow, -1, -1, createEvent(event, func));
					}
					
					if (event == "mouseleave")
					{
						object->Bind(wxEVT_LEAVE_WINDOW, GObjectEvents::onMouseLeaveWindow, -1, -1, createEvent(event, func));
					}
					
					if (event == "mousemove")
					{
						object->Bind(wxEVT_MOTION, GObjectEvents::onMouseMotion, -1, -1, createEvent(event, func));
					}
					
					if (event == "mousewheel")
					{
						object->Bind(wxEVT_MOUSEWHEEL, GObjectEvents::onMouseWheel, -1, -1, createEvent(event, func));
					}
					
					if (event == "rightdblclick" || event == "rightdoubleclick")
					{
						object->Bind(wxEVT_RIGHT_DCLICK, GObjectEvents::onRightDoubleClick, -1, -1, createEvent(event, func));
					}
					
					if (event == "rightdown")
					{
						object->Bind(wxEVT_RIGHT_DOWN, GObjectEvents::onRightDown, -1, -1, createEvent(event, func));
					}
					
					if (event == "middleclick")
					{
						object->Bind(wxEVT_MIDDLE_UP, GObjectEvents::onMiddleClick, -1, -1, createEvent(event, func));
					}
					
					if (event == "middledblclick" || event == "middledoubleclick")
					{
						object->Bind(wxEVT_MIDDLE_UP, GObjectEvents::onMiddleDoubleClick, -1, -1, createEvent(event, func));
					}
					
					if (event == "middledown")
					{
						object->Bind(wxEVT_MIDDLE_DOWN, GObjectEvents::onMiddleDown, -1, -1, createEvent(event, func));
					}
					
					if (event == "focus")
					{
						object->Bind(wxEVT_SET_FOCUS, GObjectEvents::onFocusSet, -1, -1, createEvent(event, func));
					}
					
					if (event == "focusout")
					{
						object->Bind(wxEVT_KILL_FOCUS, GObjectEvents::onFocusOut, -1, -1, createEvent(event, func));
					}
					 */
				}
			#endif
			
			#ifdef USE_V8
				v8::Local<v8::Value> GObjectEvents::executeEvent(Event *pevent, RJINT numArgs, v8::Local<v8::Value> *args)
				{
					return (*pevent)(numArgs, args);
				}

				v8::Local<v8::Value> GObjectEvents::executeEvent(const String& event, RJINT numArgs, v8::Local<v8::Value> *args)
				{
					if (events)
					{
						auto evt = events->find(event);
						if (evt != events->end())
						{
							return executeEvent(evt->second, numArgs, args);
						}
					}
					
					return v8::Undefined (V8_JAVASCRIPT_ENGINE->isolate);
				}
			
			#elif defined USE_JAVASCRIPTCORE
				JSValueRef GObjectEvents::executeEvent(Event *pevent, RJINT numArgs, JSValueRef *args)
				{
					return (*pevent)(numArgs, args);
				}

				JSValueRef GObjectEvents::executeEvent(const String& event, RJINT numArgs, JSValueRef *args)
				{
					if (events)
					{
						auto evt = events->find(event);
						if (evt != events->end())
						{
							return executeEvent(evt->second, numArgs, args);
						}
					}
					
					return JSValueMakeUndefined(JSC_JAVASCRIPT_ENGINE->globalContext);
				}
			#endif
		}
	}
}
