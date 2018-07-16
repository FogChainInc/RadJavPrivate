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
#include "v8/RadJavV8C3DLight.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPC3DLight.h"
#include "cpp/RadJavCPPColor.h"

#define C3DTYPE CPP::C3D::Light

namespace RadJAV
{
	namespace V8B
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
			
			void Light::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				//V8_CALLBACK(object, "_init", Light::_init);
				
				V8_CALLBACK(object, "setType", Light::setType);
				V8_CALLBACK(object, "getType", Light::getType);
				V8_CALLBACK(object, "setColor", Light::setColor);
				V8_CALLBACK(object, "getColor", Light::getColor);
			}

			void Light::setType(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJINT type = 1; //directional light
				
				if (args.Length() > 0)
					type = v8::Local<v8::Integer>::Cast(args[0])->IntegerValue();
				
				C3DTYPE *light = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				
				light->setType( jsLightTypeToNative(type));
			}
			
			void Light::getType(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *light = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				
				RJINT type = NativeLightTypeToJs( light->getType());
				
				args.GetReturnValue().Set(type);
			}
			
			void Light::setColor(const v8::FunctionCallbackInfo<v8::Value> &args)
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
				
				C3DTYPE *light = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				light->setDiffuseColor(nativeColor);
			}
			
			void Light::getColor(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				Ogre::ColourValue nativeColor;
				
				C3DTYPE *light = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				nativeColor = light->getDiffuseColor();
				
				v8::Handle<v8::Function> func = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "Color");
				v8::Local<v8::Object> v8color = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(func, 0, NULL);
				
				V8_JAVASCRIPT_ENGINE->v8SetNumber(v8color, "r", nativeColor.r);
				V8_JAVASCRIPT_ENGINE->v8SetNumber(v8color, "g", nativeColor.g);
				V8_JAVASCRIPT_ENGINE->v8SetNumber(v8color, "b", nativeColor.b);
				V8_JAVASCRIPT_ENGINE->v8SetNumber(v8color, "a", nativeColor.a);
				
				args.GetReturnValue().Set(v8color);
			}
#endif
		}
	}
}
#endif
