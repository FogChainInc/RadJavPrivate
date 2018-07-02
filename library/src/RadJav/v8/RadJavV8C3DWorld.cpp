/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#include "v8/RadJavV8C3DWorld.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace C3D
		{
			#ifdef C3D_USE_OGRE
			void World::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "createEntity", World::createEntity);
				V8_CALLBACK(object, "setAmbientLight", World::setAmbientLight);
			}

			void World::createEntity(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String name = parseV8Value(args[0]);
				RJINT type = 1;

				if (args.Length() > 1)
					type = v8::Local<v8::Integer>::Cast(args[1])->IntegerValue();

				std::shared_ptr<Ogre::SceneManager> sceneMgr = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::SceneManager>(args.This(), "_sceneManager");

				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> entity = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Entity");
				v8::Local<v8::Object> newEntity = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(entity, 0, NULL);
				
				std::shared_ptr<Ogre::MovableObject> object;
				std::shared_ptr<Ogre::SceneNode> node( sceneMgr->getRootSceneNode()->createChildSceneNode(), [](Ogre::SceneNode* p){
					Ogre::SceneManager* sceneManager = Ogre::Root::getSingleton()._getCurrentSceneManager();
					if(sceneManager)
						sceneManager->destroySceneNode(p);
				});

				auto movableObjectDestroyer = [](Ogre::MovableObject* p)
				{
					Ogre::SceneManager* sceneManager = Ogre::Root::getSingleton()._getCurrentSceneManager();
					if(sceneManager)
						sceneManager->destroyMovableObject(p);
				};
				
				switch (type)
				{
					case 1:		// Cube
						object.reset( sceneMgr->createEntity(name, Ogre::SceneManager::PrefabType::PT_CUBE), movableObjectDestroyer);
						break;
					case 2:		// Sphere
						object.reset( sceneMgr->createEntity(name, Ogre::SceneManager::PrefabType::PT_SPHERE), movableObjectDestroyer);
						break;
					case 3:		// Plane
						object.reset( sceneMgr->createEntity(name, Ogre::SceneManager::PrefabType::PT_PLANE), movableObjectDestroyer);
						break;
					case 4:		// Camera
						{
							Ogre::Camera *camera = sceneMgr->createCamera(name);
							camera->setNearClipDistance(1);
							camera->setAutoAspectRatio(true);
							camera->setFarClipDistance(5000);

							std::shared_ptr<Ogre::RenderWindow> rwin = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::RenderWindow>(args.This(), "_renderWindow");
							if(rwin)
							{
								Ogre::Viewport *viewport = rwin->addViewport(camera);
								
								Ogre::Real rAspectRatio = static_cast <Ogre::Real> (viewport->getActualWidth()) /
								static_cast <Ogre::Real> (viewport->getActualHeight());
								camera->setAspectRatio(rAspectRatio);
								camera->setFOVy(Ogre::Degree(90) / rAspectRatio);
							}
							else
							{
								camera->setAspectRatio(1.0);
								camera->setFOVy(Ogre::Degree(90));
							}

							object.reset(camera, movableObjectDestroyer);
						}
						break;
					case 5:		// Light
						{
							object.reset(sceneMgr->createLight(name), movableObjectDestroyer);
						}
						break;
				}

				node->attachObject(object.get());
				
				V8_JAVASCRIPT_ENGINE->v8SetExternal(newEntity, "_c3dObj", node);
				V8_JAVASCRIPT_ENGINE->v8SetExternal(newEntity, "_c3dEntity", object);

				args.GetReturnValue().Set(newEntity);
			}

			void World::setAmbientLight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<Ogre::SceneManager> sceneMgr = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::SceneManager>(args.This(), "_sceneManager");
				if(sceneMgr)
					sceneMgr->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
			}
			#endif
		}
	}
}
#endif

