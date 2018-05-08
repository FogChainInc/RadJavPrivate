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
#ifndef _RADJAV_V8_C3D_CAMERA_H_
	#define _RADJAV_V8_C3D_CAMERA_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "v8/RadJavV8C3DObject3D.h"

	namespace RadJAV
	{
		namespace V8B
		{
			namespace C3D
			{
#ifdef C3D_USE_OGRE
				class RADJAV_EXPORT Camera : public Object3D
				{
				public:
					static void createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object);

					static void setPosition(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getPosition(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getX(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getY(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getZ(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getParent(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getAppObj(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void on(const v8::FunctionCallbackInfo<v8::Value> &args);

					static void setMode(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getMode(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void setAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getAspectRatio(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void setFOV(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getFOV(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void setNearClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getNearClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void setFarClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args);
					static void getFarClipPlane(const v8::FunctionCallbackInfo<v8::Value> &args);
				};
#endif
			}
		}
	}
#endif

