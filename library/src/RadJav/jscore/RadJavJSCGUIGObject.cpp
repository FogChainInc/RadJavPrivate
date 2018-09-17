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
#include "jscore/RadJavJSCGUIGObject.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
            template<class UITYPE>
			void GObject<UITYPE>::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_createAppObj", GObject<UITYPE>::_createAppObj);
			}

            template<class UITYPE>
			JSValueRef GObject<UITYPE>::_createAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

            template<class UITYPE>
			JSValueRef GObject<UITYPE>::create(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

            template<class UITYPE>
			JSValueRef GObject<UITYPE>::addChild(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::setFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
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

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                JSObjectRef font = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObj, "_font");
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    font = CPP::Font::toJSCObject(JSC_JAVASCRIPT_ENGINE, appObject->getFont());
                
                return (font);
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::setPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
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

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
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

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getX(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                JSValueRef pos = getPosition(context, func, thisObj, numArgs, args, exception);
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(pos);
                
                return (JSValueMakeNumber(context, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x")));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getY(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                JSValueRef pos = getPosition(context, func, thisObj, numArgs, args, exception);
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(pos);
                
                return (JSValueMakeNumber(context, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "y")));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::setSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
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

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
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

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getWidth(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                JSValueRef size = getSize(context, func, thisObj, numArgs, args, exception);
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(size);
                
                return (JSValueMakeNumber(context, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x")));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getHeight(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                JSValueRef size = getSize(context, func, thisObj, numArgs, args, exception);
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(size);
                
                return (JSValueMakeNumber(context, JSC_JAVASCRIPT_ENGINE->jscGetInt(obj, "x")));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::setText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                String str = parseJSCValue(context, args[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetString(thisObj, "_text", str);
                
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    appObject->setText(str);
                
                return (JSValueMakeUndefined(context));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                String text = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObj, "_text");
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    text = appObject->getText();
                
                return (text.toJSCValue(context));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getParent(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscGetObject(thisObj, "_parent");
                
                return (obj);
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                /// @todo How do you return only the _appObj object? Not the UITYPE
                return (JSValueMakeUndefined(context));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::setVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(args[0]);
                JSC_JAVASCRIPT_ENGINE->jscSetBool(thisObj, "_visible", value);
                
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    appObject->setVisibility(value);
                
                return (JSValueMakeUndefined(context));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObj, "_visible");
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    value = appObject->getVisibility();
                
                return (JSValueMakeBoolean(context, value));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::setEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscParseBool(args[0]);
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    appObject->setEnabled(value);
                
                return (JSValueMakeUndefined(context));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::getEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                RJBOOL value = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObj, "_visible");
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    value = appObject->getEnabled();
                
                return (JSValueMakeBoolean(context, value));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::on(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
            {
                String event = parseJSCValue(context, args[0]);
                JSObjectRef func2 = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(context, args[1]);
                UITYPE *appObject = (UITYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(context, thisObj, "_appObj");
                
                if (appObject != NULL)
                    appObject->on(event, func2);
                
                return (JSValueMakeUndefined(context));
            }

            template<class UITYPE>
            JSValueRef GObject<UITYPE>::destroy(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
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
