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

#include "jscore/RadJavJSCJavascriptEngine.h"

#include <OgreResourceManager.h>
#include <OgreMaterialManager.h>
#include <OgreMovableObject.h>
#include <OgreSceneQuery.h>

#include "cpp/RadJavCPPGUICanvas3D.h"
#include "cpp/RadJavCPPC3DObject3D.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			using CppGuiObject = CPP::GUI::Canvas3D;
			
			void Canvas3D::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", Canvas3D::create);

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

			JSValueRef Canvas3D::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppGuiObject *appObject = RJNEW CppGuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				return promise;
			}

			JSValueRef Canvas3D::setAmbientLight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Color nativeColor;
				JSValueRef colorValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (colorValue && JSValueIsObject(ctx, colorValue))
				{
					JSObjectRef color = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(colorValue);
					
					nativeColor.r = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(color, "r");
					nativeColor.g = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(color, "g");
					nativeColor.b = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(color, "b");
					nativeColor.a = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(color, "a");
					
					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject)
						appObject->setAmbientLight(nativeColor);
				}
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Canvas3D::getAmbientLight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Color color;
				
				CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (appObject)
					color = appObject->getAmbientLight();

				JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "Color");
				JSObjectRef colorJs = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(func, 0, NULL);
				
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorJs, "r", color.r);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorJs, "g", color.g);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorJs, "b", color.b);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorJs, "a", color.a);
				
				return colorJs;
			}
			
			JSValueRef Canvas3D::addToScene(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef childValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (childValue && JSValueIsObject(ctx, childValue))
				{
					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject)
					{
						JSObjectRef child = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(childValue);
						
						std::shared_ptr<CPP::C3D::Transform> childNative = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CPP::C3D::Transform>(ctx, child, "_c3dObj");
						
						if(childNative)
						{
							appObject->addChild(*childNative);
							return JSValueMakeUndefined(ctx);
						}
					}
				}
				
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Child object required");
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Canvas3D::removeFromScene(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef childValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (childValue && JSValueIsObject(ctx, childValue))
				{
					CppGuiObject *appObject = (CppGuiObject *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
					
					if (appObject)
					{
						JSObjectRef child = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(childValue);
						
						std::shared_ptr<CPP::C3D::Transform> childNative = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CPP::C3D::Transform>(ctx, child, "_c3dObj");
						
						if(childNative)
						{
							appObject->removeChild(*childNative);
							return JSValueMakeUndefined(ctx);
						}
					}
				}

				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Missing child object");
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Canvas3D::createObject3D(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = String("").toJSCValue(ctx);
				
				JSValueRef args[] = { thisObject, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Object3D");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, args);
				
				return newObject;
			}
			
			JSValueRef Canvas3D::createCamera(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = String("").toJSCValue(ctx);
				
				JSValueRef args[] = { thisObject, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Camera");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, args);
				
				return newObject;
			}
			
			JSValueRef Canvas3D::createLight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;

				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = String("").toJSCValue(ctx);
				
				JSValueRef args[] = { thisObject, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Light");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, args);
				
				return newObject;
			}
			
			JSValueRef Canvas3D::createPlane(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = String("").toJSCValue(ctx);
				
				JSValueRef args[] = { thisObject, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Plane");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, args);
				
				return newObject;
			}
			
			JSValueRef Canvas3D::createCube(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = String("").toJSCValue(ctx);
				
				JSValueRef args[] = { thisObject, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Cube");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, args);
				
				return newObject;
			}
			
			JSValueRef Canvas3D::createSphere(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef nameObj;
				
				if(argumentCount)
					nameObj = arguments[0];
				else
					nameObj = String("").toJSCValue(ctx);
				
				JSValueRef args[] = { thisObject, nameObj };
				
				JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
				JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Sphere");
				JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 2, args);
				
				return newObject;
			}
			
			JSValueRef Canvas3D::loadModel(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef pathValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef nameValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);

				if (pathValue && JSValueIsString(ctx, pathValue))
				{
					if (!nameValue)
						nameValue = String("").toJSCValue(ctx);
					
					JSValueRef args[] = { thisObject, pathValue, nameValue };
					
					JSObjectRef C3D = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "C3D");
					JSObjectRef object = JSC_JAVASCRIPT_ENGINE->jscGetObject(C3D, "Model");
					JSObjectRef newObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(object, 3, args);
					
					JSObjectRef loadFunc = JSC_JAVASCRIPT_ENGINE->jscGetObject(newObject, "_load");
					if (loadFunc && JSObjectIsFunction(ctx, loadFunc))
					{
						//TODO: Not sure what to do if it fails, maybe we can add isLoaded method to Model itself
						JSC_JAVASCRIPT_ENGINE->jscCallFunction(newObject, "_load", 0, nullptr);
					}
					
					return newObject;
				}

				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "No path specified");
				return JSValueMakeUndefined(ctx);
			}
		}
	}
}
