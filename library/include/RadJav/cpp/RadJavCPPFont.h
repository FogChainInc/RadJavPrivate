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
#ifndef _RADJAV_CPP_FONT_H_
#define _RADJAV_CPP_FONT_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"
#include "RadJavHashMap.h"

#include "cpp/RadJavCPPColor.h"

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#endif

#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#ifdef __APPLE__
    #if TARGET_OS_IOS == 1
        #import <UIKit/UIKit.h>
    #endif
#endif

namespace RadJAV
{
	namespace CPP
	{
        /// The font class, pretty simple stuff.
		class RADJAV_EXPORT Font
		{
			public:
                /// Create a default font.
				Font();
                #ifdef GUI_USE_WXWIDGETS
                    /// This is unable to get the color from wxFont.
                    Font (wxFont font);
            
                    /// Convert this font to a wxFont.
                    wxFont towxFont ();
                #endif

                #ifdef __APPLE__
                    #if TARGET_OS_IOS == 1
                        /// This is unable to get color, underline, bold, italic.
                        Font (UIFont *font);

                        /// Convert this font to a UIFont.
                        UIFont *toUIFont ();
                    #endif
                #endif

				#ifdef USE_V8
                    /// Convert a V8 font object to a font.
					Font(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> obj);

                    /// Create a V8 font object from a font.
                    v8::Local<v8::Object> toV8Object();
				#endif
                #ifdef USE_JAVASCRIPTCORE
                    /// Convert a JavaScriptCore font object to a font.
                    Font(JSCJavascriptEngine *jsEngine, JSObjectRef obj);

                    /// Create a JavaScriptCore font object from a font.
                    JSObjectRef toJSCObject();
                #endif

				/// The font family used.
				String fontFamily;
				/// The font size.
				RJNUMBER size;
				/// The font color.
				CPP::Color color;
				/// Whether or not this font is underlined.
				RJBOOL underline;
				/// Whether or not this font is bold.
				RJBOOL bold;
				/// Whether or not this font is italic.
				RJBOOL italic;
		};
	}
}
#endif

