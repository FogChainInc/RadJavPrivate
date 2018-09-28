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
#include "jscore/RadJavJSCC3DCamera.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPC3DCamera.h"
#include "cpp/RadJavCPPColor.h"
#include "cpp/RadJavCPPGUICanvas3D.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace C3D
		{
			using CppC3dObject = CPP::C3D::Camera;
			
			void Camera::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", Camera::init);
				JSC_CALLBACK(object, "_setPerspective", Camera::setPerspective);
				JSC_CALLBACK(object, "_isPerspective", Camera::isPerspective);
				JSC_CALLBACK(object, "_setAspectRatio", Camera::setAspectRatio);
				JSC_CALLBACK(object, "_getAspectRatio", Camera::getAspectRatio);
				JSC_CALLBACK(object, "_setAutoAspectRatio", Camera::setAutoAspectRatio);
				JSC_CALLBACK(object, "_getAutoAspectRatio", Camera::getAutoAspectRatio);
				JSC_CALLBACK(object, "_setFOV", Camera::setFOV);
				JSC_CALLBACK(object, "_getFOV", Camera::getFOV);
				JSC_CALLBACK(object, "_setNearClipPlane", Camera::setNearClipPlane);
				JSC_CALLBACK(object, "_getNearClipPlane", Camera::getNearClipPlane);
				JSC_CALLBACK(object, "_setFarClipPlane", Camera::setFarClipPlane);
				JSC_CALLBACK(object, "_getFarClipPlane", Camera::getFarClipPlane);
				JSC_CALLBACK(object, "_setBackgroundColor", Camera::setBackgroundColor);
				JSC_CALLBACK(object, "_getBackgroundColor", Camera::getBackgroundColor);
			}

			JSValueRef Camera::init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				if( argumentCount == 0 ||
				    JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Missing Canvas3D parameter");
					return undefined;
				}

				//Check if we were already contructed
				std::shared_ptr<CppC3dObject> object = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				if(object)
					return undefined;
				
				JSObjectRef canvasObject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
				CPP::GUI::Canvas3D* canvas =
					(CPP::GUI::Canvas3D*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, canvasObject, "_appObj");
				
				if(!canvas)
					return undefined;

				String name = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "name");
				object.reset(RJNEW CppC3dObject(*canvas, name), [](CppC3dObject* p){DELETEOBJ(p)});
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_c3dObj", object);
				
				return undefined;
			}
			
			JSValueRef Camera::setPerspective(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL perspective = true;
				
				if (argumentCount > 0)
					perspective = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(arguments[0]);

				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (camera)
					camera->setPerspective(perspective);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Camera::isPerspective(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				return JSValueMakeBoolean(ctx, camera? camera->isPerspective() : true);
			}
			
			JSValueRef Camera::setAspectRatio(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RDECIMAL aspectRatio = 1.0;
				
				if (argumentCount > 0)
					aspectRatio = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
				
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (camera)
					camera->setAspectRatio(aspectRatio);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Camera::getAspectRatio(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				return JSValueMakeNumber(ctx, camera? camera->getAspectRatio(): 1.0);
			}
			
			JSValueRef Camera::setAutoAspectRatio(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL autoAspectRatio = true;
				
				if (argumentCount > 0)
					autoAspectRatio = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(ctx, arguments[0]);
				
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (camera)
					camera->setAutoAspectRatio(autoAspectRatio);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Camera::getAutoAspectRatio(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				return JSValueMakeBoolean(ctx, camera? camera->getAutoAspectRatio() : false);
			}

			JSValueRef Camera::setFOV(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RDECIMAL degree = 0.0;
				
				if (argumentCount > 0)
					degree = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
				
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (camera)
					camera->setFOV( Ogre::Degree(degree));
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Camera::getFOV(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				return JSValueMakeNumber(ctx, camera? camera->getFOV().valueDegrees() : 60.0);
			}
			
			JSValueRef Camera::setNearClipPlane(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RDECIMAL distance = 1.0;
				
				if (argumentCount > 0)
					distance = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
				
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (camera)
					camera->setNearClipPlane(distance);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Camera::getNearClipPlane(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				return JSValueMakeNumber(ctx, camera? camera->getNearClipPlane() : 1.0);
			}
			
			JSValueRef Camera::setFarClipPlane(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RDECIMAL distance = 1.0;
				
				if (argumentCount > 0)
					distance = JSC_JAVASCRIPT_ENGINE->jscParseDecimal(ctx, arguments[0]);
				
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (camera)
					camera->setFarClipPlane(distance);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Camera::getFarClipPlane(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				return JSValueMakeNumber(ctx, camera? camera->getFarClipPlane() : 100.0);
			}
			
			JSValueRef Camera::setBackgroundColor(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				Ogre::ColourValue nativeColor;
				
				if(argumentCount > 0)
				{
					JSObjectRef color = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);

					nativeColor.r = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(color, "r");
					nativeColor.g = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(color, "g");
					nativeColor.b = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(color, "b");
					nativeColor.a = JSC_JAVASCRIPT_ENGINE->jscGetDecimal(color, "a");
				}

				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (camera)
					camera->setBackgroundColor(nativeColor);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Camera::getBackgroundColor(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				Ogre::ColourValue nativeColor;
				
				std::shared_ptr<CppC3dObject> camera = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppC3dObject>(ctx, thisObject, "_c3dObj");
				
				if (camera)
					nativeColor = camera->getBackgroundColor();

				JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "Color");
				JSObjectRef colorObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(func, 0, NULL);
				
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorObject, "r", nativeColor.r);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorObject, "g", nativeColor.g);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorObject, "b", nativeColor.b);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorObject, "a", nativeColor.a);
				
				return colorObject;
			}
		}
	}
}
