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

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace GUI
		{
			void GObject::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_createAppObj", GObject::_createAppObj);
			}

			JSValueRef GObject::_createAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::create(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::addChild(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::setFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::setPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getX(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getY(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::setSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getWidth(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getHeight(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::setText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getParent(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::setVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::setEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::getEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}

			JSValueRef GObject::on(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
			{
                return (JSValueMakeUndefined(context));
			}
		}
	}
}
#endif
