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
#ifndef _RADJAV_CPP_COLOR_H_
#define _RADJAV_CPP_COLOR_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"
#include "RadJavHashMap.h"

namespace RadJAV
{
	namespace CPP
	{
		class RADJAV_EXPORT Color
		{
			public:
				Color(RJNUMBER r = 0, RJNUMBER g = 0, RJNUMBER b = 0, RJNUMBER a = 1);

				RJNUMBER r;
				RJNUMBER g;
				RJNUMBER b;
				RJNUMBER a;

				/** Return this color as a hex string.
				* @return {String} The hex string representing the color.
				*/
				String toHex();

				/** Return this color as a HTML color string.
				* @return {String} The html string representing the color.
				*/
				String toHTMLColor();

				/** Return this color as a hex color integer.
				* @return {Number} The hex integer representing the color.
				*/
				RJINT toHexInt();

				static const Color Black;
				static const Color White;
				static const Color Red;
				static const Color Green;
				static const Color Blue;
		};

		/**
		* Parse a color string.
		* @param {String} str The color string to parse.
		* @return {RadJav.Color} The color.
		*/
		Color parseColor(String str);
	}
}
#endif

