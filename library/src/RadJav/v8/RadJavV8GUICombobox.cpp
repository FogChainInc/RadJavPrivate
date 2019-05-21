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
