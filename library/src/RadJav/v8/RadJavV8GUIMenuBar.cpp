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
#include "v8/RadJavV8GUIMenuBar.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#include "v8/RadJavV8GUIWindow.h"

#include "cpp/RadJavCPPGUIMenuBar.h"

#define UITYPE CPP::GUI::MenuBar

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
			void MenuBar::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", MenuBar::create);
				V8_CALLBACK(object, "setFont", MenuBar::setFont);
				V8_CALLBACK(object, "getFont", MenuBar::getFont);
				V8_CALLBACK(object, "setPosition", MenuBar::setPosition);
				V8_CALLBACK(object, "getPosition", MenuBar::getPosition);
				V8_CALLBACK(object, "getX", MenuBar::getX);
				V8_CALLBACK(object, "getY", MenuBar::getY);
				V8_CALLBACK(object, "setSize", MenuBar::setSize);
				V8_CALLBACK(object, "getSize", MenuBar::getSize);
				V8_CALLBACK(object, "getWidth", MenuBar::getWidth);
				V8_CALLBACK(object, "getHeight", MenuBar::getHeight);
				V8_CALLBACK(object, "setText", MenuBar::setText);
				V8_CALLBACK(object, "getText", MenuBar::getText);
				V8_CALLBACK(object, "getParent", MenuBar::getParent);
				V8_CALLBACK(object, "getAppObj", MenuBar::getAppObj);
				V8_CALLBACK(object, "setVisibility", MenuBar::setVisibility);
				V8_CALLBACK(object, "getVisibility", MenuBar::getVisibility);
				V8_CALLBACK(object, "setEnabled", MenuBar::setEnabled);
				V8_CALLBACK(object, "getEnabled", MenuBar::getEnabled);
				V8_CALLBACK(object, "on", MenuBar::on);
				V8_CALLBACK(object, "destroy", MenuBar::destroy);
			}

			void MenuBar::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = RJNEW UITYPE(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}

			void MenuBar::setFont(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void MenuBar::getFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Object> font = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_font");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				#ifdef GUI_USE_WXWIDGETS
					if (appObject != NULL)
						font = appObject->getFont()->toV8Object();
				#endif

				args.GetReturnValue().Set(font);
			}

			void MenuBar::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void MenuBar::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void MenuBar::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void MenuBar::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void MenuBar::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void MenuBar::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void MenuBar::getWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void MenuBar::getHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void MenuBar::setText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String str = parseV8Value(val);
				V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "_text", str);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
			}

			void MenuBar::getText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String text = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "_text");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				args.GetReturnValue().Set(text.toV8String(args.GetIsolate()));
			}

			void MenuBar::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::Object> obj = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_parent");
				args.GetReturnValue().Set(obj);
			}

			void MenuBar::getAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(
					args.This()->Get(String("_appObj").toV8String(args.GetIsolate())));
				args.GetReturnValue().Set(ext);
			}

			void MenuBar::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				V8_JAVASCRIPT_ENGINE->v8SetBool(args.This(), "_visible", value);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
			}

			void MenuBar::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8GetBool(args.This(), "_visible");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void MenuBar::setEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
			}

			void MenuBar::getEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void MenuBar::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->on(event, func);
			}

			void MenuBar::destroy(const v8::FunctionCallbackInfo<v8::Value> &args)
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

