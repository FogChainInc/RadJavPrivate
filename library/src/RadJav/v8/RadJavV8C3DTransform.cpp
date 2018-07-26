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
#include "v8/RadJavV8C3DTransform.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#include "cpp/RadJavCPPC3DTransform.h"

#define C3DTYPE CPP::C3D::Transform

namespace RadJAV
{
	namespace V8B
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			void Transform::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_init", Transform::init);
				V8_CALLBACK(object, "_setPosition", Transform::setPosition);
				V8_CALLBACK(object, "_getPosition", Transform::getPosition);
				V8_CALLBACK(object, "_pitch", Transform::pitch);
				V8_CALLBACK(object, "_roll", Transform::roll);
				V8_CALLBACK(object, "_yaw", Transform::yaw);
				V8_CALLBACK(object, "_getX", Transform::getX);
				V8_CALLBACK(object, "_getY", Transform::getY);
				V8_CALLBACK(object, "_getZ", Transform::getZ);
				V8_CALLBACK(object, "_setScale", Transform::setScale);
				V8_CALLBACK(object, "_getScale", Transform::getScale);
				V8_CALLBACK(object, "_addChild", Transform::addChild);
				V8_CALLBACK(object, "_removeChild", Transform::removeChild);
				V8_CALLBACK(object, "_lookAt", Transform::lookAt);
				V8_CALLBACK(object, "_setDirection", Transform::setDirection);
			}
			
			void Transform::init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0 || args[0]->IsNullOrUndefined())
				{
					V8_JAVASCRIPT_ENGINE->throwException("Missing Canvas3D parameter");
					return;
				}

				//Check if we were already contructed
				std::shared_ptr<C3DTYPE> object = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				if(object)
					return;
				
				CPP::GUI::Canvas3D* canvas =
					(CPP::GUI::Canvas3D*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args[0]->ToObject(), "_appObj");
				
				if(!canvas)
					return;
				
				String name = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "name");
				object.reset(RJNEW C3DTYPE(*canvas, name), [](C3DTYPE* p){DELETEOBJ(p)});
				V8_JAVASCRIPT_ENGINE->v8SetExternal( args.This(), "_c3dObj", object);
			}
			
			void Transform::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
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
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				if (c3dObject)
					c3dObject->setPosition(Ogre::Vector3(x, y, z));
			}
			
			void Transform::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL x = 0;
				RDECIMAL y = 0;
				RDECIMAL z = 0;
				
				Ogre::Vector3 pos;
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					pos = c3dObject->getPosition();
				
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
			
			void Transform::pitch(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0)
					return;
				
				RDECIMAL value = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					c3dObject->pitch(Ogre::Degree(value));
			}
			
			void Transform::roll(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0)
					return;

				RDECIMAL value = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					c3dObject->roll(Ogre::Degree(value));
			}
			
			void Transform::yaw(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0)
					return;

				RDECIMAL value = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					c3dObject->yaw(Ogre::Degree(value));
			}
			
			void Transform::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				Ogre::Vector3 pos;
				
				if (c3dObject)
					pos = c3dObject->getPosition();
				
				args.GetReturnValue().Set(pos.x);
			}
			
			void Transform::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				Ogre::Vector3 pos;
				
				if (c3dObject)
					pos = c3dObject->getPosition();
				
				args.GetReturnValue().Set(pos.y);
			}
			
			void Transform::getZ(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				Ogre::Vector3 pos;
				
				if (c3dObject)
					pos = c3dObject->getPosition();
				
				args.GetReturnValue().Set(pos.z);
			}
			
			void Transform::setScale(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0)
					return;
				
				v8::Local<v8::Object> scaleArg = v8::Local<v8::Object>::Cast(args[0]);
				
				Ogre::Vector3 scale;
				
				if(args.Length() == 1)
				{
					scale.x = V8_JAVASCRIPT_ENGINE->v8GetDecimal(scaleArg, "x");
					scale.y = V8_JAVASCRIPT_ENGINE->v8GetDecimal(scaleArg, "y");
					scale.z = V8_JAVASCRIPT_ENGINE->v8GetDecimal(scaleArg, "z");
				}
				else if(args.Length() == 3)
				{
					scale.x = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
					scale.y = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[1]);
					scale.z = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[2]);
				}
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					c3dObject->setScale(scale);
			}
			
			void Transform::getScale(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				Ogre::Vector3 scale;
				
				if (c3dObject)
					scale = c3dObject->getScale();
				
				v8::Local<v8::Object> vector3 = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector3");
				v8::Local<v8::Object> vector3Obj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(vector3, 0, NULL);
				vector3Obj->Set(String("x").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), scale.x));
				vector3Obj->Set(String("y").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), scale.y));
				vector3Obj->Set(String("z").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), scale.z));
				
				args.GetReturnValue().Set(vector3Obj);
			}
			
			void Transform::addChild(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0)
					return;
				
				v8::Handle<v8::Object> childObject = args[0]->ToObject();
				std::shared_ptr<C3DTYPE> c3dChildObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(childObject, "_c3dObj");
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					c3dObject->addChild(c3dChildObject.get());
			}
			
			void Transform::removeChild(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				if(args.Length() == 0)
					return;
				
				v8::Handle<v8::Object> childObject = args[0]->ToObject();
				std::shared_ptr<C3DTYPE> c3dChildObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(childObject, "_c3dObj");
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					c3dObject->removeChild(c3dChildObject.get());
			}
			
			void Transform::lookAt(const v8::FunctionCallbackInfo<v8::Value> &args)
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
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				if (c3dObject)
					c3dObject->lookAt(x, y, z);
			}
			
			void Transform::setDirection(const v8::FunctionCallbackInfo<v8::Value> &args)
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
				
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				if (c3dObject)
					c3dObject->setDirection(x, y, z);
			}
#endif
		}
	}
}
#endif

