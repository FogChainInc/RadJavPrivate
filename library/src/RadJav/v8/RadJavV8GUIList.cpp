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
		}
	}
}
