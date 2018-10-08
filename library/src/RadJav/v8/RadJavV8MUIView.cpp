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
#include "v8/RadJavV8MUIView.h"

#include "RadJav.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPMUIView.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::View;
			
			void View::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", View::create);
                V8_CALLBACK(object, "setSize", View::setSize);
                V8_CALLBACK(object, "getSize", View::getSize);
                V8_CALLBACK(object, "setPosition", View::setPosition);
                V8_CALLBACK(object, "getPosition", View::getPosition);
                
                
                //V8_CALLBACK(object, "setParent", View::setParent);
                //V8_CALLBACK(object, "getParent", View::getParent);

                
//                V8_CALLBACK(object, "getX", View::getX);
//                V8_CALLBACK(object, "getY", View::getY);
//
//                V8_CALLBACK(object, "getWidth", View::getWidth);
//                V8_CALLBACK(object, "getHeight", View::getHeight);
               
//                V8_CALLBACK(object, "setFont", View::setFont);
//                V8_CALLBACK(object, "getFont", View::getFont);

//
//                V8_CALLBACK(object, "setText", View::setText);
//                V8_CALLBACK(object, "getText", View::getText);
//                V8_CALLBACK(object, "getParent", View::getParent);
//                V8_CALLBACK(object, "getAppObj", View::getAppObj);
//                V8_CALLBACK(object, "setVisibility", View::setVisibility);
//                V8_CALLBACK(object, "getVisibility", View::getVisibility);
//                V8_CALLBACK(object, "setEnabled", View::setEnabled);
//                V8_CALLBACK(object, "getEnabled", View::getEnabled);
//                V8_CALLBACK(object, "on", View::on);
//                V8_CALLBACK(object, "destroy", View::destroy);

			}

			void View::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);
				
				args.GetReturnValue().Set(promise);
			}
            
            void View::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				if (args.Length() > 1)
				{
					x = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[0]);
					y = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[1]);
				}
				else
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[0]);
					x = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x");
					y = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y");
				}

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				v8::Local<v8::Value> *args2 = RJNEW v8::Local<v8::Value>[2];
				args2[0] = v8::Number::New(args.GetIsolate(), x);
				args2[1] = v8::Number::New(args.GetIsolate(), y);
				V8_JAVASCRIPT_ENGINE->v8CallFunction(transform, "setSize", 2, args2);

				DELETE_ARRAY(args2);

				if (appObject != NULL)
					appObject->setSize(x, y);
            }
            
            
            void View::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				x = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "width");
				y = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "height");

				if (appObject != NULL)
				{
					CPP::Vector2 size = appObject->getSize();
					x = size.x;
					y = size.y;
				}

				v8::Local<v8::Object> vector2 = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector2");
				v8::Local<v8::Object> vector2obj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), x));
				vector2obj->Set(String("y").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), y));

				args.GetReturnValue().Set(vector2obj);
            }
            
            void View::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				if (args.Length() > 1)
				{
					x = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[0]);
					y = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[1]);
				}
				else
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args[0]);
					x = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x");
					y = V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y");
				}

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				v8::Local<v8::Value> *args2 = RJNEW v8::Local<v8::Value>[2];
				args2[0] = v8::Number::New(args.GetIsolate(), x);
				args2[1] = v8::Number::New(args.GetIsolate(), y);
				V8_JAVASCRIPT_ENGINE->v8CallFunction(transform, "setPosition", 2, args2);

				DELETE_ARRAY(args2);

				if (appObject != NULL)
					appObject->setPosition(x, y);
            }
            
            
            void View::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				x = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "x");
				y = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "y");

				CPP::Vector2 pos;

				if (appObject != NULL)
					pos = appObject->getPosition();

				x = pos.x;
				y = pos.y;

				v8::Local<v8::Object> vector2 = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector2");
				v8::Local<v8::Object> vector2obj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), x));
				vector2obj->Set(String("y").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), y));

				args.GetReturnValue().Set(vector2obj);
            }

            void View::setParent(const v8::FunctionCallbackInfo<v8::Value> &args)
            {
                CppMuiObject *appObject = (CppMuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
 
            }
            
		}
	}
}
