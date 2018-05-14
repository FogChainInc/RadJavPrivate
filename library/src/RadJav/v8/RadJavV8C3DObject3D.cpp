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
#include "v8/RadJavV8C3DObject3D.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#include "cpp/RadJavCPPC3DObject3D.h"

#define C3DTYPE CPP::C3D::Object3D

namespace RadJAV
{
	namespace V8B
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			void Object3D::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "setPosition", Object3D::setPosition);
				V8_CALLBACK(object, "getPosition", Object3D::getPosition);
				V8_CALLBACK(object, "pitch", Object3D::pitch);
				V8_CALLBACK(object, "roll", Object3D::roll);
				V8_CALLBACK(object, "yaw", Object3D::yaw);
				V8_CALLBACK(object, "getX", Object3D::getX);
				V8_CALLBACK(object, "getY", Object3D::getY);
				V8_CALLBACK(object, "getZ", Object3D::getZ);
				V8_CALLBACK(object, "getParent", Object3D::getParent);
				V8_CALLBACK(object, "setVisibility", Object3D::setVisibility);
				V8_CALLBACK(object, "getVisibility", Object3D::getVisibility);
				V8_CALLBACK(object, "addChild", Object3D::addChild);
				V8_CALLBACK(object, "removeChild", Object3D::removeChild);
				V8_CALLBACK(object, "lookAt", Object3D::lookAt);

				V8_CALLBACK(object, "on", Object3D::on);
			}

			void Object3D::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL x = 0;
				RDECIMAL y = 0;
				RDECIMAL z = 0;
				
				if (args.Length() > 1)
				{
					x = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
					y = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[1]);
					z = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[2]);
				}
				else
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[0]);
					x = V8_JAVASCRIPT_ENGINE->v8GetDecimal(obj, "x");
					y = V8_JAVASCRIPT_ENGINE->v8GetDecimal(obj, "y");
					z = V8_JAVASCRIPT_ENGINE->v8GetDecimal(obj, "z");
				}
				
				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				v8::Local<v8::Value> *args3 = RJNEW v8::Local<v8::Value>[3];
				args3[0] = v8::Number::New(args.GetIsolate(), x);
				args3[1] = v8::Number::New(args.GetIsolate(), y);
				args3[2] = v8::Number::New(args.GetIsolate(), z);
				V8_JAVASCRIPT_ENGINE->v8CallFunction(transform, "setPosition", 3, args3);
				
				DELETE_ARRAY(args3);
				
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				if (c3dObject != NULL)
					c3dObject->setPosition(Ogre::Vector3(x, y, z));
			}

			void Object3D::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				RDECIMAL x = 0;
				RDECIMAL y = 0;
				RDECIMAL z = 0;
				
				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				x = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "x");
				y = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "y");
				y = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "z");

				Ogre::Vector3 pos = c3dObject->getPosition();
				
				x = pos.x;
				y = pos.y;
				z = pos.z;
				
				v8::Local<v8::Object> vector3 = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector3");
				v8::Local<v8::Object> vector3Obj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(vector3, 0, NULL);
				vector3Obj->Set(String("x").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), x));
				vector3Obj->Set(String("y").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), y));
				vector3Obj->Set(String("z").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), z));

				args.GetReturnValue().Set(vector3Obj);
			}
			
			void Object3D::pitch(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL value = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				c3dObject->pitch(Ogre::Degree(value));
			}
			
			void Object3D::roll(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL value = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				c3dObject->roll(Ogre::Degree(value));
			}

			void Object3D::yaw(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL value = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				c3dObject->yaw(Ogre::Degree(value));
			}

			void Object3D::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				Ogre::Vector3 pos = c3dObject->getPosition();
				args.GetReturnValue().Set(pos.x);
			}

			void Object3D::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				Ogre::Vector3 pos = c3dObject->getPosition();
				args.GetReturnValue().Set(pos.y);
			}

			void Object3D::getZ(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				Ogre::Vector3 pos = c3dObject->getPosition();
				args.GetReturnValue().Set(pos.z);
			}

			void Object3D::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				v8::Handle<v8::Object> parent = V8_JAVASCRIPT_ENGINE->v8GetObject(transform, "_parent");

				args.GetReturnValue().Set(parent);
			}

			void Object3D::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL visible = true;
				
				if (args.Length() > 0)
					visible = v8::Local<v8::Boolean>::Cast(args[0])->BooleanValue();

				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				
				c3dObject->setVisible(visible);
			}

			void Object3D::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				args.GetReturnValue().Set(c3dObject->getVisible());
			}

			void Object3D::addChild(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *c3dChildObject = NULL;
				
				if(args.Length() == 0)
					return;
				
				v8::Handle<v8::Object> childObject = v8::Handle<v8::Object>::Cast(args[0]);
				c3dChildObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(childObject, "_c3dObj");

				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				c3dObject->addChild(c3dChildObject);
			}
			
			void Object3D::removeChild(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *c3dChildObject = NULL;
				
				if(args.Length() == 0)
					return;
				
				v8::Handle<v8::Object> childObject = v8::Handle<v8::Object>::Cast(args[0]);
				c3dChildObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(childObject, "_c3dObj");

				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				c3dObject->removeChild(c3dChildObject);
			}

			void Object3D::lookAt(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL x = 0;
				RDECIMAL y = 0;
				RDECIMAL z = 0;
				
				if (args.Length() > 1)
				{
					x = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
					y = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[1]);
					z = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[2]);
				}
				else
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[0]);
					x = V8_JAVASCRIPT_ENGINE->v8GetDecimal(obj, "x");
					y = V8_JAVASCRIPT_ENGINE->v8GetDecimal(obj, "y");
					z = V8_JAVASCRIPT_ENGINE->v8GetDecimal(obj, "z");
				}
				
				C3DTYPE *c3dObject = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				if (c3dObject != NULL)
					c3dObject->lookAt(x, y, z);
			}

			void Object3D::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			v8::Persistent<v8::Value> *Object3DBase::createEvent(String event, v8::Local<v8::Function> function)
			{
				v8::Persistent<v8::Value> *persistent = RJNEW v8::Persistent<v8::Value>();
				persistent->Reset(V8_JAVASCRIPT_ENGINE->isolate, function);

				auto found = events.find(event);
				auto end = events.end();

				if (found != end)
				{
					v8::Persistent<v8::Value> *evt = events.at(event);
					DELETE_OBJ(evt);

					events.erase(event);
				}

				events.insert(HashMapPair<String, v8::Persistent<v8::Value> *>(event, persistent));

				return (persistent);
			}

			void Object3DBase::addNewEvent(String event, Ogre::MovableObject *object, v8::Local<v8::Function> func)
			{
			}

			v8::Local<v8::Value> Object3DBase::executeEvent(v8::Persistent<v8::Value> *pevent, RJINT numArgs, v8::Local<v8::Value> *args)
			{
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(pevent->Get(V8_JAVASCRIPT_ENGINE->isolate));
				v8::Local<v8::Value> result;

				if (V8_JAVASCRIPT_ENGINE->v8IsNull(func) == false)
					result = func->Call(V8_JAVASCRIPT_ENGINE->globalContext->Global(), numArgs, args);

				return (result);
			}
#endif
		}
	}
}
#endif

