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
#ifndef _RADJAV_GUI_JSC_GOBJECT_H_
#define _RADJAV_GUI_JSC_GOBJECT_H_

#include "RadJavPreprocessor.h"
#include <JavaScriptCore/JavaScriptCore.h>

namespace RadJAV
{
	namespace JSC
	{
		/// Contains classes for the OS GUI.
		namespace GUI
		{
			/**
			 *  Root class for GUI anf MUI objects.
			 */
			class RADJAV_EXPORT GObject
			{
			public:
				/** @method createJSCCallbacks
				 * Binds static C++ methods to JS class callbacks. "This" object is resolved manually within methods.
				 * @param context JS engine context
				 * @param object JS class
				 */
				static void createJSCCallbacks(JSContextRef context, JSObjectRef object);
				
				/** @method _createAppObj
				 * //TODO: not sure what to do here
				 * @param context JS engine context
				 * @param func JS function 
				 * @param thisObj reference to this object in JS
				 * @param numArgs number of arguments for function call
				 * @param args arguments
				 * @param exception exception
				 * @return JSValueRef Returns meaningful JS object for get and create methods or JSValueMakeUndefined for others.
				 */
				static JSValueRef _createAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				/** @method create
				 * //TODO: not sure what to do here
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs number of arguments for function call
				 * @param args arguments
				 * @param exception unused
				 * @return JSValueRef Returns meaningful JS object for get and create methods or JSValueMakeUndefined for others.
				 */
				static JSValueRef create(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				/** @method addChild
				 * Makes args[0] child of thisObj
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs number of arguments for function call
				 * @param args arguments
				 * @param exception unused
				 * @return JSValueRef Returns meaningful JS object for get and create methods or JSValueMakeUndefined for others.
				 */
				static JSValueRef addChild(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				/** @method setFont
				 * Makes sets font for objects that support it
				 * @param context JS engine context
				 * @param func unused
				 * @param thisObj reference to this object in JS
				 * @param numArgs number of arguments for function call
				 * @param args JSObjectRef array. arg[0] is expected to be JSObjectRef of font type. Further arguments are ignored. Will likely crash if no arguments supplied.
				 * @param exception unused
				 * @return unused.
				 */
				static JSValueRef setFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method setFont
				 * Makes sets font for objects that support it
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs number of arguments for function call
				 * @param args JSObjectRef array. arg[0] is expected to be JSObjectRef of font type. Further arguments are ignored. Will likely crash if no arguments supplied.
				 * @param exception exception
				 * @return JSValueRef Returns meaningful JS object for get and create methods or JSValueMakeUndefined for others.
				 */
				static JSValueRef getFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef setPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getX(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getY(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef setSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getWidth(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getHeight(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef setText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getParent(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef setVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef setEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef getEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef on(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				static JSValueRef destroy(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
			};
		}
	}
}

#endif
