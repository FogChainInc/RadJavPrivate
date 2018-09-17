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
#include "jscore/RadJavJSCGUICombobox.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUICombobox.h"

#define UITYPE CPP::GUI::Combobox

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			void Combobox::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", Combobox::create);
				JSC_CALLBACK(object, "setFont", Combobox::setFont);
				JSC_CALLBACK(object, "getFont", Combobox::getFont);
				JSC_CALLBACK(object, "setPosition", Combobox::setPosition);
				JSC_CALLBACK(object, "getPosition", Combobox::getPosition);
				JSC_CALLBACK(object, "getX", Combobox::getX);
				JSC_CALLBACK(object, "getY", Combobox::getY);
				JSC_CALLBACK(object, "setSize", Combobox::setSize);
				JSC_CALLBACK(object, "getSize", Combobox::getSize);
				JSC_CALLBACK(object, "getWidth", Combobox::getWidth);
				JSC_CALLBACK(object, "getHeight", Combobox::getHeight);
				JSC_CALLBACK(object, "setText", Combobox::setText);
				JSC_CALLBACK(object, "getText", Combobox::getText);
				JSC_CALLBACK(object, "getParent", Combobox::getParent);
				JSC_CALLBACK(object, "getAppObj", Combobox::getAppObj);
				JSC_CALLBACK(object, "setVisibility", Combobox::setVisibility);
				JSC_CALLBACK(object, "getVisibility", Combobox::getVisibility);
				JSC_CALLBACK(object, "setEnabled", Combobox::setEnabled);
				JSC_CALLBACK(object, "getEnabled", Combobox::getEnabled);
				JSC_CALLBACK(object, "on", Combobox::on);
				JSC_CALLBACK(object, "destroy", Combobox::destroy);

				JSC_CALLBACK(object, "addItem", Combobox::addItem);
				JSC_CALLBACK(object, "setItems", Combobox::setItems);
				JSC_CALLBACK(object, "deleteItem", Combobox::deleteItem);
				JSC_CALLBACK(object, "getItem", Combobox::getItem);
				JSC_CALLBACK(object, "getItems", Combobox::getItems);
				JSC_CALLBACK(object, "getNumItems", Combobox::getNumItems);
				JSC_CALLBACK(object, "clear", Combobox::clear);
				JSC_CALLBACK(object, "setSelectedItemIndex", Combobox::setSelectedItemIndex);
				JSC_CALLBACK(object, "getSelectedItemIndex", Combobox::getSelectedItemIndex);
			}

			void Combobox::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = RJNEW UITYPE(JSC_JAVASCRIPT_ENGINE, args);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				return JSValueMake(promise);
			}

			void Combobox::setFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSC_JAVASCRIPT_ENGINE->jscSetObject(thisObject, "_font", JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]));

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
				{
					CPP::Font *font = RJNEW CPP::Font(JSC_JAVASCRIPT_ENGINE, JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]));
					CPP::Font *oldfont = appObject->getFont();
					DELETEOBJ(oldfont);

					appObject->setFont(font);
				}
			}

			void Combobox::getFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSObjectRef font = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_font");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
#ifdef GUI_USE_WXWIDGETS
				if (appObject != NULL)
					font = CPP::Font::toJSCObject(JSC_JAVASCRIPT_ENGINE, appObject->getFont());
#endif
				return JSValueMake(font);
			}

			void Combobox::setPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseInt(arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseInt(arguments[1]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y");
				}

				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				JSValueRef *args2 = RJNEW JSValueRef[2];
				args2[0] = JSValueMakeNumber(args.GetIsolate(), x);
				args2[1] = JSValueMakeNumber(args.GetIsolate(), y);
				JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setPosition", 2, args2);

				DELETE_ARRAY(args2);

				if (appObject != NULL)
					appObject->setPosition(x, y);
			}

			void Combobox::getPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				x = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "x");
				y = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "y");

				CPP::Vector2 pos;

				if (appObject != NULL)
					pos = appObject->getPosition();

				x = pos.x;
				y = pos.y;

				JSObjectRef vector2 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector2");
				JSObjectRef vector2obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toJSCString(), JSValueMakeNumber(args.GetIsolate(), x));
				vector2obj->Set(String("y").toJSCString(), JSValueMakeNumber(args.GetIsolate(), y));

				return JSValueMake(vector2obj);
			}

			void Combobox::getX(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			void Combobox::getY(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y"));
			}

			void Combobox::setSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseInt(arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseInt(arguments[1]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y");
				}

				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				JSValueRef *args2 = RJNEW JSValueRef[2];
				args2[0] = JSValueMakeNumber(args.GetIsolate(), x);
				args2[1] = JSValueMakeNumber(args.GetIsolate(), y);
				JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setSize", 2, args2);

				DELETE_ARRAY(args2);

				if (appObject != NULL)
					appObject->setSize(x, y);
			}

			void Combobox::getSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				RJINT x = 0;
				RJINT y = 0;

				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				x = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "width");
				y = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "height");

				if (appObject != NULL)
				{
					CPP::Vector2 size = appObject->getSize();
					x = size.x;
					y = size.y;
				}

				JSObjectRef vector2 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector2");
				JSObjectRef vector2obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector2, 0, NULL);
				vector2obj->Set(String("x").toJSCString(), JSValueMakeNumber(args.GetIsolate(), x));
				vector2obj->Set(String("y").toJSCString(), JSValueMakeNumber(args.GetIsolate(), y));

				return JSValueMake(vector2obj);
			}

			void Combobox::getWidth(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			void Combobox::getHeight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y"));
			}

			void Combobox::setText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(arguments[0]);
				String str = parseV8Value(val);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "_text", str);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
			}

			void Combobox::getText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				String text = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "_text");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				return JSValueMake(text.toJSCString());
			}

			void Combobox::getParent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_parent");
				return JSValueMake(obj);
			}

			void Combobox::getAppObj(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(
					thisObject->Get(String("_appObj").toJSCString()));
				return JSValueMake(ext);
			}

			void Combobox::setVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_visible", value);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
			}

			void Combobox::getVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_visible");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				return JSValueMake(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Combobox::setEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
			}

			void Combobox::getEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				return JSValueMake(v8::Boolean::New(args.GetIsolate(), value));
			}

			void Combobox::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				String event = parseV8Value(arguments[0]);
				JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[1]);
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->on(event, func);
			}

			void Combobox::addItem(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSValueRef::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 0));
				JSObjectRef item;

				if (value->IsString() == true)
				{
					item = v8::Object::New(args.GetIsolate());
					item->Set(String("text").toJSCString(), v8::Local<v8::String>::Cast(value));
				}

				if (value->IsObject() == true)
					item = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, value);

				String text = JSC_JAVASCRIPT_ENGINE->jscGetString(item, "text");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->addItem(text);
			}

			void Combobox::setItems(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSValueRef::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 0));
				v8::Local<v8::Array> ary = v8::Local<v8::Array>::Cast(value);
				RJUINT length = ary->Length();
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
				{
					Array<CPP::GUI::Combobox::Item> *items = RJNEW Array<CPP::GUI::Combobox::Item>();

					for (RJUINT iIdx = 0; iIdx < length; iIdx++)
					{
						JSObjectRef item = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ary->Get(iIdx));
						String text = JSC_JAVASCRIPT_ENGINE->jscGetString(item, "text");
						items->push_back(CPP::GUI::Combobox::Item(text));
					}

					DELETEOBJ(appObject->_items);
					appObject->setItems(items);
				}
			}

			void Combobox::deleteItem(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSValueRef::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 0));
				v8::Local<v8::Integer> iindex = v8::Local<v8::Integer>::Cast(value);
				RJINT index = iindex->Value();

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->deleteItem(index);
			}

			void Combobox::getItem(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSValueRef::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 0));
				v8::Local<v8::Integer> iindex = v8::Local<v8::Integer>::Cast(value);
				RJINT index = iindex->Value();
				JSObjectRef obj = v8::Object::New(JSC_JAVASCRIPT_ENGINE->isolate);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
				{
					CPP::GUI::Combobox::Item item = appObject->getItem(index);

					obj->Set(String("name").toJSCString(JSC_JAVASCRIPT_ENGINE->isolate), item.name.toJSCString(JSC_JAVASCRIPT_ENGINE->isolate));
					obj->Set(String("text").toJSCString(JSC_JAVASCRIPT_ENGINE->isolate), item.text.toJSCString(JSC_JAVASCRIPT_ENGINE->isolate));

					return JSValueMake(obj);
				}
			}

			void Combobox::getItems(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				v8::Local<v8::Array> newary;
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					newary = CPP::GUI::Combobox::Item::toV8Array (JSC_JAVASCRIPT_ENGINE, appObject->getItems());

				return JSValueMake(newary);
			}

			void Combobox::getNumItems(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJUINT size = 0;
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					size = appObject->getNumItems();

				return JSValueMake(v8::Integer::New(args.GetIsolate(), size));
			}

			void Combobox::clear(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->clear();
			}

			void Combobox::setSelectedItemIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef value = JSValueRef::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 0));
				v8::Local<v8::Integer> iindex = v8::Local<v8::Integer>::Cast(value);
				RJINT index = iindex->Value();

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setSelectedItemIndex(index);
			}

			void Combobox::getSelectedItemIndex(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJINT index = 0;
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					index = appObject->getSelectedItemIndex();

				return JSValueMake(v8::Integer::New(args.GetIsolate(), index));
			}

			void Combobox::destroy(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (appObject != NULL)
					delete appObject;
				
				JSC_JAVASCRIPT_ENGINE->jscClearExternal(thisObject, "_appObj");
			}
		}
	}
}
#endif

