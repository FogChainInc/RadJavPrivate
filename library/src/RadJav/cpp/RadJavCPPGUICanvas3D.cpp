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
#include "cpp/RadJavCPPC3DTransform.h"
#include "wxOgreRenderWindow.h"

#ifdef C3D_USE_OGRE
	#include <Ogre.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				Canvas3DFrame::Canvas3DFrame(const wxString &text, const wxPoint &pos, const wxSize &size, Array<CanvasResource *> resources)
					: wxFrame(NULL, wxID_ANY, text, pos, size),
					  renderingWidget(nullptr),
					  sceneManager(nullptr)
				{
					// Create dummy window just to hold device, prevents constant lost devices
					// when resizing the 'real' viewable windows
					// We'll never render to this window or give it a viewport or camera, it's
					// only there to hold the rendering device
					createRenderWindow(this, wxSize(1, 1), false);
					
                    // Creation of first window will have created rendering context
                    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

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

					Ogre::Root* ogreRoot = Ogre::Root::getSingletonPtr();
					if(!ogreRoot)
						return;
					
					//Create widget to render to
					renderingWidget = RJNEW wxOgreRenderWindow( ogreRoot, this, wxID_ANY, wxPoint(0, 0), size);

					//Create SceneManager if not already
					try
					{
						sceneManager = ogreRoot->getSceneManager("sceneManager");
					}
					catch(Ogre::Exception)
					{
						if(!sceneManager)
						{
							sceneManager = ogreRoot->createSceneManager(Ogre::String("OctreeSceneManager"),
																		Ogre::String("sceneManager"));
							
						}
					}
				}

				void Canvas3DFrame::addChild(const C3D::Transform& child)
				{
					if(sceneManager)
					{
						sceneManager->getRootSceneNode()->addChild(child.node);
					}
				}

				void Canvas3DFrame::removeChild(const C3D::Transform& child)
				{
					if(sceneManager)
					{
						sceneManager->getRootSceneNode()->removeChild(child.node);
					}
				}

				void Canvas3DFrame::onClose(wxCloseEvent &evt)
				{
					if (IsTopLevel() == true)
					{
						wxExit();
						return;
					}
					
					Destroy();
				}

				void Canvas3DFrame::onJSClose(wxCloseEvent &evt)
				{
					//TODO: add JavaScriptCore implementation
					#ifdef USE_V8
						Event *pevent = (Event *)evt.GetEventUserData();
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
				}

				void Canvas3DFrame::onJSMinimized(wxIconizeEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}

				void Canvas3DFrame::onClick(wxMouseEvent &evt)
				{
					Event *pevent = (Event *)evt.GetEventUserData();
					executeEvent(pevent);
				}
			#endif

			#ifdef USE_V8
			Canvas3D::Canvas3D(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args)
			{
			}
			#elif defined USE_JAVASCRIPTCORE
			Canvas3D::Canvas3D(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
				: GObject (jsEngine, thisObject, argumentCount, arguments)
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
                    //Initializing 3D Engine, if not already(handled inside)
					#ifdef USE_V8
                    	V8_JAVASCRIPT_ENGINE->start3DEngine();
					#elif defined USE_JAVASCRIPTCORE
						JSC_JAVASCRIPT_ENGINE->start3DEngine();
					#endif

                    Canvas3DFrame *object = RJNEW Canvas3DFrame( _text,
                                                                wxPoint(_transform->x, _transform->y),
                                                                wxSize(_transform->width, _transform->height),
                                                                Array<CanvasResource*>());
                
                    RadJav::app->SetTopWindow(object);
                    object->Show(_visible);
                    RadJav::app->SetActive(_visible, object);
                
                    _appObj = object;
					linkWith(object);
                
                    //if (icon != "")
                    //    setIcon(icon);
                
                    setup();
				#endif
			}

            Ogre::RenderWindow* Canvas3D::getRenderWindow() const
            {
                if (_appObj == NULL)
                    return nullptr;
                
                Canvas3DFrame* win = dynamic_cast<Canvas3DFrame*>(_appObj);
                if(win && win->renderingWidget)
				{
					return win->renderingWidget->getRenderWindow();
				}

				return nullptr;
            }
			
			Ogre::SceneManager* Canvas3D::getSceneManager() const
			{
				if (_appObj == NULL)
					return nullptr;
				
				Canvas3DFrame* win = dynamic_cast<Canvas3DFrame*>(_appObj);
				if(win && win->sceneManager)
				{
					return win->sceneManager;
				}
				
				return nullptr;
			}

			void Canvas3D::setPosition(RJINT x, RJINT y)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						_appObj->SetPosition( wxPoint(x, y));
					}
				#endif
			}

			CPP::Vector2 Canvas3D::getPosition()
			{
				CPP::Vector2 newpos;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxPoint pos = _appObj->GetPosition();
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
						_appObj->SetSize(width, height);
				#endif
			}

			CPP::Vector2 Canvas3D::getSize()
			{
				CPP::Vector2 size = _transform->getSize();

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxSize wxsize = _appObj->GetSize();
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
                    {
                        wxFrame* win = dynamic_cast<wxFrame*>(_appObj);
                        if (win)
                            win->SetTitle(text.towxString());
                    }
				#endif
			}

			String Canvas3D::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
                        wxFrame* win = dynamic_cast<wxFrame*>(_appObj);
                        if (win)
                        {
                            wxString wxtext = win->GetTitle ();
                            text = parsewxString(wxtext);
                        }
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
							_appObj->Show();
						else
							_appObj->Hide();
					}
				#endif
			}

			RJBOOL Canvas3D::getVisibility()
			{
				RJBOOL visible = _visible;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						visible = _appObj->IsShown();
				#endif

				return (visible);
			}

			void Canvas3D::setEnabled(RJBOOL enabled)
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

			RJBOOL Canvas3D::getEnabled()
			{
				RJBOOL enabled = false;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						enabled = _appObj->IsEnabled();
				#endif

				return (enabled);
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
			void Canvas3D::on(String event, RJ_FUNC_TYPE func)
			{
				CPP::GUI::Canvas3DFrame *object = (CPP::GUI::Canvas3DFrame *)_appObj;
				CPP::GUI::Canvas3DFrame *obj = (CPP::GUI::Canvas3DFrame *)object->GetParent();

				obj->addNewEvent(event, obj, func);

				if (event == "close")
				{
					obj->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(Canvas3DFrame::onJSClose), obj->createEvent(event, func));
				}

				if (event == "minimize")
				{
					obj->Connect(wxEVT_ICONIZE, wxIconizeEventHandler(Canvas3DFrame::onJSMinimized), obj->createEvent(event, func));
				}
			}
			#endif
			
			void Canvas3D::setAmbientLight(const Color& color)
			{
				Ogre::SceneManager* sceneManager = getSceneManager();
				if(sceneManager)
				{
					Ogre::ColourValue ogreColor(color.r, color.g, color.b, color.a);
					sceneManager->setAmbientLight(ogreColor);
				}
			}

			Color Canvas3D::getAmbientLight() const
			{
				Color color;
				
				Ogre::SceneManager* sceneManager = getSceneManager();
				if(sceneManager)
				{
					Ogre::ColourValue ogreColor = sceneManager->getAmbientLight();
					color.r = ogreColor.r;
					color.g = ogreColor.g;
					color.b = ogreColor.b;
					color.a = ogreColor.a;
				}
				
				return color;
			}

			void Canvas3D::addChild(const C3D::Transform& child)
			{
				CPP::GUI::Canvas3DFrame* object = static_cast<CPP::GUI::Canvas3DFrame*>(_appObj);
				
				if(object)
				{
					object->addChild(child);
				}
			}
			
			void Canvas3D::removeChild(const C3D::Transform& child)
			{
				CPP::GUI::Canvas3DFrame* object = static_cast<CPP::GUI::Canvas3DFrame*>(_appObj);
				
				if(object)
				{
					object->addChild(child);
				}
			}
			
#ifdef GUI_USE_WXWIDGETS
			wxBEGIN_EVENT_TABLE(Canvas3DFrame, wxFrame)
			EVT_CLOSE(Canvas3DFrame::onClose)
			wxEND_EVENT_TABLE()
#endif
		}
	}
}

