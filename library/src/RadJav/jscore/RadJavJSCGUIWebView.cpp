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
#include "jscore/RadJavJSCGUIWebView.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIWebView.h"

#define UITYPE CPP::GUI::WebView

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			#ifdef WXWIDGETS_HAS_WEBVIEW
			void WebView::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", WebView::create);
				JSC_CALLBACK(object, "setFont", WebView::setFont);
				JSC_CALLBACK(object, "getFont", WebView::getFont);
				JSC_CALLBACK(object, "setPosition", WebView::setPosition);
				JSC_CALLBACK(object, "getPosition", WebView::getPosition);
				JSC_CALLBACK(object, "getX", WebView::getX);
				JSC_CALLBACK(object, "getY", WebView::getY);
				JSC_CALLBACK(object, "setSize", WebView::setSize);
				JSC_CALLBACK(object, "getSize", WebView::getSize);
				JSC_CALLBACK(object, "getWidth", WebView::getWidth);
				JSC_CALLBACK(object, "getHeight", WebView::getHeight);
				JSC_CALLBACK(object, "setText", WebView::setText);
				JSC_CALLBACK(object, "getText", WebView::getText);
				JSC_CALLBACK(object, "getParent", WebView::getParent);
				JSC_CALLBACK(object, "getAppObj", WebView::getAppObj);
				JSC_CALLBACK(object, "setVisibility", WebView::setVisibility);
				JSC_CALLBACK(object, "getVisibility", WebView::getVisibility);
				JSC_CALLBACK(object, "setEnabled", WebView::setEnabled);
				JSC_CALLBACK(object, "getEnabled", WebView::getEnabled);
				JSC_CALLBACK(object, "on", WebView::on);
				JSC_CALLBACK(object, "destroy", WebView::destroy);
			}

			JSValueRef WebView::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = RJNEW UITYPE(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);

				return promise;
			}

			JSValueRef WebView::setFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

			JSValueRef WebView::getFont(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSObjectRef font = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_font");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					font = CPP::Font::toJSCObject(JSC_JAVASCRIPT_ENGINE, appObject->getFont());

				return font;
			}

			JSValueRef WebView::setPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
				JSValueRef args2[2];
				args2[0] = JSValueMakeNumber(ctx, x);
				args2[1] = JSValueMakeNumber(ctx, y);
				JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setPosition", 2, args2);

				if (appObject != NULL)
					appObject->setPosition(x, y);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebView::getPosition(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "x", x);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "y", y);

				return vector2obj;
			}

			JSValueRef WebView::getX(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef pos = getPosition(ctx, function, thisObject, argumentCount, arguments, nullptr);
				
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, pos);
				
				return JSC_JAVASCRIPT_ENGINE->jscGetObject(obj, "x");
			}

			JSValueRef WebView::getY(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef pos = getPosition(ctx, function, thisObject, argumentCount, arguments, nullptr);
				
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, pos);

				return JSC_JAVASCRIPT_ENGINE->jscGetObject(obj, "y");
			}

			JSValueRef WebView::setSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
				JSValueRef args2[2];
				args2[0] = JSValueMakeNumber(ctx, x);
				args2[1] = JSValueMakeNumber(ctx, y);
				JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setSize", 2, args2);

				if (appObject != NULL)
					appObject->setSize(x, y);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebView::getSize(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "x", x);
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "y", y);

				return vector2obj;
			}

			JSValueRef WebView::getWidth(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef size = getSize(ctx, function, thisObject, argumentCount, arguments, nullptr);
				
				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, size);

				return JSC_JAVASCRIPT_ENGINE->jscGetObject(obj, "x");
			}

			JSValueRef WebView::getHeight(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef size = getSize(ctx, function, thisObject, argumentCount, arguments, nullptr);

				JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, size);

				return JSC_JAVASCRIPT_ENGINE->jscGetObject(obj, "y");
			}

			JSValueRef WebView::setText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				String str = parseJSCValue(ctx, arguments[0]);
				JSC_JAVASCRIPT_ENGINE->jscSetString(thisObject, "_text", str);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setText(str);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebView::getText(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				String text = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "_text");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					text = appObject->getText();

				return text.toJSCValue(ctx);
			}

			JSValueRef WebView::getParent(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				return JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_parent");
			}

			JSValueRef WebView::getAppObj(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				return JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObject, "_appObj");
			}

			JSValueRef WebView::setVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObject, "_visible", value);

				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setVisibility(value);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebView::getVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_visible");
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					value = appObject->getVisibility();

				return JSValueMakeBoolean(ctx, value);
			}

			JSValueRef WebView::setEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(arguments[0]);
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->setEnabled(value);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebView::getEnabled(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL value = false;
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					value = appObject->getEnabled();

				return JSValueMakeBoolean(ctx, value);
			}

			JSValueRef WebView::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				String event = parseJSCValue(ctx, arguments[0]);
				JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[1]);
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");

				if (appObject != NULL)
					appObject->on(event, func);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebView::destroy(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				if (appObject != NULL)
					delete appObject;
				
				JSC_JAVASCRIPT_ENGINE->jscClearExternal(ctx, thisObject, "_appObj");
				
				return JSValueMakeUndefined(ctx);
			}
			#endif
		}
	}
}
#endif

