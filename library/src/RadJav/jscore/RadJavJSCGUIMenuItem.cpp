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
#include "jscore/RadJavJSCGUIMenuItem.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIMenuItem.h"

#define UITYPE CPP::GUI::MenuItem

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			void MenuItem::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", MenuItem::create);
				JSC_CALLBACK(object, "setFont", MenuItem::setFont);
				JSC_CALLBACK(object, "getFont", MenuItem::getFont);
				JSC_CALLBACK(object, "setPosition", MenuItem::setPosition);
				JSC_CALLBACK(object, "getPosition", MenuItem::getPosition);
				JSC_CALLBACK(object, "getX", MenuItem::getX);
				JSC_CALLBACK(object, "getY", MenuItem::getY);
				JSC_CALLBACK(object, "setSize", MenuItem::setSize);
				JSC_CALLBACK(object, "getSize", MenuItem::getSize);
				JSC_CALLBACK(object, "getWidth", MenuItem::getWidth);
				JSC_CALLBACK(object, "getHeight", MenuItem::getHeight);
				JSC_CALLBACK(object, "setText", MenuItem::setText);
				JSC_CALLBACK(object, "getText", MenuItem::getText);
				JSC_CALLBACK(object, "getParent", MenuItem::getParent);
				JSC_CALLBACK(object, "getAppObj", MenuItem::getAppObj);
				JSC_CALLBACK(object, "setVisibility", MenuItem::setVisibility);
				JSC_CALLBACK(object, "getVisibility", MenuItem::getVisibility);
				JSC_CALLBACK(object, "setEnabled", MenuItem::setEnabled);
				JSC_CALLBACK(object, "getEnabled", MenuItem::getEnabled);
				JSC_CALLBACK(object, "on", MenuItem::on);
				JSC_CALLBACK(object, "destroy", MenuItem::destroy);
			}

			void MenuItem::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = RJNEW UITYPE(JSC_JAVASCRIPT_ENGINE, args);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				return JSValueMake(promise);
			}

			void MenuItem::setFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

			void MenuItem::getFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSObjectRef font = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_font");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
#ifdef GUI_USE_WXWIDGETS
				if (appObject != NULL)
					font = CPP::Font::toJSCObject(JSC_JAVASCRIPT_ENGINE, appObject->getFont());
#endif
				return JSValueMake(font);
			}

			void MenuItem::setPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

			void MenuItem::getPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

			void MenuItem::getX(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			void MenuItem::getY(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getPosition(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y"));
			}

			void MenuItem::setSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

			void MenuItem::getSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

			void MenuItem::getWidth(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			void MenuItem::getHeight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				getSize(args);
				v8::ReturnValue<v8::Value> ret = args.GetReturnValue();
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, ret.Get());

				return JSValueMake(JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y"));
			}

			void MenuItem::setText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(arguments[0]);
				String str = parseV8Value(val);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "_text", str);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
			}

			void MenuItem::getText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				String text = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "_text");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				return JSValueMake(text.toJSCString());
			}

			void MenuItem::getParent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_parent");
				return JSValueMake(obj);
			}

			void MenuItem::getAppObj(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				v8::Handle<v8::External> ext = v8::Handle<v8::External>::Cast(
					thisObject->Get(String("_appObj").toJSCString()));
				return JSValueMake(ext);
			}

			void MenuItem::setVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_visible", value);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
			}

			void MenuItem::getVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_visible");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				return JSValueMake(v8::Boolean::New(args.GetIsolate(), value));
			}

			void MenuItem::setEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
			}

			void MenuItem::getEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				return JSValueMake(v8::Boolean::New(args.GetIsolate(), value));
			}

			void MenuItem::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
			}

			void MenuItem::destroy(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

