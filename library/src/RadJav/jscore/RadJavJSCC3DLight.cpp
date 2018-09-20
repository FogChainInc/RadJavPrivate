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
#include "jscore/RadJavJSCC3DLight.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPC3DLight.h"
#include "cpp/RadJavCPPColor.h"

#define C3DTYPE CPP::C3D::Light

namespace RadJAV
{
	namespace JSC
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			
			Ogre::Light::LightTypes jsLightTypeToNative(RJINT lightType)
			{
				switch(lightType)
				{
					case 1: // Directional
						return Ogre::Light::LT_DIRECTIONAL;
						break;
						
					case 2: // Point
						return Ogre::Light::LT_POINT;
						break;
						
					case 3: // Spot
						return Ogre::Light::LT_SPOTLIGHT;
						break;
						
					default: // Directional
						return Ogre::Light::LT_DIRECTIONAL;
				}
			}
			
			RJINT NativeLightTypeToJs(Ogre::Light::LightTypes lightType)
			{
				switch(lightType)
				{
					case Ogre::Light::LT_DIRECTIONAL: // Directional
						return 1;
						break;
						
					case Ogre::Light::LT_POINT: // Point
						return 2;
						break;
						
					case Ogre::Light::LT_SPOTLIGHT: // Spot
						return 3;
						break;
						
					default:
						return 1; // Directional
				}
			}
			
			void Light::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", Light::init);
				JSC_CALLBACK(object, "_setType", Light::setType);
				JSC_CALLBACK(object, "_getType", Light::getType);
				JSC_CALLBACK(object, "_setColor", Light::setColor);
				JSC_CALLBACK(object, "_getColor", Light::getColor);
			}

			JSValueRef Light::init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				if( argumentCount == 0 ||
				    JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Missing Canvas3D parameter");
					return undefined;
				}

				//Check if we were already contructed
				std::shared_ptr<C3DTYPE> object = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				if(object)
					return undefined;
				
				JSObjectRef canvasObject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
				CPP::GUI::Canvas3D* canvas =
					(CPP::GUI::Canvas3D*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, canvasObject, "_appObj");
				
				if(!canvas)
					return undefined;
				
				String name = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "name");
				object.reset(RJNEW C3DTYPE(*canvas, Ogre::Light::LT_DIRECTIONAL, name), [](C3DTYPE* p){DELETEOBJ(p)});
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_c3dObj", object);
				
				return undefined;
			}

			JSValueRef Light::setType(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJINT type = 1; //directional light
				
				if (argumentCount > 0)
					type = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, arguments[0]);
				
				std::shared_ptr<C3DTYPE> light = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				
				if (light)
					light->setType( jsLightTypeToNative(type));
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Light::getType(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<C3DTYPE> light = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				RJINT type = 1;
				
				if (light)
					type = NativeLightTypeToJs( light->getType());
				
				return JSValueMakeNumber(ctx, type);
			}
			
			JSValueRef Light::setColor(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
				
				std::shared_ptr<C3DTYPE> light = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				
				if (light)
					light->setDiffuseColor(nativeColor);
				
				return JSValueMakeUndefined(ctx);
			}
			
			JSValueRef Light::getColor(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				Ogre::ColourValue nativeColor;
				
				std::shared_ptr<C3DTYPE> light = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				
				if (light)
					nativeColor = light->getDiffuseColor();
				
				JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "Color");
				JSObjectRef colorObject = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(func, 0, NULL);
				
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorObject, "r", nativeColor.r);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorObject, "g", nativeColor.g);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorObject, "b", nativeColor.b);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(colorObject, "a", nativeColor.a);
				
				return colorObject;
			}
#endif
		}
	}
}
#endif
