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
#include "v8/RadJavV8GUITextbox.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPGUITextbox.h"

#define UITYPE CPP::GUI::Textbox

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
			void Textbox::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Textbox::create);
				V8_CALLBACK(object, "setFont", Textbox::setFont);
				V8_CALLBACK(object, "getFont", Textbox::getFont);
				V8_CALLBACK(object, "setPosition", Textbox::setPosition);
				V8_CALLBACK(object, "getPosition", Textbox::getPosition);
				V8_CALLBACK(object, "getX", Textbox::getX);
				V8_CALLBACK(object, "getY", Textbox::getY);
				V8_CALLBACK(object, "setSize", Textbox::setSize);
				V8_CALLBACK(object, "getSize", Textbox::getSize);
				V8_CALLBACK(object, "getWidth", Textbox::getWidth);
				V8_CALLBACK(object, "getHeight", Textbox::getHeight);
				V8_CALLBACK(object, "setText", Textbox::setText);
				V8_CALLBACK(object, "getText", Textbox::getText);
				V8_CALLBACK(object, "getParent", Textbox::getParent);
				V8_CALLBACK(object, "getAppObj", Textbox::getAppObj);
				V8_CALLBACK(object, "setVisibility", Textbox::setVisibility);
				V8_CALLBACK(object, "getVisibility", Textbox::getVisibility);
				V8_CALLBACK(object, "setEnabled", Textbox::setEnabled);
				V8_CALLBACK(object, "getEnabled", Textbox::getEnabled);
				V8_CALLBACK(object, "on", Textbox::on);
				V8_CALLBACK(object, "destroy", Textbox::destroy);
			}

			void Textbox::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = RJNEW UITYPE(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}

			void Textbox::setFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				V8_JAVASCRIPT_ENGINE->v8SetObject(args.This(), "_font", v8::Local<v8::Object>::Cast(args[0]));

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
				{
					CPP::Font *font = RJNEW CPP::Font(V8_JAVASCRIPT_ENGINE, v8::Local<v8::Object>::Cast(args[0]));
					CPP::Font *oldfont = appObject->getFont();
					DELETEOBJ(oldfont);

					appObject->setFont(font);
				}
			}

			void Textbox::getFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Object> font = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_font");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				#ifdef GUI_USE_WXWIDGETS
					if (appObject != NULL)
						font = appObject->getFont()->toV8Object();
				#endif
				args.GetReturnValue().Set(font);
			}

			void Textbox::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
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

			void Textbox::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
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

			void Textbox::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void Textbox::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void Textbox::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
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

			void Textbox::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
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

			void Textbox::getWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void Textbox::getHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void Textbox::setText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String str = parseV8Value(val);
				V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "_text", str);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
			}

			void Textbox::getText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String text = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "_text");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				args.GetReturnValue().Set(text.toV8String(args.GetIsolate()));
			}

			void Textbox::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::Object> obj = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_parent");
				args.GetReturnValue().Set(obj);
			}

			void Textbox::getAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(
					args.This()->Get(String("_appObj").toV8String(args.GetIsolate())));
				args.GetReturnValue().Set(ext);
			}

			void Textbox::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				V8_JAVASCRIPT_ENGINE->v8SetBool(args.This(), "_visible", value);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
			}

			void Textbox::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8GetBool(args.This(), "_visible");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Textbox::setEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
			}

			void Textbox::getEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Textbox::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->on(event, func);
			}

			void Textbox::destroy(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				if (appObject != NULL)
					delete appObject;
				
				V8_JAVASCRIPT_ENGINE->v8ClearExternal(args.This(), "_appObj");
			}
		}
	}
}
#endif

