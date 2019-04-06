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
#include "cpp/desktop/RadJavCPPGUICanvas3DFrame.h"

#include "RadJav.h"
#include "RadJavString.h"
#include "cpp/RadJavCPPC3DTransform.h"
#include "cpp/desktop/wxOgreRenderWindow.h"

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

				wxBEGIN_EVENT_TABLE(Canvas3DFrame, wxFrame)
				EVT_CLOSE(Canvas3DFrame::onClose)
				wxEND_EVENT_TABLE()
			#endif
		}
	}
}
