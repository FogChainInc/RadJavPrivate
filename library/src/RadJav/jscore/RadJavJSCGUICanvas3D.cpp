/*
	MIT-LICENSE
	Copyright (c) 2018 Higher Edge Software, LLC

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
#include "jscore/RadJavJSCGUICanvas3D.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

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
	namespace JSC
	{
		namespace GUI
		{
#ifdef C3D_USE_OGRE
			void Canvas3D::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", Canvas3D::create);
				JSC_CALLBACK(object, "setFont", Canvas3D::setFont);
				JSC_CALLBACK(object, "getFont", Canvas3D::getFont);
				JSC_CALLBACK(object, "setPosition", Canvas3D::setPosition);
				JSC_CALLBACK(object, "getPosition", Canvas3D::getPosition);
				JSC_CALLBACK(object, "getX", Canvas3D::getX);
				JSC_CALLBACK(object, "getY", Canvas3D::getY);
				JSC_CALLBACK(object, "setSize", Canvas3D::setSize);
				JSC_CALLBACK(object, "getSize", Canvas3D::getSize);
				JSC_CALLBACK(object, "getWidth", Canvas3D::getWidth);
				JSC_CALLBACK(object, "getHeight", Canvas3D::getHeight);
				JSC_CALLBACK(object, "setText", Canvas3D::setText);
				JSC_CALLBACK(object, "getText", Canvas3D::getText);
				JSC_CALLBACK(object, "getParent", Canvas3D::getParent);
				JSC_CALLBACK(object, "setVisibility", Canvas3D::setVisibility);
				JSC_CALLBACK(object, "getVisibility", Canvas3D::getVisibility);
				JSC_CALLBACK(object, "setEnabled", Canvas3D::setEnabled);
				JSC_CALLBACK(object, "getEnabled", Canvas3D::getEnabled);
				JSC_CALLBACK(object, "on", Canvas3D::on);

				JSC_CALLBACK(object, "_setAmbientLightColor", Canvas3D::setAmbientLight);
				JSC_CALLBACK(object, "_getAmbientLightColor", Canvas3D::getAmbientLight);
				JSC_CALLBACK(object, "_addToScene", Canvas3D::addToScene);
				JSC_CALLBACK(object, "_removeFromScene", Canvas3D::removeFromScene);
				JSC_CALLBACK(object, "_createObject3D", Canvas3D::createObject3D);
				JSC_CALLBACK(object, "_createCamera", Canvas3D::createCamera);
				JSC_CALLBACK(object, "_createLight", Canvas3D::createLight);
				JSC_CALLBACK(object, "_createPlane", Canvas3D::createPlane);
				JSC_CALLBACK(object, "_createCube", Canvas3D::createCube);
				JSC_CALLBACK(object, "_createSphere", Canvas3D::createSphere);
				JSC_CALLBACK(object, "_loadModel", Canvas3D::loadModel);
			}

			void Canvas3D::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = RJNEW UITYPE(JSC_JAVASCRIPT_ENGINE, args);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				return JSValueMake(promise);
			}

			void Canvas3D::setFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSC_JAVASCRIPT_ENGINE->jscSetObject(thisObject, "_font", JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]));

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
				{
					CPP::Font *font = RJNEW CPP::Font(JSC_JAVASCRIPT_ENGINE, JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]));
					CPP::Font *oldfont = appObject->getFont();
					DELETEOBJ(oldfont);

					appObject->setFont(font);
				}
			}

			void Canvas3D::getFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSObjectRef font = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_font");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					font = CPP::Font::toJSCObject(JSC_JAVASCRIPT_ENGINE, appObject->getFont());

				return JSValueMake(font);
			}

			void Canvas3D::setPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseInt(arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseInt(arguments[1]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y");
				}

				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				JSValueRef *args2 = RJNEW JSValueRef[2];
				args2[0] = JSValueMakeNumber(args.GetIsolate(), x);
				args2[1] = JSValueMakeNumber(args.GetIsolate(), y);
				JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setPosition", 2, args2);

				DELETE_ARRAY(args2);

				if (appObject != NULL)
					appObject->setPosition(x, y);
			}

			void Canvas3D::getPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				x = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "x");
				y = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "y");

				CPP::Vector2 pos;

				if (appObject != NULL)
					pos = appObject->getPosition();

				x = pos.x;
				y = pos.y;

				JSObjectRef vector2 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector2");
				JSObjectRef vector2obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toJSCString(), JSValueMakeNumber(args.GetIsolate(), x));
				vector2obj->Set(String("y").toJSCString(), JSValueMakeNumber(args.GetIsolate(), y));

				return JSValueMake(vector2obj);
			}

			void Canvas3D::getX(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			void Canvas3D::getY(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y"));
			}

			void Canvas3D::setSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseInt(arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseInt(arguments[1]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y");
				}

				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				JSValueRef *args2 = RJNEW JSValueRef[2];
				args2[0] = JSValueMakeNumber(args.GetIsolate(), x);
				args2[1] = JSValueMakeNumber(args.GetIsolate(), y);
				JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setSize", 2, args2);

				DELETE_ARRAY(args2);

				if (appObject != NULL)
					appObject->setSize(x, y);
			}

			void Canvas3D::getSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				x = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "width");
				y = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "height");

				if (appObject != NULL)
				{
					CPP::Vector2 size = appObject->getSize();
					x = size.x;
					y = size.y;
				}

				JSObjectRef vector2 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector2");
				JSObjectRef vector2obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toJSCString(), JSValueMakeNumber(args.GetIsolate(), x));
				vector2obj->Set(String("y").toJSCString(), JSValueMakeNumber(args.GetIsolate(), y));

				return JSValueMake(vector2obj);
			}

			void Canvas3D::getWidth(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			void Canvas3D::getHeight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y"));
			}

			void Canvas3D::setText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(arguments[0]);
				String str = parseV8Value(val);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "_text", str);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
			}

			void Canvas3D::getText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				String text = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "_text");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				return JSValueMake(text.toJSCString());
			}

			void Canvas3D::getParent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_parent");
				return JSValueMake(obj);
			}

			void Canvas3D::setVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_visible", value);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
			}

			void Canvas3D::getVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_visible");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				return JSValueMake(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Canvas3D::setEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
			}

			void Canvas3D::getEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				return JSValueMake(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Canvas3D::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				String event = parseV8Value(arguments[0]);
				JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[1]);
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->on(event, func);
			}

			void Canvas3D::setAmbientLight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Color nativeColor;
				
				if(argumentCount > 0)
				{
					v8::Isolate* isolate = args.GetIsolate();
					JSObjectRef color = JSObjectRef::Cast(arguments[0]);
					
					nativeColor.r = color->Get( String("r").toJSCString(isolate))->NumberValue();
					nativeColor.g = color->Get( String("g").toJSCString(isolate))->NumberValue();
					nativeColor.b = color->Get( String("b").toJSCString(isolate))->NumberValue();
					nativeColor.a = color->Get( String("a").toJSCString(isolate))->NumberValue();
				}
				
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (appObject)
					appObject->setAmbientLight(nativeColor);
			}
			
			void Canvas3D::getAmbientLight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Color color;
				
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (appObject)
					color = appObject->getAmbientLight();

				v8::Handle<v8::Function> func = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "Color");
				JSObjectRef v8color = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(func, 0, NULL);
				
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(v8color, "r", color.r);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(v8color, "g", color.g);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(v8color, "b", color.b);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(v8color, "a", color.a);
				
				return JSValueMake(v8color);
			}
			
			void Canvas3D::addToScene(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				if(argumentCount == 0 || arguments[0]->IsNullOrUndefined())
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Missing child object");
					return;
				}

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject || argumentCount == 0)
					return;
				
				JSObjectRef child = arguments[0]->ToObject();
				
				std::shared_ptr<CPP::C3D::Transform> childNative = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CPP::C3D::Transform>(child, "_c3dObj");

				if(childNative)
					appObject->addChild(*childNative);
			}
			
			void Canvas3D::removeFromScene(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				if(argumentCount == 0 || arguments[0]->IsNullOrUndefined())
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Missing child object");
					return;
				}

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject || argumentCount == 0)
					return;
				
				JSObjectRef child = arguments[0]->ToObject();
				
				std::shared_ptr<CPP::C3D::Transform> childNative = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CPP::C3D::Transform>(child, "_c3dObj");

				if(childNative)
					appObject->removeChild(*childNative);
			}

			void Canvas3D::createObject3D(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				JSValueRef thisObj = thisObject->ToObject();
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Object3D");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, arguments);
				
				return JSValueMake(newObject);
			}
			
			void Canvas3D::createCamera(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				JSValueRef thisObj = thisObject->ToObject();
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Camera");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, arguments);
				
				return JSValueMake(newObject);
			}
			
			void Canvas3D::createLight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				JSValueRef thisObj = thisObject->ToObject();
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Light");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, arguments);
				
				return JSValueMake(newObject);
			}
			
			void Canvas3D::createPlane(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				JSValueRef thisObj = thisObject->ToObject();
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Plane");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, arguments);
				
				return JSValueMake(newObject);
			}
			
			void Canvas3D::createCube(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				JSValueRef thisObj = thisObject->ToObject();
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Cube");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, arguments);
				
				return JSValueMake(newObject);
			}
			
			void Canvas3D::createSphere(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				JSValueRef thisObj = thisObject->ToObject();
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				
				v8::Handle<v8::Value> arguments[] = { thisObj, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Sphere");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, arguments);
				
				return JSValueMake(newObject);
			}
			
			void Canvas3D::loadModel(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef pathObj;
				JSValueRef nameObj;
				JSValueRef thisObj = thisObject->ToObject();
				
				size_t numOfArgs = argumentCount;
				if(numOfArgs == 0)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("No path specified");
					return;
				}
				
				pathObj = arguments[0];
				
				if(numOfArgs == 1)
					nameObj = v8::String::NewFromUtf8(args.GetIsolate(), "");
				else
					nameObj = arguments[1];
				
				v8::Handle<v8::Value> arguments[] = { thisObj, pathObj, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Model");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 3, arguments);
				
				JSObjectRef loadFunc = JSC_JAVASCRIPT_ENGINE->jscGetObject(newObject, "_load");
				if(loadFunc->IsFunction())
				{
					//TODO: Not sure what to do if it fails, maybe we can add isLoaded method to Model itself
					JSC_JAVASCRIPT_ENGINE->jscCallFunction(newObject, "_load", 0, nullptr);
				}
				
				return JSValueMake(newObject);
			}
#endif
		}
	}
}
#endif

