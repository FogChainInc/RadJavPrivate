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
#include "v8/RadJavV8GUICanvas3D.h"

#include "RadJav.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>
#include <OgreMovableObject.h>
#include <OgreSceneQuery.h>

#include "cpp/RadJavCPPGUICanvas3D.h"
#include "cpp/RadJavCPPC3DObject3D.h"

#define UITYPE CPP::GUI::Canvas3D

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
			void Canvas3D::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Canvas3D::create);
				V8_CALLBACK(object, "_setAmbientLightColor", Canvas3D::setAmbientLight);
				V8_CALLBACK(object, "_getAmbientLightColor", Canvas3D::getAmbientLight);
				V8_CALLBACK(object, "_addToScene", Canvas3D::addToScene);
				V8_CALLBACK(object, "_removeFromScene", Canvas3D::removeFromScene);
				V8_CALLBACK(object, "_createObject3D", Canvas3D::createObject3D);
				V8_CALLBACK(object, "_createCamera", Canvas3D::createCamera);
				V8_CALLBACK(object, "_createLight", Canvas3D::createLight);
				V8_CALLBACK(object, "_createPlane", Canvas3D::createPlane);
				V8_CALLBACK(object, "_createCube", Canvas3D::createCube);
				V8_CALLBACK(object, "_createSphere", Canvas3D::createSphere);
				V8_CALLBACK(object, "_loadModel", Canvas3D::loadModel);
			}

			void Canvas3D::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = RJNEW UITYPE(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}

			void Canvas3D::setAmbientLight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Color nativeColor;
				
				if(args.Length() > 0)
				{
					v8::Isolate* isolate = args.GetIsolate();
					v8::Handle<v8::Object> color = v8::Handle<v8::Object>::Cast(args[0]);

					nativeColor.r = V8_JAVASCRIPT_ENGINE->v8GetDecimal(color, "r");
					nativeColor.g = V8_JAVASCRIPT_ENGINE->v8GetDecimal(color, "g");
					nativeColor.b = V8_JAVASCRIPT_ENGINE->v8GetDecimal(color, "b");
					nativeColor.a = V8_JAVASCRIPT_ENGINE->v8GetDecimal(color, "a");
				}
				
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				
				if (appObject)
					appObject->setAmbientLight(nativeColor);
			}
			
			void Canvas3D::getAmbientLight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Color color;
				
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				
				if (appObject)
					color = appObject->getAmbientLight();

				v8::Handle<v8::Function> func = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "Color");
				v8::Local<v8::Object> v8color = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(func, 0, NULL);
				
				V8_JAVASCRIPT_ENGINE->v8SetNumber(v8color, "r", color.r);
				V8_JAVASCRIPT_ENGINE->v8SetNumber(v8color, "g", color.g);
				V8_JAVASCRIPT_ENGINE->v8SetNumber(v8color, "b", color.b);
				V8_JAVASCRIPT_ENGINE->v8SetNumber(v8color, "a", color.a);
				
				args.GetReturnValue().Set(v8color);
			}
			
			void Canvas3D::addToScene(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0 || args[0]->IsNullOrUndefined())
				{
					V8_JAVASCRIPT_ENGINE->throwException("Missing child object");
					return;
				}

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				
				if (!appObject || args.Length() == 0)
					return;
				
				v8::Local<v8::Object> child = args[0]->ToObject(args.GetIsolate ());
				
				std::shared_ptr<CPP::C3D::Transform> childNative = V8_JAVASCRIPT_ENGINE->v8GetExternal<CPP::C3D::Transform>(child, "_c3dObj");

				if(childNative)
					appObject->addChild(*childNative);
			}
			
			void Canvas3D::removeFromScene(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0 || args[0]->IsNullOrUndefined())
				{
					V8_JAVASCRIPT_ENGINE->throwException("Missing child object");
					return;
				}

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				
				if (!appObject || args.Length() == 0)
					return;
				
				v8::Local<v8::Object> child = args[0]->ToObject(args.GetIsolate());
				
				std::shared_ptr<CPP::C3D::Transform> childNative = V8_JAVASCRIPT_ENGINE->v8GetExternal<CPP::C3D::Transform>(child, "_c3dObj");

				if(childNative)
					appObject->removeChild(*childNative);
			}

			void Canvas3D::createObject3D(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> nameObj;
				v8::Local<v8::Value> thisObj = args.This()->ToObject(args.GetIsolate ());
				
				if(args.Length())
					nameObj = args[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Object3D");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 2, arguments);
				
				args.GetReturnValue().Set(newObject);
			}
			
			void Canvas3D::createCamera(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> nameObj;
				v8::Local<v8::Value> thisObj = args.This()->ToObject(args.GetIsolate());
				
				if(args.Length())
					nameObj = args[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Camera");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 2, arguments);
				
				args.GetReturnValue().Set(newObject);
			}
			
			void Canvas3D::createLight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> nameObj;
				v8::Local<v8::Value> thisObj = args.This()->ToObject(args.GetIsolate());
				
				if(args.Length())
					nameObj = args[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Light");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 2, arguments);
				
				args.GetReturnValue().Set(newObject);
			}
			
			void Canvas3D::createPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> nameObj;
				v8::Local<v8::Value> thisObj = args.This()->ToObject(args.GetIsolate());
				
				if(args.Length())
					nameObj = args[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Plane");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 2, arguments);
				
				args.GetReturnValue().Set(newObject);
			}
			
			void Canvas3D::createCube(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> nameObj;
				v8::Local<v8::Value> thisObj = args.This()->ToObject(args.GetIsolate());
				
				if(args.Length())
					nameObj = args[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Cube");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 2, arguments);
				
				args.GetReturnValue().Set(newObject);
			}
			
			void Canvas3D::createSphere(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> nameObj;
				v8::Local<v8::Value> thisObj = args.This()->ToObject(args.GetIsolate());
				
				if(args.Length())
					nameObj = args[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Sphere");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 2, arguments);
				
				args.GetReturnValue().Set(newObject);
			}
			
			void Canvas3D::loadModel(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> pathObj;
				v8::Local<v8::Value> nameObj;
				v8::Local<v8::Value> thisObj = args.This()->ToObject(args.GetIsolate());
				
				size_t numOfArgs = args.Length();
				if(numOfArgs == 0)
				{
					V8_JAVASCRIPT_ENGINE->throwException("No path specified");
					return;
				}
				
				pathObj = args[0];
				
				if(numOfArgs == 1)
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				else
					nameObj = args[1];
				
				v8::Handle<v8::Value> arguments[] = { thisObj, pathObj, nameObj };
				
				v8::Local<v8::Object> C3D = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "C3D");
				v8::Local<v8::Object> object = V8_JAVASCRIPT_ENGINE->v8GetObject(C3D, "Model");
				v8::Local<v8::Object> newObject = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(object, 3, arguments);
				
				v8::Local<v8::Object> loadFunc = V8_JAVASCRIPT_ENGINE->v8GetObject(newObject, "_load");
				if(loadFunc->IsFunction())
				{
					//TODO: Not sure what to do if it fails, maybe we can add isLoaded method to Model itself
					V8_JAVASCRIPT_ENGINE->v8CallFunction(newObject, "_load", 0, nullptr);
				}
				
				args.GetReturnValue().Set(newObject);
			}
		}
	}
}
