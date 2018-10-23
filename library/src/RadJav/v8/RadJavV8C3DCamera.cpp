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
#include "cpp/RadJavCPPGUICanvas3D.h"

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
				V8_CALLBACK(object, "_init", Camera::init);
				V8_CALLBACK(object, "_setPerspective", Camera::setPerspective);
				V8_CALLBACK(object, "_isPerspective", Camera::isPerspective);
				V8_CALLBACK(object, "_setAspectRatio", Camera::setAspectRatio);
				V8_CALLBACK(object, "_getAspectRatio", Camera::getAspectRatio);
				V8_CALLBACK(object, "_setAutoAspectRatio", Camera::setAutoAspectRatio);
				V8_CALLBACK(object, "_getAutoAspectRatio", Camera::getAutoAspectRatio);
				V8_CALLBACK(object, "_setFOV", Camera::setFOV);
				V8_CALLBACK(object, "_getFOV", Camera::getFOV);
				V8_CALLBACK(object, "_setNearClipPlane", Camera::setNearClipPlane);
				V8_CALLBACK(object, "_getNearClipPlane", Camera::getNearClipPlane);
				V8_CALLBACK(object, "_setFarClipPlane", Camera::setFarClipPlane);
				V8_CALLBACK(object, "_getFarClipPlane", Camera::getFarClipPlane);
				V8_CALLBACK(object, "_setBackgroundColor", Camera::setBackgroundColor);
				V8_CALLBACK(object, "_getBackgroundColor", Camera::getBackgroundColor);
			}

			void Camera::init(const v8::FunctionCallbackInfo<v8::Value> &args)
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
					(CPP::GUI::Canvas3D*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args[0]->ToObject(args.GetIsolate()), "_appObj");
				
				if(!canvas)
					return;

				String name = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "name");
				object.reset(RJNEW C3DTYPE(*canvas, name), [](C3DTYPE* p){DELETEOBJ(p)});
				V8_JAVASCRIPT_ENGINE->v8SetExternal( args.This(), "_c3dObj", object);
			}
			
			void Camera::setPerspective(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL perspective = true;
				
				if (args.Length() > 0)
					perspective = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);

				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					camera->setPerspective(perspective);
			}
			
			void Camera::isPerspective(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					args.GetReturnValue().Set( camera->isPerspective());
				else
					args.GetReturnValue().Set(true);
			}
			
			void Camera::setAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL aspectRatio = 1.0;
				
				if (args.Length() > 0)
					aspectRatio = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					camera->setAspectRatio(aspectRatio);
			}
			
			void Camera::getAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					args.GetReturnValue().Set(camera->getAspectRatio());
				else
					args.GetReturnValue().Set(1.0);
			}
			
			void Camera::setAutoAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL autoAspectRatio = true;
				
				if (args.Length() > 0)
					autoAspectRatio = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					camera->setAutoAspectRatio(autoAspectRatio);
			}
			
			void Camera::getAutoAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					args.GetReturnValue().Set(camera->getAutoAspectRatio());
				else
					args.GetReturnValue().Set(false);
			}

			void Camera::setFOV(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL degree = 0.0;
				
				if (args.Length() > 0)
					degree = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					camera->setFOV( Ogre::Degree(degree));
			}

			void Camera::getFOV(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					args.GetReturnValue().Set( camera->getFOV().valueDegrees());
				else
					args.GetReturnValue().Set(60.0);
			}
			
			void Camera::setNearClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL distance = 1.0;
				
				if (args.Length() > 0)
					distance = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					camera->setNearClipPlane(distance);
			}
			
			void Camera::getNearClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					args.GetReturnValue().Set( camera->getNearClipPlane());
				else
					args.GetReturnValue().Set(1.0);
			}
			
			void Camera::setFarClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RDECIMAL distance = 1.0;
				
				if (args.Length() > 0)
					distance = V8_JAVASCRIPT_ENGINE->v8ParseDecimal(args[0]);
				
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					camera->setFarClipPlane(distance);
			}
			
			void Camera::getFarClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					args.GetReturnValue().Set( camera->getFarClipPlane());
				else
					args.GetReturnValue().Set(100.0);
			}
			
			void Camera::setBackgroundColor(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				Ogre::ColourValue nativeColor;
				
				if(args.Length() > 0)
				{
					v8::Isolate* isolate = args.GetIsolate();
					v8::Handle<v8::Object> color = args[0]->ToObject(args.GetIsolate ());
					
					nativeColor.r = V8_JAVASCRIPT_ENGINE->v8GetDecimal (color, "r");
					nativeColor.g = V8_JAVASCRIPT_ENGINE->v8GetDecimal(color, "g");
					nativeColor.b = V8_JAVASCRIPT_ENGINE->v8GetDecimal(color, "b");
					nativeColor.a = V8_JAVASCRIPT_ENGINE->v8GetDecimal(color, "a");
				}

				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
					camera->setBackgroundColor(nativeColor);
			}
			
			void Camera::getBackgroundColor(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				Ogre::ColourValue nativeColor;
				
				std::shared_ptr<C3DTYPE> camera = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (camera)
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

