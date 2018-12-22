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
				 * Sets font for objects that support it
				 * @param context JS engine context
				 * @param func unused
				 * @param thisObj reference to this object in JS
				 * @param numArgs number of arguments for function call
				 * @param args JSObjectRef array. arg[0] is expected to be JSObjectRef of font type. Further arguments are ignored. Will likely crash if no arguments supplied.
				 * @param exception unused
				 * @return unused.
				 */
				static JSValueRef setFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getFont
				 * Tries to receive font from underlying C++ object, returns font of JS object if not available.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns font.
				 */
				static JSValueRef getFont(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				/** @method setPosition
				 * Sets position for objects that support it.
				 * @param context JS engine context
				 * @param func unused
				 * @param thisObj reference to this object in JS
				 * @param numArgs Expected to be either 1 or > 2
				 * @param args JSObjectRef array. It's expected to have two values of Int type or structure incapsulating them. Further arguments are ignored.
				 * @param exception unused
				 * @return unused.
				 */
				static JSValueRef setPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				/** @method getPosition
				 * Tries to receive position from underlying C++ object, returns position of JS object if not available.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns position.
				 */
				static JSValueRef getPosition(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				/** @method getX
				 * Wrapper over getPosition.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns x position.
				 */
				static JSValueRef getX(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getY
				 * Wrapper over getPosition.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns y position.
				 */
				static JSValueRef getY(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method setSize
				 * Sets size for objects that support it.
				 * @param context JS engine context
				 * @param func unused
				 * @param thisObj reference to this object in JS
				 * @param numArgs Expected to be either 1 or > 2
				 * @param args JSObjectRef array. It's expected to have two values of Int type or structure incapsulating them. Further arguments are ignored.
				 * @param exception unused
				 * @return unused.
				 */
				static JSValueRef setSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getSize
				 * Tries to receive size from underlying C++ object, returns size of JS object if not available.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns size.
				 */
				static JSValueRef getSize(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getWidth
				 * Wrapper over getSize.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns width.
				 */
				static JSValueRef getWidth(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getHeight
				 * Wrapper over getSize.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns height.
				 */
				static JSValueRef getHeight(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method setText
				 * Sets text for objects that support it.
				 * @param context JS engine context
				 * @param func unused
				 * @param thisObj reference to this object in JS
				 * @param numArgs Expected to be 1
				 * @param args JSObjectRef array. It's expected to have String value.
				 * @param exception unused
				 * @return unused.
				 */
				static JSValueRef setText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getText
				 * Tries to receive size from underlying C++ object, returns poistion of JS object if not available.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns text.
				 */
				static JSValueRef getText(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getParent
				 * Returns parent object.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns parent object.
				 */
				static JSValueRef getParent(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getAppObj
				 * Returns object within application associated to thisObj in JS.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef Returns appObject.
				 */
				static JSValueRef getAppObj(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method setVisibility
				 * Sets visibility.
				 * @param context JS engine context
				 * @param func unused
				 * @param thisObj reference to this object in JS
				 * @param numArgs Expected to be 1
				 * @param args JSObjectRef array. It's expected to have Boolean value.
				 * @param exception unused
				 * @return unused.
				 */
				static JSValueRef setVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getVisibility
				 * Tries to receive visibility from underlying C++ object, returns visibility of JS object if not available.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef ValueRef contains bool visibility.
				 */
				static JSValueRef getVisibility(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method setEnabled
				 * Sets enabled.
				 * @param context JS engine context
				 * @param func unused
				 * @param thisObj reference to this object in JS
				 * @param numArgs Expected to be 1
				 * @param args JSObjectRef array. It's expected to have Boolean value.
				 * @param exception unused
				 * @return unused.
				 */
				static JSValueRef setEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method getEnabled
				 * Tries to receive enabled property from underlying C++ object, returns enabled of JS object if not available.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return JSValueRef ValueRef contains bool enabled.
				 */
				static JSValueRef getEnabled(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method on
				 * Subscribes to event.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs is expected to be 2
				 * @param args arg[0] is expected to be String arg[1] - function reference
				 * @param exception unused
				 * @return unused
				 */
				static JSValueRef on(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
				
				/** @method destroy
				 * Clears data.
				 * @param context JS engine context
				 * @param func JS function
				 * @param thisObj reference to this object in JS
				 * @param numArgs unused
				 * @param args unused
				 * @param exception unused
				 * @return unused
				 */
				static JSValueRef destroy(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception);
			};
		}
	}
}

#endif
