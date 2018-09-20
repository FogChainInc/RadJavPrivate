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
#include "jscore/RadJavJSCC3DTransform.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"
#include "cpp/RadJavCPPC3DTransform.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace C3D
		{
			using CppC3dObject = CPP::C3D::Transform;
			
			void Transform::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", Transform::init);
				JSC_CALLBACK(object, "_setPosition", Transform::setPosition);
				JSC_CALLBACK(object, "_getPosition", Transform::getPosition);
				JSC_CALLBACK(object, "_pitch", Transform::pitch);
				JSC_CALLBACK(object, "_roll", Transform::roll);
				JSC_CALLBACK(object, "_yaw", Transform::yaw);
				JSC_CALLBACK(object, "_getX", Transform::getX);
				JSC_CALLBACK(object, "_getY", Transform::getY);
				JSC_CALLBACK(object, "_getZ", Transform::getZ);
				JSC_CALLBACK(object, "_setScale", Transform::setScale);
				JSC_CALLBACK(object, "_getScale", Transform::getScale);
				JSC_CALLBACK(object, "_addChild", Transform::addChild);
				JSC_CALLBACK(object, "_removeChild", Transform::removeChild);
				JSC_CALLBACK(object, "_lookAt", Transform::lookAt);
				JSC_CALLBACK(object, "_setDirection", Transform::setDirection);
			}
			
			JSValueRef Transform::init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				if(argumentCount == 0 || JSValueIsUndefined(ctx, arguments[0]))
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Missing Canvas3D parameter");
					return undefined;
				}

				//Check if we were already constructed
				std::shared_ptr<CppC3dObject> object = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				if(object)
					return undefined;
				
				CPP::GUI::Canvas3D* canvas =
					(CPP::GUI::Canvas3D*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx,
																			   JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]), "_appObj");
				
				if(!canvas)
					return undefined;
				
				String name = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "name");
				object.reset(RJNEW CppC3dObject(*canvas, name), [](CppC3dObject* p){DELETEOBJ(p)});
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_c3dObj", object);
				
				return undefined;
			}
			
			JSValueRef Transform::setPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				RDECIMAL x = 0;
				RDECIMAL y = 0;
				RDECIMAL z = 0;
				
				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[1]);
					z = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[2]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "y");
					z = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "z");
				}
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				if (c3dObject)
					c3dObject->setPosition(Ogre::Vector3(x, y, z));
				
				return undefined;
			}
			
			JSValueRef Transform::getPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RDECIMAL x = 0;
				RDECIMAL y = 0;
				RDECIMAL z = 0;
				
				Ogre::Vector3 pos;
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (c3dObject)
					pos = c3dObject->getPosition();
				
				x = pos.x;
				y = pos.y;
				z = pos.z;
				
				JSObjectRef vector3 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector3");
				JSObjectRef vector3Obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector3, 0, NULL);

				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector3Obj, "x", x);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector3Obj, "y", y);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector3Obj, "z", z);
				
				return vector3Obj;
			}
			
			JSValueRef Transform::pitch(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				if(argumentCount == 0)
					return undefined;
				
				RDECIMAL value = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (c3dObject)
					c3dObject->pitch(Ogre::Degree(value));
				
				return undefined;
			}
			
			JSValueRef Transform::roll(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				if(argumentCount == 0)
					return undefined;

				RDECIMAL value = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (c3dObject)
					c3dObject->roll(Ogre::Degree(value));
				
				return undefined;
			}
			
			JSValueRef Transform::yaw(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				if(argumentCount == 0)
					return undefined;

				RDECIMAL value = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (c3dObject)
					c3dObject->yaw(Ogre::Degree(value));
				
				return undefined;
			}
			
			JSValueRef Transform::getX(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				Ogre::Vector3 pos;
				
				if (c3dObject)
					pos = c3dObject->getPosition();
				
				return JSValueMakeNumber(ctx, pos.x);
			}
			
			JSValueRef Transform::getY(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				Ogre::Vector3 pos;
				
				if (c3dObject)
					pos = c3dObject->getPosition();
				
				return JSValueMakeNumber(ctx, pos.y);
			}
			
			JSValueRef Transform::getZ(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				Ogre::Vector3 pos;
				
				if (c3dObject)
					pos = c3dObject->getPosition();
				
				return JSValueMakeNumber(ctx, pos.z);
			}
			
			JSValueRef Transform::setScale(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				if(argumentCount == 0)
					return undefined;
				
				JSObjectRef scaleArg = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
				
				Ogre::Vector3 scale;
				
				if(argumentCount == 1)
				{
					scale.x = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(scaleArg, "x");
					scale.y = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(scaleArg, "y");
					scale.z = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(scaleArg, "z");
				}
				else if(argumentCount == 3)
				{
					scale.x = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
					scale.y = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[1]);
					scale.z = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[2]);
				}
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (c3dObject)
					c3dObject->setScale(scale);
				
				return undefined;
			}
			
			JSValueRef Transform::getScale(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				Ogre::Vector3 scale;
				
				if (c3dObject)
					scale = c3dObject->getScale();
				
				JSObjectRef vector3 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector3");
				JSObjectRef vector3Obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector3, 0, NULL);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector3Obj, "x", scale.x);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector3Obj, "y", scale.y);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector3Obj, "z", scale.z);
				
				return vector3Obj;
			}
			
			JSValueRef Transform::addChild(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				if(argumentCount == 0)
					return undefined;
				
				JSObjectRef childObject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
				std::shared_ptr<CppC3dObject> c3dChildObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, childObject, "_c3dObj");
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (c3dObject)
					c3dObject->addChild(c3dChildObject.get());
				
				return undefined;
			}
			
			JSValueRef Transform::removeChild(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				if(argumentCount == 0)
					return undefined;
				
				JSObjectRef childObject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
				std::shared_ptr<CppC3dObject> c3dChildObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, childObject, "_c3dObj");
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (c3dObject)
					c3dObject->removeChild(c3dChildObject.get());
				
				return undefined;
			}
			
			JSValueRef Transform::lookAt(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RDECIMAL x = 0;
				RDECIMAL y = 0;
				RDECIMAL z = 0;
				
				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[1]);
					z = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[2]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "y");
					z = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "z");
				}
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				if (c3dObject)
					c3dObject->lookAt(x, y, z);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Transform::setDirection(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RDECIMAL x = 0;
				RDECIMAL y = 0;
				RDECIMAL z = 0;
				
				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(arguments[1]);
					z = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(arguments[2]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "y");
					z = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(obj, "z");
				}
				
				std::shared_ptr<CppC3dObject> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				if (c3dObject)
					c3dObject->setDirection(x, y, z);
				
				return JSValueMakeUndefined(ctx);
			}
		}
	}
}
