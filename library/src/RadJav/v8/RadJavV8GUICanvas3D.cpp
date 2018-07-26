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

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#ifdef C3D_USE_OGRE
	#include <OgreResourceManager.h>
	#include <OgreMaterialManager.h>
	#include <OgreMovableObject.h>
	#include <OgreSceneQuery.h>
#endif

#include "cpp/RadJavCPPGUICanvas3D.h"
#include "cpp/RadJavCPPC3DObject3D.h"

#define UITYPE CPP::GUI::Canvas3D

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
#ifdef C3D_USE_OGRE
			void Canvas3D::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Canvas3D::create);
				V8_CALLBACK(object, "setFont", Canvas3D::setFont);
				V8_CALLBACK(object, "getFont", Canvas3D::getFont);
				V8_CALLBACK(object, "setPosition", Canvas3D::setPosition);
				V8_CALLBACK(object, "getPosition", Canvas3D::getPosition);
				V8_CALLBACK(object, "getX", Canvas3D::getX);
				V8_CALLBACK(object, "getY", Canvas3D::getY);
				V8_CALLBACK(object, "setSize", Canvas3D::setSize);
				V8_CALLBACK(object, "getSize", Canvas3D::getSize);
				V8_CALLBACK(object, "getWidth", Canvas3D::getWidth);
				V8_CALLBACK(object, "getHeight", Canvas3D::getHeight);
				V8_CALLBACK(object, "setText", Canvas3D::setText);
				V8_CALLBACK(object, "getText", Canvas3D::getText);
				V8_CALLBACK(object, "getParent", Canvas3D::getParent);
				V8_CALLBACK(object, "setVisibility", Canvas3D::setVisibility);
				V8_CALLBACK(object, "getVisibility", Canvas3D::getVisibility);
				V8_CALLBACK(object, "setEnabled", Canvas3D::setEnabled);
				V8_CALLBACK(object, "getEnabled", Canvas3D::getEnabled);
				V8_CALLBACK(object, "on", Canvas3D::on);

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

			void Canvas3D::setFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				V8_JAVASCRIPT_ENGINE->v8SetObject(args.This(), "_font", v8::Local<v8::Object>::Cast(args[0]));

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
				{
					CPP::Font *font = RJNEW CPP::Font(V8_JAVASCRIPT_ENGINE, v8::Local<v8::Object>::Cast(args[0]));
					CPP::Font *oldfont = appObject->getFont();
					DELETEOBJ(oldfont);

					appObject->setFont(font);
				}
			}

			void Canvas3D::getFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Object> font = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_font");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					font = CPP::Font::toV8Object(V8_JAVASCRIPT_ENGINE, appObject->getFont());

				args.GetReturnValue().Set(font);
			}

			void Canvas3D::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				if (args.Length() > 1)
				{
					x = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[0]);
					y = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[1]);
				}
				else
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[0]);
					x = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x");
					y = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y");
				}

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				v8::Local<v8::Value> *args2 = RJNEW v8::Local<v8::Value>[2];
				args2[0] = v8::Number::New(args.GetIsolate(), x);
				args2[1] = v8::Number::New(args.GetIsolate(), y);
				V8_JAVASCRIPT_ENGINE->v8CallFunction(transform, "setPosition", 2, args2);

				DELETE_ARRAY(args2);

				if (appObject != NULL)
					appObject->setPosition(x, y);
			}

			void Canvas3D::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				x = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "x");
				y = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "y");

				CPP::Vector2 pos;

				if (appObject != NULL)
					pos = appObject->getPosition();

				x = pos.x;
				y = pos.y;

				v8::Local<v8::Object> vector2 = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector2");
				v8::Local<v8::Object> vector2obj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), x));
				vector2obj->Set(String("y").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), y));

				args.GetReturnValue().Set(vector2obj);
			}

			void Canvas3D::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void Canvas3D::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void Canvas3D::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				if (args.Length() > 1)
				{
					x = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[0]);
					y = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[1]);
				}
				else
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[0]);
					x = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x");
					y = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y");
				}

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				v8::Local<v8::Value> *args2 = RJNEW v8::Local<v8::Value>[2];
				args2[0] = v8::Number::New(args.GetIsolate(), x);
				args2[1] = v8::Number::New(args.GetIsolate(), y);
				V8_JAVASCRIPT_ENGINE->v8CallFunction(transform, "setSize", 2, args2);

				DELETE_ARRAY(args2);

				if (appObject != NULL)
					appObject->setSize(x, y);
			}

			void Canvas3D::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				x = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "width");
				y = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "height");

				if (appObject != NULL)
				{
					CPP::Vector2 size = appObject->getSize();
					x = size.x;
					y = size.y;
				}

				v8::Local<v8::Object> vector2 = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector2");
				v8::Local<v8::Object> vector2obj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), x));
				vector2obj->Set(String("y").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), y));

				args.GetReturnValue().Set(vector2obj);
			}

			void Canvas3D::getWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void Canvas3D::getHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void Canvas3D::setText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String str = parseV8Value(val);
				V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "_text", str);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
			}

			void Canvas3D::getText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String text = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "_text");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				args.GetReturnValue().Set(text.toV8String(args.GetIsolate()));
			}

			void Canvas3D::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::Object> obj = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_parent");
				args.GetReturnValue().Set(obj);
			}

			void Canvas3D::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				V8_JAVASCRIPT_ENGINE->v8SetBool(args.This(), "_visible", value);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
			}

			void Canvas3D::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8GetBool(args.This(), "_visible");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Canvas3D::setEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
			}

			void Canvas3D::getEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Canvas3D::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->on(event, func);
			}

			void Canvas3D::setAmbientLight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Color nativeColor;
				
				if(args.Length() > 0)
				{
					v8::Isolate* isolate = args.GetIsolate();
					v8::Handle<v8::Object> color = v8::Handle<v8::Object>::Cast(args[0]);
					
					nativeColor.r = color->Get( String("r").toV8String(isolate))->NumberValue();
					nativeColor.g = color->Get( String("g").toV8String(isolate))->NumberValue();
					nativeColor.b = color->Get( String("b").toV8String(isolate))->NumberValue();
					nativeColor.a = color->Get( String("a").toV8String(isolate))->NumberValue();
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
				
				v8::Local<v8::Object> child = args[0]->ToObject();
				
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
				
				v8::Local<v8::Object> child = args[0]->ToObject();
				
				std::shared_ptr<CPP::C3D::Transform> childNative = V8_JAVASCRIPT_ENGINE->v8GetExternal<CPP::C3D::Transform>(child, "_c3dObj");

				if(childNative)
					appObject->removeChild(*childNative);
			}

			void Canvas3D::createObject3D(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> nameObj;
				v8::Local<v8::Value> thisObj = args.This()->ToObject();
				
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
				v8::Local<v8::Value> thisObj = args.This()->ToObject();
				
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
				v8::Local<v8::Value> thisObj = args.This()->ToObject();
				
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
				v8::Local<v8::Value> thisObj = args.This()->ToObject();
				
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
				v8::Local<v8::Value> thisObj = args.This()->ToObject();
				
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
				v8::Local<v8::Value> thisObj = args.This()->ToObject();
				
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
				v8::Local<v8::Value> thisObj = args.This()->ToObject();
				
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
#endif
		}
	}
}
#endif

