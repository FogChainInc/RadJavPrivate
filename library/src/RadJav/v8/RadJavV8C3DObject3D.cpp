/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#include "v8/RadJavV8C3DObject3D.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#include "cpp/RadJavCPPC3DObject3D.h"

#define C3DTYPE CPP::C3D::Object3D

namespace RadJAV
{
	namespace V8B
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			void Object3D::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_init", Object3D::init);
				V8_CALLBACK(object, "_setVisibility", Object3D::setVisibility);
				V8_CALLBACK(object, "_getVisibility", Object3D::getVisibility);
			}

			void Object3D::init(const v8::FunctionCallbackInfo<v8::Value> &args)
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
					(CPP::GUI::Canvas3D*)V8_JAVASCRIPT_ENGINE->v8GetExternal(args[0]->ToObject(), "_appObj");
				
				if(!canvas)
					return;

				String name = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "name");
				object.reset(RJNEW C3DTYPE(*canvas, name), [](C3DTYPE* p){DELETEOBJ(p)});
				V8_JAVASCRIPT_ENGINE->v8SetExternal( args.This(), "_c3dObj", object);
			}

			void Object3D::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL visible = true;
				
				if (args.Length() > 0)
					visible = v8::Local<v8::Boolean>::Cast(args[0])->BooleanValue();

				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					c3dObject->setVisible(visible);
			}

			void Object3D::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<C3DTYPE> c3dObject = V8_JAVASCRIPT_ENGINE->v8GetExternal<C3DTYPE>(args.This(), "_c3dObj");
				
				if (c3dObject)
					args.GetReturnValue().Set(c3dObject->getVisible());
				else
					args.GetReturnValue().Set(false);
			}
#endif
		}
	}
}
#endif

