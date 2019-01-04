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
#ifndef _RADJAV_GUI_CPP_VECTOR4_H_
#define _RADJAV_GUI_CPP_VECTOR4_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"
#include "RadJavHashMap.h"

#include "cpp/RadJavCPPMath.h"

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
		/**
		 * @ingroup group_basic_cpp
		 * @brief Vector4 class.
		 */
		class RADJAV_EXPORT Vector4
		{
			public:
                #ifdef USE_V8
                    Vector4(v8::Local<v8::Object> obj);

                    v8::Local<v8::Object> toV8Object();
                #endif
                #ifdef USE_JAVASCRIPTCORE
                    Vector4(JSObjectRef obj);

                    JSObjectRef toJSCObject();
                #endif

				inline Vector4(RJNUMBER x = 0, RJNUMBER y = 0, RJNUMBER z = 0, RJNUMBER w = 0)
				{
					this->x = x;
					this->y = y;
					this->z = z;
					this->w = w;
				}

				/** Convert this object to a string.
				* @return {String} The string representing this object.
				*/
				inline String toString()
				{
					return (String::fromInt (x) + "," + String::fromInt (y) + "," + String::fromInt(z) + "," + String::fromInt(w));
				}

				/// The X component.
				RJNUMBER x;
				/// The Y component.
				RJNUMBER y;
				/// The Z component.
				RJNUMBER z;
				/// The W component.
				RJNUMBER w;

				/** Parse a Vector4 string and create a Vector4 object from it.
				* @param {String} str The string to parse.
				* @return {RadJav.Vector4} The new Vector4 created from this string.
				*/
				static Vector4 parseVector4(String str);
		};
	}
}
#endif

