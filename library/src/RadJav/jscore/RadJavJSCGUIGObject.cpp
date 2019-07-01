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
#include "jscore/RadJavJSCGUIGObject.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIGObject.h"
#include "cpp/RadJavCPPFont.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			using CppGuiObject = CPP::GUI::GObject;
			
			void GObject::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_createAppObj", GObject::_createAppObj);
				JSC_CALLBACK(object, "create", GObject::create);
				JSC_CALLBACK(object, "addChild", GObject::addChild);
				JSC_CALLBACK(object, "setFont", GObject::setFont);
				JSC_CALLBACK(object, "getFont", GObject::getFont);
				JSC_CALLBACK(object, "setPosition", GObject::setPosition);
				JSC_CALLBACK(object, "getPosition", GObject::getPosition);
				JSC_CALLBACK(object, "getX", GObject::getX);
				JSC_CALLBACK(object, "getY", GObject::getY);
				JSC_CALLBACK(object, "setSize", GObject::setSize);
				JSC_CALLBACK(object, "getSize", GObject::getSize);
				JSC_CALLBACK(object, "getWidth", GObject::getWidth);
				JSC_CALLBACK(object, "getHeight", GObject::getHeight);
				JSC_CALLBACK(object, "setText", GObject::setText);
				JSC_CALLBACK(object, "getText", GObject::getText);
				JSC_CALLBACK(object, "getParent", GObject::getParent);
				JSC_CALLBACK(object, "getAppObj", GObject::getAppObj);
				JSC_CALLBACK(object, "setVisibility", GObject::setVisibility);
				JSC_CALLBACK(object, "getVisibility", GObject::getVisibility);
				JSC_CALLBACK(object, "setEnabled", GObject::setEnabled);
				JSC_CALLBACK(object, "getEnabled", GObject::getEnabled);
				JSC_CALLBACK(object, "on", GObject::on);
				JSC_CALLBACK(object, "destroy", GObject::destroy);
			}

			JSValueRef GObject::_createAppObj(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				//TODO: not sure what to do here
				return (JSValueMakeUndefined(ctx));
			}

			JSValueRef GObject::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				//TODO: not sure what to do here
				return (JSValueMakeUndefined(ctx));
			}

			JSValueRef GObject::addChild(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				if (argumentCount > 0)
				{
					JSObjectRef argument =  JSValueToObject(ctx, arguments[0], exception);
					CppGuiObject *childAppObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, argument, "_appObj");
					if (!childAppObject)
					{
						JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Valid child object is required");
						return undefined;
					}
					
					appObject->addChild(childAppObject);
				}
				
				return undefined;
			}

			JSValueRef GObject::setFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				JSObjectRef arg0 = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(arguments[0]);
				JSC_JAVASCRIPT_ENGINE->jscSetObject(thisObject, "_font", arg0);

				CPP::Font *font = RJNEW CPP::Font(JSC_JAVASCRIPT_ENGINE, arg0);
				CPP::Font *oldfont = appObject->getFont();
				DELETEOBJ(oldfont);
				
				appObject->setFont(font);

				return undefined;
			}

			JSValueRef GObject::getFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}

				JSObjectRef font = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_font");
				
				font = appObject->getFont ()->toJSCObject ();
				
				return font;
			}

			JSValueRef GObject::setPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				RJINT x = 0;
				RJINT y = 0;
				
				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseInt (arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseInt (arguments[1]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y");
				}
				
				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				JSValueRef args2[2];
				args2[0] = JSValueMakeNumber(ctx, x);
				args2[1] = JSValueMakeNumber(ctx, y);
				JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setPosition", 2, args2);
				
				appObject->setPosition(x, y);
				
				return undefined;
			}

			JSValueRef GObject::getPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				RJINT x = 0;
				RJINT y = 0;
				
				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				x = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "x");
				y = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "y");
				
				CPP::Vector2 pos;
				
				pos = appObject->getPosition();
				
				x = pos.x;
				y = pos.y;
				
				JSObjectRef vector2 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector2");
				JSObjectRef vector2obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector2, 0, NULL);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "x", x);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "y", y);
				
				return vector2obj;
			}

			JSValueRef GObject::getX(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef pos = getPosition(ctx, function, thisObject, argumentCount, arguments, exception);
				
				if (JSValueIsUndefined(ctx, pos))
					return pos;
				
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(pos);
				
				return JSValueMakeNumber(ctx, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			JSValueRef GObject::getY(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef pos = getPosition(ctx, function, thisObject, argumentCount, arguments, exception);

				if (JSValueIsUndefined(ctx, pos))
					return pos;
				
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(pos);
				
				return JSValueMakeNumber(ctx, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y"));
			}

			JSValueRef GObject::setSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				RJINT x = 0;
				RJINT y = 0;
				
				if (argumentCount > 1)
				{
					x = JSC_JAVASCRIPT_ENGINE->jscParseInt (arguments[0]);
					y = JSC_JAVASCRIPT_ENGINE->jscParseInt (arguments[1]);
				}
				else
				{
					JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(arguments[0]);
					x = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x");
					y = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y");
				}
				
				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				JSValueRef args2[2];
				args2[0] = JSValueMakeNumber(ctx, x);
				args2[1] = JSValueMakeNumber(ctx, y);
				JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setSize", 2, args2);
				
				appObject->setSize(x, y);
				
				return undefined;
			}

			JSValueRef GObject::getSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				RJINT x = 0;
				RJINT y = 0;
				
				JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_transform");
				x = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "width");
				y = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "height");
				
				CPP::Vector2 size;
				
				size = appObject->getSize();
				
				x = size.x;
				y = size.y;
				
				JSObjectRef vector2 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector2");
				JSObjectRef vector2obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector2, 0, NULL);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "width", x);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "height", y);
				
				return vector2obj;
			}

			JSValueRef GObject::getWidth(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef size = getSize(ctx, function, thisObject, argumentCount, arguments, exception);

				if (JSValueIsUndefined(ctx, size))
					return size;
				
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(size);
				
				return JSValueMakeNumber(ctx, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			JSValueRef GObject::getHeight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef size = getSize(ctx, function, thisObject, argumentCount, arguments, exception);

				if (JSValueIsUndefined(ctx, size))
					return size;
				
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(size);
				
				return JSValueMakeNumber(ctx, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x"));
			}

			JSValueRef GObject::setText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				String str = parseJSCValue(ctx, arguments[0]);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "_text", str);
				
				appObject->setText(str);
				
				return undefined;
			}

			JSValueRef GObject::getText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				String text = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "_text");
				
				text = appObject->getText();
				
				return text.toJSCValue(ctx);
			}

			JSValueRef GObject::getParent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_parent");
				
				return obj;
			}

			JSValueRef GObject::getAppObj(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				/// @todo How do you return only the _appObj object? Not the CppGuiObject
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_appObj");
				
				return obj;
			}

			JSValueRef GObject::setVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_visible", value);
				
				appObject->setVisibility(value);
				
				return undefined;
			}

			JSValueRef GObject::getVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_visible");
				
				value = appObject->getVisibility();
				
				return JSValueMakeBoolean(ctx, value);
			}

			JSValueRef GObject::setEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);

				appObject->setEnabled(value);
				
				return undefined;
			}

			JSValueRef GObject::getEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_visible");

				value = appObject->getEnabled();
				
				return JSValueMakeBoolean(ctx, value);
			}

			JSValueRef GObject::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				String event = parseJSCValue(ctx, arguments[0]);
				JSObjectRef func2 = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[1]);

				appObject->on(event, func2);
				
				return undefined;
			}
			
			JSValueRef GObject::destroy(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				CppGuiObject *appObject = (CppGuiObject *) JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Object instance was not created");
					return undefined;
				}
				
				DELETEOBJ(appObject);
				
				JSC_JAVASCRIPT_ENGINE->jscClearExternal(ctx, thisObject, "_appObj");
				
				return undefined;
			}
		}
	}
}
