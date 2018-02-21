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

namespace RadJAV
{
	namespace CPP
	{
		class RADJAV_EXPORT Font
		{
			public:
				Font();
				#ifdef USE_V8
					Font(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> obj);

					#ifdef GUI_USE_WXWIDGETS
						static v8::Local<v8::Object> toV8Object(V8JavascriptEngine *jsEngine, Font *font);
					#endif
				#endif

				/** The font family used.
				*/
				String fontFamily;
				/** The font size.
				*/
				RJNUMBER size;
				/** The font color.
				*/
				CPP::Color color;
				/** Whether or not this font is underlined.
				*/
				RJBOOL underline;
				/** Whether or not this font is bold.
				*/
				RJBOOL bold;
				/** Whether or not this font is italic.
				*/
				RJBOOL italic;
		};
	}
}
#endif

