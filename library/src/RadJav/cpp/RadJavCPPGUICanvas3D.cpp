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
#include "cpp/RadJavCPPGUICanvas3D.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				Canvas3DFrame::Canvas3DFrame(const wxString &text, const wxPoint &pos, const wxSize &size, Array<CanvasResource *> resources)
					: wxFrame(NULL, wxID_ANY, text, pos, size), GObjectBase()
				{
					mRenderWindow = NULL;
					mSceneMgr = NULL;
					mRoot = NULL;

					V8_JAVASCRIPT_ENGINE->start3DEngine();
					mRoot = V8_JAVASCRIPT_ENGINE->mRoot;

					for (RJUINT iIdx = 0; iIdx < resources.size(); iIdx++)
					{
						CanvasResource *resource = resources.at(iIdx);
						Ogre::ResourceGroupManager::getSingleton().addResourceLocation(resource->name, resource->type, resource->group);
						Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup(resource->group);

						if (resource->loadAtStart == true)
						{
							Ogre::ResourceManager::ResourceMapIterator rmiIterator =
								Ogre::MaterialManager::getSingletonPtr()->getResourceIterator();

							while (rmiIterator.hasMoreElements() == true)
							{
								Ogre::ResourceManager::ResourceHandleMap::mapped_type mtResource = rmiIterator.getNext();
								std::string groupName = mtResource->getGroup();

								if (groupName.find(resource->group) != std::string::npos)
								{
									mtResource->load();
									mtResource->touch();
								}
							}
						}
					}
				}

				void Canvas3DFrame::onClose(wxCloseEvent &evt)
				{
					if (IsTopLevel() == true)
					{
						V8_JAVASCRIPT_ENGINE->exit(0);

						return;
					}

					Destroy();
				}

				void Canvas3DFrame::onJSClose(wxCloseEvent &evt)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)evt.GetEventUserData();
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
				}

				void Canvas3DFrame::onJSMinimized(wxIconizeEvent &evt)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)evt.GetEventUserData();
					executeEvent(pevent);
				}

				void Canvas3DFrame::onClick(wxMouseEvent &evt)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)evt.GetEventUserData();
					executeEvent(pevent);
				}
			#endif

			#ifdef USE_V8
			Canvas3D::Canvas3D(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args)
			{
			}
			#endif

			Canvas3D::Canvas3D(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void Canvas3D::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					try {
						V8_JAVASCRIPT_ENGINE->start3DEngine();
						Ogre::RenderWindow *rwindow = V8_JAVASCRIPT_ENGINE->mRoot->initialise(true, _text);

						TempCanvasObj *object = RJNEW TempCanvasObj();
						object->mRoot = V8_JAVASCRIPT_ENGINE->mRoot;
						object->renderWindow = rwindow;

						_appObj = (wxWindow *)object;
					}
					catch(Ogre::Exception e)
					{
						std::cerr << e.what();
					}

					setup();
				#endif
			}

			void Canvas3D::setPosition(RJINT x, RJINT y)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						CPP::GUI::Canvas3DFrame *obj = (CPP::GUI::Canvas3DFrame *)_appObj->GetParent();
						obj->SetPosition(wxPoint(x, y));
					}
				#endif
			}

			CPP::Vector2 Canvas3D::getPosition()
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

			void Canvas3D::setSize(RJINT width, RJINT height)
			{
				_transform->setSize(width, height);

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						_appObj->GetParent ()->SetSize(width, height);
				#endif
			}

			CPP::Vector2 Canvas3D::getSize()
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

			void Canvas3D::setText(String text)
			{
				_text = text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						((Canvas3DFrame *)_appObj->GetParent())->SetTitle(text.towxString());
				#endif
			}

			String Canvas3D::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxString wxtext = ((Canvas3DFrame *)_appObj->GetParent ())->GetTitle ();
						text = parsewxString(wxtext);
					}
				#endif

				return (text);
			}

			void Canvas3D::setVisibility(RJBOOL visible)
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

			RJBOOL Canvas3D::getVisibility()
			{
				RJBOOL visible = _visible;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						visible = _appObj->GetParent ()->IsShown();
				#endif

				return (visible);
			}

			void Canvas3D::setEnabled(RJBOOL enabled)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						if (enabled == true)
							((Canvas3DFrame *)_appObj->GetParent())->Enable();
						else
							((Canvas3DFrame *)_appObj->GetParent())->Disable();
					}
				#endif
			}

			RJBOOL Canvas3D::getEnabled()
			{
				RJBOOL enabled = false;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						enabled = ((Canvas3DFrame *)_appObj->GetParent())->IsEnabled();
				#endif

				return (enabled);
			}

			#ifdef USE_V8
			void Canvas3D::on(String event, v8::Local<v8::Function> func)
			{
				CPP::GUI::Canvas3DFrame *object = (CPP::GUI::Canvas3DFrame *)_appObj;
				CPP::GUI::Canvas3DFrame *obj = (CPP::GUI::Canvas3DFrame *)object->GetParent();

				obj->addNewEvent(event, obj, func);

				if (event == "close")
				{
					v8::Persistent<v8::Value> *pevent = obj->createEvent(event, func);
					obj->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(Canvas3DFrame::onJSClose), (wxObject *)pevent);
				}

				if (event == "minimize")
				{
					v8::Persistent<v8::Value> *pevent = obj->createEvent(event, func);
					obj->Connect(wxEVT_ICONIZE, wxIconizeEventHandler(Canvas3DFrame::onJSMinimized), (wxObject *)pevent);
				}
			}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::Canvas3DFrame, wxFrame)
		EVT_CLOSE(RadJAV::CPP::GUI::Canvas3DFrame::onClose)
	wxEND_EVENT_TABLE()
#endif

