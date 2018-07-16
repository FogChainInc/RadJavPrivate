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
#include "v8/RadJavV8GUIList.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPGUIList.h"

#define UITYPE CPP::GUI::List

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
			void List::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "create", List::create);
				V8_CALLBACK(object, "setFont", List::setFont);
				V8_CALLBACK(object, "getFont", List::getFont);
				V8_CALLBACK(object, "setPosition", List::setPosition);
				V8_CALLBACK(object, "getPosition", List::getPosition);
				V8_CALLBACK(object, "getX", List::getX);
				V8_CALLBACK(object, "getY", List::getY);
				V8_CALLBACK(object, "setSize", List::setSize);
				V8_CALLBACK(object, "getSize", List::getSize);
				V8_CALLBACK(object, "getWidth", List::getWidth);
				V8_CALLBACK(object, "getHeight", List::getHeight);
				V8_CALLBACK(object, "setText", List::setText);
				V8_CALLBACK(object, "getText", List::getText);
				V8_CALLBACK(object, "getParent", List::getParent);
				V8_CALLBACK(object, "getAppObj", List::getAppObj);
				V8_CALLBACK(object, "setVisibility", List::setVisibility);
				V8_CALLBACK(object, "getVisibility", List::getVisibility);
				V8_CALLBACK(object, "setEnabled", List::setEnabled);
				V8_CALLBACK(object, "getEnabled", List::getEnabled);
				V8_CALLBACK(object, "on", List::on);
				V8_CALLBACK(object, "destroy", List::destroy);

				V8_CALLBACK(object, "addColumn", List::addColumn);
				V8_CALLBACK(object, "setColumns", List::setColumns);
				V8_CALLBACK(object, "addRow", List::addRow);
				V8_CALLBACK(object, "setRows", List::setRows);
				V8_CALLBACK(object, "getSelectedRows", List::getSelectedRows);
				V8_CALLBACK(object, "deleteRows", List::deleteRows);
			}

			void List::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				UITYPE *appObject = RJNEW UITYPE(V8_JAVASCRIPT_ENGINE, args);
				appObject->create();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", appObject);
				v8::Local<v8::Function> _guiFinishedCreatingGObject = V8_JAVASCRIPT_ENGINE->v8GetFunction(V8_RADJAV, "_guiFinishedCreatingGObject");
				v8::Local<v8::Object> promise = V8_JAVASCRIPT_ENGINE->createPromise(args.This(), _guiFinishedCreatingGObject);

				args.GetReturnValue().Set(promise);
			}

			void List::setFont(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void List::getFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Object> font = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_font");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					font = CPP::Font::toV8Object(V8_JAVASCRIPT_ENGINE, appObject->getFont());

				args.GetReturnValue().Set(font);
			}

			void List::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void List::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void List::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void List::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void List::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void List::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
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

			void List::getWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "x"));
			}

			void List::getHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(ret.Get());

				args.GetReturnValue().Set(V8_JAVASCRIPT_ENGINE->v8GetInt(obj, "y"));
			}

			void List::setText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String str = parseV8Value(val);
				V8_JAVASCRIPT_ENGINE->v8SetString(args.This(), "_text", str);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
			}

			void List::getText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String text = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "_text");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				args.GetReturnValue().Set(text.toV8String(args.GetIsolate()));
			}

			void List::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::Object> obj = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_parent");
				args.GetReturnValue().Set(obj);
			}

			void List::getAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(
					args.This()->Get(String("_appObj").toV8String(args.GetIsolate())));
				args.GetReturnValue().Set(ext);
			}

			void List::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				V8_JAVASCRIPT_ENGINE->v8SetBool(args.This(), "_visible", value);

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
			}

			void List::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8GetBool(args.This(), "_visible");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void List::setEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = V8_JAVASCRIPT_ENGINE->v8ParseBool(args[0]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
			}

			void List::getEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), value));
			}

			void List::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->on(event, func);
			}

			void List::addColumn(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> value = v8::Local<v8::Value>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				v8::Local<v8::Object> column;

				if (value->IsString() == true)
				{
					column = v8::Object::New(args.GetIsolate());
					column->Set(String("text").toV8String(args.GetIsolate()), v8::Local<v8::String>::Cast(value));

					if (args.Length() > 1)
					{
						v8::Local<v8::Integer> width = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
						column->Set(String("width").toV8String(args.GetIsolate()), width);
					}

					if (args.Length() > 2)
					{
						v8::Local<v8::Integer> key = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
						column->Set(String("key").toV8String(args.GetIsolate()), key);
					}
				}

				if (value->IsObject() == true)
					column = v8::Local<v8::Object>::Cast(value);

				v8::Local<v8::Array> _columns = v8::Local<v8::Array>::Cast(V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_columns"));
				_columns->Set(_columns->Length(), column);
				String text = V8_JAVASCRIPT_ENGINE->v8GetString(column, "text");
				RJINT width = V8_JAVASCRIPT_ENGINE->v8GetInt(column, "width");
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->addColumn(text, width);
			}

			void List::setColumns(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> value = v8::Local<v8::Value>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				V8_JAVASCRIPT_ENGINE->v8SetValue(args.This(), "_columns", value);

				v8::Local<v8::Array> ary = v8::Local<v8::Array>::Cast(value);
				RJUINT length = ary->Length();
				Array<CPP::GUI::List::Column> *columns = RJNEW Array<CPP::GUI::List::Column> ();

				for (RJUINT iIdx = 0; iIdx < length; iIdx++)
				{
					v8::Local<v8::Object> column = v8::Local<v8::Object>::Cast(ary->Get(iIdx));
					String text = V8_JAVASCRIPT_ENGINE->v8GetString(column, "text");
					RJINT width = V8_JAVASCRIPT_ENGINE->v8GetInt(column, "width");
					CPP::GUI::List::Column col(text, width);

					columns->push_back(col);
				}

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
				{
					DELETEOBJ (appObject->_columns);
					appObject->setColumns(columns);
				}
			}

			void List::addRow(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Object> item = v8::Local<v8::Object>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				v8::Local<v8::Array> rowItems = v8::Local<v8::Array>::Cast(item->Get(String("items").toV8String(args.GetIsolate())));
				CPP::GUI::List::Row row;
				RJUINT length = rowItems->Length();

				for (RJUINT iIdx = 0; iIdx < length; iIdx++)
				{
					v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(rowItems->Get(iIdx));
					String str = V8_JAVASCRIPT_ENGINE->v8GetString(obj, "text");

					row.items.push_back(CPP::GUI::List::Item (str));
				}

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->addRow(row);
			}

			void List::setRows(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Array> rows = v8::Local<v8::Array>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				RJUINT length = rows->Length();
				Array<CPP::GUI::List::Row> *appRows = RJNEW Array<CPP::GUI::List::Row> ();

				for (RJUINT iIdx = 0; iIdx < length; iIdx++)
				{
					v8::Local<v8::Object> item = v8::Local<v8::Object>::Cast(rows->Get(iIdx));
					v8::Local<v8::Array> rowItems = v8::Local<v8::Array>::Cast(item->Get(String("items").toV8String(args.GetIsolate())));
					RJUINT length = rowItems->Length();
					CPP::GUI::List::Row row;

					for (RJUINT iJdx = 0; iJdx < length; iJdx++)
					{
						String str = parseV8Value(item->Get(iJdx));

						row.items.push_back(CPP::GUI::List::Item (str));
					}

					appRows->push_back(row);
				}

				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
					appObject->setRows(appRows);

				DELETEOBJ(appRows);
			}

			void List::getSelectedRows(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::GUI::List::Selection appSelection;
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");
				v8::Local<v8::Object> result;

				if (appObject != NULL)
				{
					appSelection = appObject->getSelectedRows();
					result = CPP::GUI::List::Selection::toV8Object (V8_JAVASCRIPT_ENGINE, &appSelection);
				}

				args.GetReturnValue().Set(result);
			}

			void List::deleteRows(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Object> selection = v8::Local<v8::Object>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				UITYPE *appObject = (UITYPE *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_appObj");

				if (appObject != NULL)
				{
					CPP::GUI::List::Selection select = CPP::GUI::List::Selection::fromV8Object(V8_JAVASCRIPT_ENGINE, selection);
					appObject->deleteRows(select);
				}
			}

			void List::destroy(const v8::FunctionCallbackInfo<v8::Value> &args)
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

