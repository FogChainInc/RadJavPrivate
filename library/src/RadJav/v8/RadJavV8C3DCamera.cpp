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
				
				V8_CALLBACK(object, "setPosition", Camera::setPosition);
				V8_CALLBACK(object, "getPosition", Camera::getPosition);
				V8_CALLBACK(object, "getX", Camera::getX);
				V8_CALLBACK(object, "getY", Camera::getY);
				V8_CALLBACK(object, "getZ", Camera::getZ);
				V8_CALLBACK(object, "getParent", Camera::getParent);
				V8_CALLBACK(object, "getAppObj", Camera::getAppObj);
				V8_CALLBACK(object, "setVisibility", Camera::setVisibility);
				V8_CALLBACK(object, "getVisibility", Camera::getVisibility);
				V8_CALLBACK(object, "on", Camera::on);

				V8_CALLBACK(object, "setMode", Camera::setMode);
				V8_CALLBACK(object, "getMode", Camera::getMode);
				V8_CALLBACK(object, "setAspectRatio", Camera::setAspectRatio);
				V8_CALLBACK(object, "getAspectRatio", Camera::getAspectRatio);
				V8_CALLBACK(object, "setFOV", Camera::setFOV);
				V8_CALLBACK(object, "getFOV", Camera::getFOV);
				V8_CALLBACK(object, "setNearClipPlane", Camera::setNearClipPlane);
				V8_CALLBACK(object, "getNearClipPlane", Camera::getNearClipPlane);
				V8_CALLBACK(object, "setFarClipPlane", Camera::setFarClipPlane);
				V8_CALLBACK(object, "getFarClipPlane", Camera::getFarClipPlane);
			}

			void Camera::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				int b = 1;
				b+=1;
				b= b/10;
			}
			
			void Camera::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::getZ(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::getAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}

			void Camera::setMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			void Camera::getMode(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			void Camera::setAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			void Camera::getAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			void Camera::setFOV(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				int a = 1;
				a+=1;
				//cout<< a;
			}

			void Camera::getFOV(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			void Camera::setNearClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			void Camera::getNearClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			void Camera::setFarClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			void Camera::getFarClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				
			}
			
			#endif
		}
	}
}
#endif

