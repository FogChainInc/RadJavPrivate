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
#include "v8/RadJavV8C3DCamera.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPC3DCamera.h"
#include "cpp/RadJavCPPColor.h"

#define C3DTYPE CPP::C3D::Camera

namespace RadJAV
{
	namespace V8B
	{
		namespace C3D
		{
			#ifdef C3D_USE_OGRE
			void Camera::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				//V8_CALLBACK(object, "_init", Camera::_init);

				V8_CALLBACK(object, "setMode", Camera::setMode);
				V8_CALLBACK(object, "getMode", Camera::getMode);
				V8_CALLBACK(object, "setAspectRatio", Camera::setAspectRatio);
				V8_CALLBACK(object, "getAspectRatio", Camera::getAspectRatio);
				V8_CALLBACK(object, "setAutoAspectRatio", Camera::setAutoAspectRatio);
				V8_CALLBACK(object, "getAutoAspectRatio", Camera::getAutoAspectRatio);
				V8_CALLBACK(object, "setFOV", Camera::setFOV);
				V8_CALLBACK(object, "getFOV", Camera::getFOV);
				V8_CALLBACK(object, "setNearClipPlane", Camera::setNearClipPlane);
				V8_CALLBACK(object, "getNearClipPlane", Camera::getNearClipPlane);
				V8_CALLBACK(object, "setFarClipPlane", Camera::setFarClipPlane);
				V8_CALLBACK(object, "getFarClipPlane", Camera::getFarClipPlane);
				V8_CALLBACK(object, "setBackgroundColor", Camera::setBackgroundColor);
				V8_CALLBACK(object, "getBackgroundColor", Camera::getBackgroundColor);
			}

			/*
			void Camera::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String name = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "name");
				C3DTYPE* camera = RJNEW C3DTYPE(name, NULL);
				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_c3dObj", camera);
			}
			*/
			
			void Camera::setMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL perspective = true;
				
				if (args.Length() > 0)
					perspective = v8::Local<v8::Boolean>::Cast(args[0])->BooleanValue();

				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				camera->setMode (perspective ? Ogre::PT_PERSPECTIVE : Ogre::PT_ORTHOGRAPHIC);
			}
			
			void Camera::getMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				args.GetReturnValue().Set( camera->getMode() == Ogre::PT_PERSPECTIVE);
			}
			
			void Camera::setAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL aspectRatio = 1.0;
				
				if (args.Length() > 0)
					aspectRatio = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				camera->setAspectRatio(aspectRatio);
			}
			
			void Camera::getAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				args.GetReturnValue().Set(camera->getAspectRatio());
			}
			
			void Camera::setAutoAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL autoAspectRatio = true;
				
				if (args.Length() > 0)
					autoAspectRatio = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				camera->setAutoAspectRatio(autoAspectRatio);
			}
			
			void Camera::getAutoAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				args.GetReturnValue().Set(camera->getAutoAspectRatio());
			}

			void Camera::setFOV(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL degree = 0.0;
				
				if (args.Length() > 0)
					degree = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				
				camera->setFOV( Ogre::Degree(degree));
			}

			void Camera::getFOV(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				args.GetReturnValue().Set( camera->getFOV().valueDegrees());
			}
			
			void Camera::setNearClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL distance = 1.0;
				
				if (args.Length() > 0)
					distance = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				
				camera->setNearClipPlane(distance);
			}
			
			void Camera::getNearClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				args.GetReturnValue().Set( camera->getNearClipPlane());
			}
			
			void Camera::setFarClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL distance = 1.0;
				
				if (args.Length() > 0)
					distance = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				
				camera->setFarClipPlane(distance);
			}
			
			void Camera::getFarClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				args.GetReturnValue().Set( camera->getFarClipPlane());
			}
			
			void Camera::setBackgroundColor(const v8::FunctionCallbackInfo<v8::Value> &args)
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

				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				camera->setBackgroundColor(nativeColor);
			}
			
			void Camera::getBackgroundColor(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				Ogre::ColourValue nativeColor;
				
				C3DTYPE *camera = (C3DTYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_c3dObj");
				nativeColor = camera->getBackgroundColor();

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

