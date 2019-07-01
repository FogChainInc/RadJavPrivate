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
#include "v8/RadJavV8GUIGObject.h"

#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
			using CppGuiObject = CPP::GUI::GObject;
			
			void GObject::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_createAppObj", GObject::_createAppObj);
				V8_CALLBACK(object, "create", GObject::create);
				V8_CALLBACK(object, "_addChild", GObject::addChild);
				V8_CALLBACK(object, "setFont", GObject::setFont);
				V8_CALLBACK(object, "getFont", GObject::getFont);
				V8_CALLBACK(object, "setPosition", GObject::setPosition);
				V8_CALLBACK(object, "getPosition", GObject::getPosition);
				V8_CALLBACK(object, "getX", GObject::getX);
				V8_CALLBACK(object, "getY", GObject::getY);
				V8_CALLBACK(object, "setSize", GObject::setSize);
				V8_CALLBACK(object, "getSize", GObject::getSize);
				V8_CALLBACK(object, "getWidth", GObject::getWidth);
				V8_CALLBACK(object, "getHeight", GObject::getHeight);
				V8_CALLBACK(object, "setText", GObject::setText);
				V8_CALLBACK(object, "getText", GObject::getText);
				V8_CALLBACK(object, "getParent", GObject::getParent);
				V8_CALLBACK(object, "getAppObj", GObject::getAppObj);
				V8_CALLBACK(object, "setVisibility", GObject::setVisibility);
				V8_CALLBACK(object, "getVisibility", GObject::getVisibility);
				V8_CALLBACK(object, "setEnabled", GObject::setEnabled);
				V8_CALLBACK(object, "getEnabled", GObject::getEnabled);
				V8_CALLBACK(object, "on", GObject::on);
				V8_CALLBACK(object, "destroy", GObject::destroy);
			}

			void GObject::_createAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				//TODO: not sure what to do here
			}

			void GObject::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				//TODO: not sure what to do here
			}

			void GObject::addChild(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
                
				if (args.Length())
				{
					v8::Local<v8::Object> childJs = v8::Local<v8::Object>::Cast(args[0]);
					
					CppGuiObject *childAppObject = (CppGuiObject *) V8_JAVASCRIPT_ENGINE->v8GetExternal(childJs, "_appObj");
					if (!childAppObject)
					{
						V8_JAVASCRIPT_ENGINE->throwException("Valid child object is required");
						return;
					}
					
					appObject->addChild(childAppObject);
				}
			}

			void GObject::setFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				V8_JAVASCRIPT_ENGINE->v8SetObject(args.This(), "_font", v8::Local<v8::Object>::Cast(args[0]));

				CPP::Font *font = RJNEW CPP::Font(V8_JAVASCRIPT_ENGINE, v8::Local<v8::Object>::Cast(args[0]));
				CPP::Font *oldfont = appObject->getFont();
				DELETEOBJ(oldfont);
				
				appObject->setFont(font);
			}

			void GObject::getFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				v8::Local<v8::Object> font = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_font");

				#ifdef GUI_USE_WXWIDGETS
					font = appObject->getFont()->toV8Object();
				#endif

				args.GetReturnValue().Set(font);
			}

			void GObject::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
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

				appObject->setPosition(x, y);
			}

			void GObject::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				RJINT x = 0;
				RJINT y = 0;

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				x = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "x");
				y = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "y");

				CPP::Vector2 pos;

				pos = appObject->getPosition();

				x = pos.x;
				y = pos.y;

				v8::Local<v8::Object> vector2 = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector2");
				v8::Local<v8::Object> vector2obj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), x));
				vector2obj->Set(String("y").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), y));

				args.GetReturnValue().Set(vector2obj);
			}

			void GObject::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				
				if(V8_JAVASCRIPT_ENGINE->v8IsNull(ret.Get()))
					return;
				
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void GObject::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				
				if(V8_JAVASCRIPT_ENGINE->v8IsNull(ret.Get()))
					return;
				
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void GObject::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
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

				appObject->setSize(x, y);
			}

			void GObject::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				RJINT x = 0;
				RJINT y = 0;

				v8::Handle<v8::Object> transform = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_transform");
				x = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "width");
				y = V8_JAVASCRIPT_ENGINE->v8GetInt(transform, "height");

				CPP::Vector2 size = appObject->getSize();
				x = size.x;
				y = size.y;

				v8::Local<v8::Object> vector2 = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector2");
				v8::Local<v8::Object> vector2obj = V8_JAVASCRIPT_ENGINE->v8CallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), x));
				vector2obj->Set(String("y").toV8String(args.GetIsolate()), v8::Number::New(args.GetIsolate(), y));

				args.GetReturnValue().Set(vector2obj);
			}

			void GObject::getWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();

				if(V8_JAVASCRIPT_ENGINE->v8IsNull(ret.Get()))
					return;
				
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void GObject::getHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();

				if(V8_JAVASCRIPT_ENGINE->v8IsNull(ret.Get()))
					return;
				
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void GObject::setText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String str = parseV8Value(val);
				V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "_text", str);

				appObject->setText(str);
			}

			void GObject::getText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				String text = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "_text");

				text = appObject->getText();

				args.GetReturnValue().Set(text.toV8String(args.GetIsolate()));
			}

			void GObject::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				v8::Handle<v8::Object> obj = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_parent");
				args.GetReturnValue().Set(obj);
			}

			void GObject::getAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(
					args.This()->Get(String("_appObj").toV8String(args.GetIsolate())));
				args.GetReturnValue().Set(ext);
			}

			void GObject::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				V8_JAVASCRIPT_ENGINE->v8SetBool(args.This(), "_visible", value);

				appObject->setVisibility(value);
			}

			void GObject::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8GetBool(args.This(), "_visible");

				value = appObject->getVisibility();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void GObject::setEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);

				appObject->setEnabled(value);
			}

			void GObject::getEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				RJBOOL value = false;

				value = appObject->getEnabled();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void GObject::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);

				appObject->on(event, func);
			}
			
			void GObject::destroy(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CppGuiObject *appObject = (CppGuiObject *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (!appObject)
				{
					V8_JAVASCRIPT_ENGINE->throwException("Object instance was not created");
					return;
				}
				
				delete appObject;
				
				V8_JAVASCRIPT_ENGINE->v8ClearExternal(args.This(), "_appObj");
			}
		}
	}
}
