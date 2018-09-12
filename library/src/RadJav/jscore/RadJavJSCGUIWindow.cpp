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
#include "jscore/RadJavJSCGUIWindow.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIWindow.h"

#define UITYPE CPP::GUI::Window

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			void Window::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
                JSC_CALLBACK(object, "create", Window::create);
                JSC_CALLBACK(object, "setFont", Window::setFont);
                JSC_CALLBACK(object, "getFont", Window::getFont);
                JSC_CALLBACK(object, "setPosition", Window::setPosition);
                JSC_CALLBACK(object, "getPosition", Window::getPosition);
                JSC_CALLBACK(object, "getX", Window::getX);
                JSC_CALLBACK(object, "getY", Window::getY);
                JSC_CALLBACK(object, "setSize", Window::setSize);
                JSC_CALLBACK(object, "getSize", Window::getSize);
                JSC_CALLBACK(object, "getWidth", Window::getWidth);
                JSC_CALLBACK(object, "getHeight", Window::getHeight);
                JSC_CALLBACK(object, "setText", Window::setText);
                JSC_CALLBACK(object, "getText", Window::getText);
                JSC_CALLBACK(object, "getParent", Window::getParent);
                JSC_CALLBACK(object, "getAppObj", Window::getAppObj);
                JSC_CALLBACK(object, "setVisibility", Window::setVisibility);
                JSC_CALLBACK(object, "getVisibility", Window::getVisibility);
                JSC_CALLBACK(object, "setEnabled", Window::setEnabled);
                JSC_CALLBACK(object, "getEnabled", Window::getEnabled);
                JSC_CALLBACK(object, "setIcon", Window::setIcon);
                JSC_CALLBACK(object, "on", Window::on);
                JSC_CALLBACK(object, "destroy", Window::destroy);
			}

			JSValueRef Window::create(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                UITYPE *appObject = RJNEW UITYPE(JSC_JAVASCRIPT_ENGINE, thisObj, numArgs, args);
                appObject->create();
                
                JSC_JAVASCRIPT_ENGINE->jscSetExternal(context, thisObj, "_appObj", appObject);
                JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
                JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObj, _guiFinishedCreatingGObject);
                
                return (promise);
			}

			JSValueRef Window::setFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                JSObjectRef arg0 = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(args[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetObject(thisObj, "_font", arg0);
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                {
                    CPP::Font *font = RJNEW CPP::Font(JSC_JAVASCRIPT_ENGINE, arg0);
                    CPP::Font *oldfont = appObject->getFont();
                    DELETEOBJ(oldfont);
                    
                    appObject->setFont(font);
                }

                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                JSObjectRef font = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObj, "_font");
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    font = CPP::Font::toJSCObject(JSC_JAVASCRIPT_ENGINE, appObject->getFont());

                return (font);
			}

			JSValueRef Window::setPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                RJINT x = 0;
                RJINT y = 0;

                if (numArgs > 1)
                {
                    x = JSC_JAVASCRIPT_ENGINE->jscParseInt (args[0]);
                    y = JSC_JAVASCRIPT_ENGINE->jscParseInt (args[1]);
                }
                else
                {
                    JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(args[0]);
                    x = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x");
                    y = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y");
                }

                JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObj, "_transform");
                JSValueRef *args2 = RJNEW JSValueRef[2];
                args2[0] = JSValueMakeNumber(context, x);
                args2[1] = JSValueMakeNumber(context, y);
                JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setPosition", 2, args2);

                DELETE_ARRAY(args2);

                if (appObject != NULL)
                    appObject->setPosition(x, y);

                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                RJINT x = 0;
                RJINT y = 0;
                
                JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObj, "_transform");
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
                
                return (vector2obj);
			}

			JSValueRef Window::getX(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                JSValueRef pos = getPosition(context, func, thisObj, numArgs, args, exception);
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(pos);

                return (JSValueMakeNumber(context, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x")));
			}

			JSValueRef Window::getY(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                JSValueRef pos = getPosition(context, func, thisObj, numArgs, args, exception);
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(pos);

                return (JSValueMakeNumber(context, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y")));
			}

			JSValueRef Window::setSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                RJINT x = 0;
                RJINT y = 0;
                
                if (numArgs > 1)
                {
                    x = JSC_JAVASCRIPT_ENGINE->jscParseInt (args[0]);
                    y = JSC_JAVASCRIPT_ENGINE->jscParseInt (args[1]);
                }
                else
                {
                    JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(args[0]);
                    x = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x");
                    y = JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y");
                }
                
                JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObj, "_transform");
                JSValueRef *args2 = RJNEW JSValueRef[2];
                args2[0] = JSValueMakeNumber(context, x);
                args2[1] = JSValueMakeNumber(context, y);
                JSC_JAVASCRIPT_ENGINE->jscCallFunction(transform, "setSize", 2, args2);
                
                DELETE_ARRAY(args2);
                
                if (appObject != NULL)
                    appObject->setSize(x, y);
                
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                RJINT x = 0;
                RJINT y = 0;
                
                JSObjectRef transform = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObj, "_transform");
                x = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "width");
                y = JSC_JAVASCRIPT_ENGINE->jscGetInt(transform, "height");
                
                CPP::Vector2 size;
                
                if (appObject != NULL)
                    size = appObject->getSize();
                
                x = size.x;
                y = size.y;
                
                JSObjectRef vector2 = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector2");
                JSObjectRef vector2obj = JSC_JAVASCRIPT_ENGINE->jscCallAsConstructor(vector2, 0, NULL);
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "width", x);
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector2obj, "height", y);
                
                return (vector2obj);
			}

			JSValueRef Window::getWidth(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                JSValueRef size = getSize(context, func, thisObj, numArgs, args, exception);
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(size);
                
                return (JSValueMakeNumber(context, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x")));
			}

			JSValueRef Window::getHeight(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                JSValueRef size = getSize(context, func, thisObj, numArgs, args, exception);
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(size);
                
                return (JSValueMakeNumber(context, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x")));
			}

			JSValueRef Window::setText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                String str = parseJSCValue(context, args[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetString(thisObj, "_text", str);

                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    appObject->setText(str);

                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                String text = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObj, "_text");
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    text = appObject->getText();

                return (text.toJSCValue(context));
			}

			JSValueRef Window::getParent(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObj, "_parent");

                return (obj);
			}

			JSValueRef Window::getAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                /// @todo How do you return only the _appObj object? Not the UITYPE
                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::setVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(args[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObj, "_visible", value);

                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    appObject->setVisibility(value);

                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObj, "_visible");
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    value = appObject->getVisibility();

                return (JSValueMakeBoolean(context, value));
			}

			JSValueRef Window::setEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(args[0]);
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    appObject->setEnabled(value);

                return (JSValueMakeUndefined(context));
			}

			JSValueRef Window::getEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObj, "_visible");
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    value = appObject->getEnabled();

                return (JSValueMakeBoolean(context, value));
			}
            
            JSValueRef Window::setIcon(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                String value = parseJSCValue(context, args[0]);
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                {
                    try
                    {
                        appObject->setIcon(value);
                    }
                    catch (Exception ex)
                    {
                        JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage ());
                    }
                }

                return (JSValueMakeUndefined(context));
            }

			JSValueRef Window::on(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                String event = parseJSCValue(context, args[0]);
                JSObjectRef func2 = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(context, args[1]);
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    appObject->on(event, func2);

                return (JSValueMakeUndefined(context));
			}
            
            JSValueRef Window::destroy(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");

                if (appObject != NULL)
                    DELETEOBJ(appObject);

                JSC_JAVASCRIPT_ENGINE->jscClearExternal(context, thisObj, "_appObj");

                return (JSValueMakeUndefined(context));
            }
		}
	}
}
#endif
