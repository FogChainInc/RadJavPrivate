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
#ifndef _RADJAV_CPP_MATH_H_
#define _RADJAV_CPP_MATH_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"
#include "RadJavHashMap.h"

namespace RadJAV
{
	namespace CPP
	{
		/**
		 * @ingroup group_math_cpp
		 * @brief Math class.
		 * @details Provide Math routines.
		 */
		class RADJAV_EXPORT Math
		{
			public:
				static RJNUMBER sqrt(RJNUMBER num);
				static RJNUMBER pow(RJNUMBER base, RJNUMBER exponent);

				static RJNUMBER abs(RJNUMBER num);

				static RJNUMBER cos(RJNUMBER num);
				static RJNUMBER acos(RJNUMBER num);
				static RJNUMBER cosh(RJNUMBER num);
				static RJNUMBER sin(RJNUMBER num);
				static RJNUMBER asin(RJNUMBER num);
				static RJNUMBER sinh(RJNUMBER num);
				static RJNUMBER tan(RJNUMBER num);
				static RJNUMBER atan(RJNUMBER num);
				static RJNUMBER tanh(RJNUMBER num);

				static RJNUMBER ceil(RJNUMBER num);
				static RJNUMBER floor(RJNUMBER num);

				static RJNUMBER log(RJNUMBER num);
				/// Get a log value.
				static RJNUMBER log10(RJNUMBER arg);

				/// Map a value.
				static RJNUMBER map(RJNUMBER value, RJNUMBER start1, RJNUMBER stop1, RJNUMBER start2, RJNUMBER stop2);

				/// Clamp a value.
				static RJNUMBER clamp(RJNUMBER x, RJNUMBER a, RJNUMBER b);

				/// Convert degrees to radians.
				static RJNUMBER degreesToRadians (RJNUMBER degrees);

				/** Convert radians to degrees.
				* @param {Number} radians The radians value to convert.
				* @return {Number} The degrees.
				*/
				static RJNUMBER radiansToDegrees (RJNUMBER radians);

				/// Get a random number.
				static RJNUMBER random();
				/** Get a random number within a range.
				* @param {Number} min The minimum value.
				* @param {Number} max The maximum value.
				* @return {Number} The random number within the range.
				*/
				static RJNUMBER randomRange (RJNUMBER min, RJNUMBER max);

				static const RJNUMBER PI;
				static const RJNUMBER inv180;
				static const RJNUMBER invPI;
		};
	}
}
#endif

