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
#include "v8/RadJavV8GUICombobox.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPGUICombobox.h"

#define UITYPE CPP::GUI::Combobox

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
			void Combobox::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", Combobox::create);
				V8_CALLBACK(object, "setFont", Combobox::setFont);
				V8_CALLBACK(object, "getFont", Combobox::getFont);
				V8_CALLBACK(object, "setPosition", Combobox::setPosition);
				V8_CALLBACK(object, "getPosition", Combobox::getPosition);
				V8_CALLBACK(object, "getX", Combobox::getX);
				V8_CALLBACK(object, "getY", Combobox::getY);
				V8_CALLBACK(object, "setSize", Combobox::setSize);
				V8_CALLBACK(object, "getSize", Combobox::getSize);
				V8_CALLBACK(object, "getWidth", Combobox::getWidth);
				V8_CALLBACK(object, "getHeight", Combobox::getHeight);
				V8_CALLBACK(object, "setText", Combobox::setText);
				V8_CALLBACK(object, "getText", Combobox::getText);
				V8_CALLBACK(object, "getParent", Combobox::getParent);
				V8_CALLBACK(object, "getAppObj", Combobox::getAppObj);
				V8_CALLBACK(object, "setVisibility", Combobox::setVisibility);
				V8_CALLBACK(object, "getVisibility", Combobox::getVisibility);
				V8_CALLBACK(object, "setEnabled", Combobox::setEnabled);
				V8_CALLBACK(object, "getEnabled", Combobox::getEnabled);
				V8_CALLBACK(object, "on", Combobox::on);

				V8_CALLBACK(object, "addItem", Combobox::addItem);
				V8_CALLBACK(object, "setItems", Combobox::setItems);
				V8_CALLBACK(object, "deleteItem", Combobox::deleteItem);
				V8_CALLBACK(object, "getItem", Combobox::getItem);
				V8_CALLBACK(object, "getItems", Combobox::getItems);
				V8_CALLBACK(object, "getNumItems", Combobox::getNumItems);
				V8_CALLBACK(object, "clear", Combobox::clear);
				V8_CALLBACK(object, "setSelectedItemIndex", Combobox::setSelectedItemIndex);
				V8_CALLBACK(object, "getSelectedItemIndex", Combobox::getSelectedItemIndex);
			}

			void Combobox::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = RJNEW UITYPE(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}

			void Combobox::setFont(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void Combobox::getFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Object> font = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_font");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					font = CPP::Font::toV8Object(V8_JAVASCRIPT_ENGINE, appObject->getFont());

				args.GetReturnValue().Set(font);
			}

			void Combobox::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void Combobox::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void Combobox::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void Combobox::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void Combobox::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void Combobox::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void Combobox::getWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void Combobox::getHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void Combobox::setText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String str = parseV8Value(val);
				V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "_text", str);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
			}

			void Combobox::getText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String text = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "_text");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				args.GetReturnValue().Set(text.toV8String(args.GetIsolate()));
			}

			void Combobox::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::Object> obj = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_parent");
				args.GetReturnValue().Set(obj);
			}

			void Combobox::getAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(
					args.This()->Get(String("_appObj").toV8String(args.GetIsolate())));
				args.GetReturnValue().Set(ext);
			}

			void Combobox::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				V8_JAVASCRIPT_ENGINE->v8SetBool(args.This(), "_visible", value);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
			}

			void Combobox::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8GetBool(args.This(), "_visible");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Combobox::setEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
			}

			void Combobox::getEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Combobox::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->on(event, func);
			}

			void Combobox::addItem(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> value = v8::Local<v8::Value>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				v8::Local<v8::Object> item;

				if (value->IsString() == true)
				{
					item = v8::Object::New(args.GetIsolate());
					item->Set(String("text").toV8String(args.GetIsolate()), v8::Local<v8::String>::Cast(value));
				}

				if (value->IsObject() == true)
					item = v8::Local<v8::Object>::Cast(value);

				String text = V8_JAVASCRIPT_ENGINE->v8GetString(item, "text");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->addItem(text);
			}

			void Combobox::setItems(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> value = v8::Local<v8::Value>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				v8::Local<v8::Array> ary = v8::Local<v8::Array>::Cast(value);
				RJUINT length = ary->Length();
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
				{
					Array<CPP::GUI::Combobox::Item> *items = RJNEW Array<CPP::GUI::Combobox::Item>();

					for (RJUINT iIdx = 0; iIdx < length; iIdx++)
					{
						v8::Local<v8::Object> item = v8::Local<v8::Object>::Cast(ary->Get(iIdx));
						String text = V8_JAVASCRIPT_ENGINE->v8GetString(item, "text");
						items->push_back(CPP::GUI::Combobox::Item(text));
					}

					DELETEOBJ(appObject->_items);
					appObject->setItems(items);
				}
			}

			void Combobox::deleteItem(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> value = v8::Local<v8::Value>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				v8::Local<v8::Integer> iindex = v8::Local<v8::Integer>::Cast(value);
				RJINT index = iindex->Value();

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->deleteItem(index);
			}

			void Combobox::getItem(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> value = v8::Local<v8::Value>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				v8::Local<v8::Integer> iindex = v8::Local<v8::Integer>::Cast(value);
				RJINT index = iindex->Value();
				v8::Local<v8::Object> obj = v8::Object::New(V8_JAVASCRIPT_ENGINE->isolate);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
				{
					CPP::GUI::Combobox::Item item = appObject->getItem(index);

					obj->Set(String("name").toV8String(V8_JAVASCRIPT_ENGINE->isolate), item.name.toV8String(V8_JAVASCRIPT_ENGINE->isolate));
					obj->Set(String("text").toV8String(V8_JAVASCRIPT_ENGINE->isolate), item.text.toV8String(V8_JAVASCRIPT_ENGINE->isolate));

					args.GetReturnValue().Set(obj);
				}
			}

			void Combobox::getItems(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Array> newary;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					newary = CPP::GUI::Combobox::Item::toV8Array (V8_JAVASCRIPT_ENGINE, appObject->getItems());

				args.GetReturnValue().Set(newary);
			}

			void Combobox::getNumItems(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJUINT size = 0;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					size = appObject->getNumItems();

				args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), size));
			}

			void Combobox::clear(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->clear();
			}

			void Combobox::setSelectedItemIndex(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> value = v8::Local<v8::Value>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				v8::Local<v8::Integer> iindex = v8::Local<v8::Integer>::Cast(value);
				RJINT index = iindex->Value();

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setSelectedItemIndex(index);
			}

			void Combobox::getSelectedItemIndex(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJINT index = 0;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					index = appObject->getSelectedItemIndex();

				args.GetReturnValue().Set(v8::Integer::New(args.GetIsolate(), index));
			}
		}
	}
}
#endif

