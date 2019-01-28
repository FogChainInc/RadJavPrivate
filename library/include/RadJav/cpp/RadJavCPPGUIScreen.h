/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#ifndef _RADJAV_CPP_GUI_SCREEN_H_
	#define _RADJAV_CPP_GUI_SCREEN_H_

	#include "RadJavPreprocessor.h"

	#include "RadJavString.h"
	#include "RadJavHashMap.h"

	#ifdef USE_V8
		#include "v8/RadJavV8JavascriptEngine.h"
	#endif

	#ifdef USE_JAVASCRIPTCORE
		#include "jscore/RadJavJSCJavascriptEngine.h"
	#endif

	namespace RadJAV
	{
		namespace CPP
		{
			namespace GUI
			{
				/**
				 * @ingroup group_gui_cpp
				 * @brief Screen class.
				 */
				class RADJAV_EXPORT Screen
				{
					public:
						Screen(RJINT width = 0, RJINT height = 0, RJNUMBER scale = 1);
						#ifdef USE_V8
							Screen(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> obj);

							static v8::Local<v8::Object> toV8Object(V8JavascriptEngine *jsEngine, Font *font);
						#endif
						#ifdef USE_JAVASCRIPTCORE
							Screen(JSCJavascriptEngine *jsEngine, JSObjectRef obj);

							static JSObjectRef toJSCObject(JSCJavascriptEngine *jsEngine, Font *font);
						#endif

						RJINT getWidth ();
						RJINT getHeight ();
						RJNUMBER getScale ();

						/// The width of the device's screen.
						RJINT width;
						/// The height of the device's screen.
						RJINT height;
						/// The scale of points to pixels on the user's screen.
						RJNUMBER scale;
				};
			}
		}
	}
#endif

