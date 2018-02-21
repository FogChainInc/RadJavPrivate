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
#include "cpp/RadJavCPPMath.h"

#include "RadJav.h"
#include "RadJavString.h"

#include <cmath>

namespace RadJAV
{
	namespace CPP
	{
		const RJNUMBER PI = M_PI;
		const RJNUMBER inv180 = 1/ 180;
		const RJNUMBER invPI = 1/ Math::PI;

		RJNUMBER Math::sqrt(RJNUMBER num)
		{
			return (sqrt (num));
		}

		RJNUMBER Math::pow(RJNUMBER base, RJNUMBER exponent)
		{
			return (pow(base, exponent));
		}

		RJNUMBER Math::abs(RJNUMBER num)
		{
			return (abs(num));
		}

		RJNUMBER Math::cos(RJNUMBER num)
		{
			return (cos(num));
		}

		RJNUMBER Math::acos(RJNUMBER num)
		{
			return (acos(num));
		}

		RJNUMBER Math::cosh(RJNUMBER num)
		{
			return (cosh(num));
		}

		RJNUMBER Math::sin(RJNUMBER num)
		{
			return (sin(num));
		}

		RJNUMBER Math::asin(RJNUMBER num)
		{
			return (asin(num));
		}

		RJNUMBER Math::sinh(RJNUMBER num)
		{
			return (sinh(num));
		}

		RJNUMBER Math::tan(RJNUMBER num)
		{
			return (tan(num));
		}

		RJNUMBER Math::atan(RJNUMBER num)
		{
			return (atan(num));
		}

		RJNUMBER Math::tanh(RJNUMBER num)
		{
			return (tanh(num));
		}

		RJNUMBER Math::ceil(RJNUMBER num)
		{
			return (ceil (num));
		}

		RJNUMBER Math::floor(RJNUMBER num)
		{
			return (floor(num));
		}

		RJNUMBER Math::log(RJNUMBER num)
		{
			return (log(num));
		}

		RJNUMBER Math::log10(RJNUMBER arg)
		{
			// http://kevin.vanzonneveld.net
			// +   original by: Philip Peterson
			// +   improved by: Onno Marsman
			// +   improved by: Tod Gentille
			// +   improved by: Brett Zamir (http://brett-zamir.me)
			return log(arg) / 2.302585092994046; // Math.LN10
		}

		RJNUMBER Math::map(RJNUMBER value, RJNUMBER start1, RJNUMBER stop1, RJNUMBER start2, RJNUMBER stop2)
		{
			return (start2 + (stop2 - start2) * (value - start1) / (stop1 - start1));
		}

		RJNUMBER Math::clamp(RJNUMBER x, RJNUMBER a, RJNUMBER b)
		{
			RJNUMBER dReturn = 0;

			if (x < a)
				dReturn = a;
			else
			{
				if (x > b)
					dReturn = b;
				else
					dReturn = x;
			}

			return (dReturn);
		}

		RJNUMBER Math::degreesToRadians(RJNUMBER degrees)
		{
			return (degrees * (M_PI * (1 / 180)));
		}

		RJNUMBER Math::radiansToDegrees(RJNUMBER radians)
		{
			return (radians * (180 * (1 / M_PI)));
		}

		RJNUMBER Math::random()
		{
			return (rand ());
		}

		RJNUMBER Math::randomRange(RJNUMBER min, RJNUMBER max)
		{
			return ((rand () % (RJINT)max) + min);
		}
	}
}

