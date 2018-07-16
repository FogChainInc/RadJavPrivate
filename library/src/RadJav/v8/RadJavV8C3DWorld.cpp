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
#include "v8/RadJavV8C3DObject3D.h"
#include "cpp/RadJavCPPC3DCamera.h"
#include "cpp/RadJavCPPC3DLight.h"
#include "cpp/RadJavCPPC3DPlane.h"
#include "cpp/RadJavCPPC3DCube.h"
#include "cpp/RadJavCPPC3DSphere.h"
#include "cpp/RadJavCPPC3DModel.h"
#include "cpp/RadJavCPPC3DModelFactory.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace C3D
		{
			#ifdef C3D_USE_OGRE
			
			Ogre::SceneManager::PrefabType jsPrimitiveTypeToNative(RJINT primitiveType)
			{
				switch(primitiveType)
				{
					case 1:
						return Ogre::SceneManager::PT_PLANE;
					break;
					case 2:
						return Ogre::SceneManager::PT_CUBE;
					break;
					case 3:
						return Ogre::SceneManager::PT_SPHERE;
					break;
						
					default:
						return Ogre::SceneManager::PT_PLANE;
				}
			}
			
			void World::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				//V8_CALLBACK(object, "createEntity", World::createEntity);
				V8_CALLBACK(object, "createCamera", World::createCamera);
				V8_CALLBACK(object, "createLight", World::createLight);
				V8_CALLBACK(object, "createPrimitive", World::createPrimitive);
				V8_CALLBACK(object, "createEmpty", World::createEmpty);
				V8_CALLBACK(object, "loadModel", World::loadModel);
				V8_CALLBACK(object, "setAmbientLight", World::setAmbientLight);
			}

			void World::createCamera(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String name = parseV8Value(args[0]);

				//Object3D* obj3D = NULL;
				
				/*if (args.Length() > 1)
				{
					v8::Local<v8::Object> object3D = v8::Local<v8::Object>::Cast(args[1]);
					obj3D = (Object3D *)V8_JAVASCRIPT_ENGINE->v8GetExternal(object3D, "_c3dObj");
				}*/
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Camera");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 0, NULL);
				
				std::shared_ptr<Ogre::SceneManager> sceneMgr = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::SceneManager>(args.This(), "_sceneManager");
				std::shared_ptr<Ogre::RenderWindow> renderWin = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::RenderWindow>(args.This(), "_renderWindow");

				std::shared_ptr<CPP::C3D::Camera> nativeObject(RJNEW CPP::C3D::Camera(*sceneMgr, *renderWin, name), [](CPP::C3D::Camera* p){});
				V8_JAVASCRIPT_ENGINE->v8SetExternal(newObject, "_c3dObj", nativeObject);

				args.GetReturnValue().Set(newObject);
			}
			
			void World::createLight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String name = parseV8Value(args[0]);
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Light");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 0, NULL);
				
				std::shared_ptr<Ogre::SceneManager> sceneMgr = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::SceneManager>(args.This(), "_sceneManager");
				
				std::shared_ptr<CPP::C3D::Light> nativeObject(RJNEW CPP::C3D::Light(*sceneMgr, Ogre::Light::LT_DIRECTIONAL, name), [](CPP::C3D::Light* p){});
				V8_JAVASCRIPT_ENGINE->v8SetExternal(newObject, "_c3dObj", nativeObject);
				
				args.GetReturnValue().Set(newObject);
			}
			
			void World::createPrimitive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String name = parseV8Value(args[0]);
				
				RJINT type = 1;
				if (args.Length() > 1)
					type = v8::Local<v8::Integer>::Cast(args[1])->IntegerValue();
				
				Ogre::SceneManager::PrefabType prefabType = jsPrimitiveTypeToNative(type);
				
				std::shared_ptr<Ogre::SceneManager> sceneMgr = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::SceneManager>(args.This(), "_sceneManager");
				std::shared_ptr<CPP::C3D::Object3D> nativeObject = NULL;
				
				switch(prefabType)
				{
					case Ogre::SceneManager::PT_PLANE:
						nativeObject.reset(RJNEW CPP::C3D::Plane(*sceneMgr, name), [](CPP::C3D::Plane* p){});
					break;
					case Ogre::SceneManager::PT_CUBE:
						nativeObject.reset(RJNEW CPP::C3D::Cube(*sceneMgr, name), [](CPP::C3D::Cube* p){});
					break;
					case Ogre::SceneManager::PT_SPHERE:
						nativeObject.reset(RJNEW CPP::C3D::Sphere(*sceneMgr, name), [](CPP::C3D::Sphere* p){});
					break;
					default:;
				}

				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Object3D");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 0, NULL);

				V8_JAVASCRIPT_ENGINE->v8SetExternal(newObject, "_c3dObj", nativeObject);
				
				args.GetReturnValue().Set(newObject);
			}

			void World::createEmpty(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String name = parseV8Value(args[0]);
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Object3D");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 0, NULL);
				
				std::shared_ptr<Ogre::SceneManager> sceneMgr = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::SceneManager>(args.This(), "_sceneManager");

				std::shared_ptr<CPP::C3D::Object3D> nativeObject(RJNEW CPP::C3D::Object3D(*sceneMgr, name), [](CPP::C3D::Object3D* p){});
				V8_JAVASCRIPT_ENGINE->v8SetExternal(newObject, "_c3dObj", nativeObject);
				
				args.GetReturnValue().Set(newObject);
			}

			void World::loadModel(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String name = parseV8Value(args[0]);
				String path;
				if (args.Length() > 1)
					path = parseV8Value(args[1]);

				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Object3D");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 0, NULL);
				
				std::shared_ptr<Ogre::SceneManager> sceneMgr = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::SceneManager>(args.This(), "_sceneManager");
				
				std::shared_ptr<CPP::C3D::Model> model(CPP::C3D::ModelFactory::load( sceneMgr.get(), name, path, nullptr), [](CPP::C3D::Model* p){});
				V8_JAVASCRIPT_ENGINE->v8SetExternal(newObject, "_c3dObj", model);
				
				args.GetReturnValue().Set(newObject);
			}

			/*
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
			 */
			
			void World::setAmbientLight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				Ogre::ColourValue nativeColor;
				
				if(args.Length() > 0)
				{
					v8::Isolate* isolate = args.GetIsolate();
					v8::Handle<v8::Object> color = v8::Handle<v8::Object>::Cast(args[0]);
					
					nativeColor.r = color->Get( String("r").toV8String(isolate))->NumberValue();
					nativeColor.g = color->Get( String("g").toV8String(isolate))->NumberValue();
					nativeColor.b = color->Get( String("b").toV8String(isolate))->NumberValue();
					nativeColor.a = color->Get( String("a").toV8String(isolate))->NumberValue();
				}
				
				std::shared_ptr<Ogre::SceneManager> sceneMgr = V8_JAVASCRIPT_ENGINE->v8GetExternal<Ogre::SceneManager>(args.This(), "_sceneManager");
				if(sceneMgr)
					sceneMgr->setAmbientLight(nativeColor);
			}
			#endif
		}
	}
}
#endif

