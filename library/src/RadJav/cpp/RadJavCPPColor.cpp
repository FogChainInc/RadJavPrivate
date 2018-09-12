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
#include "cpp/RadJavCPPColor.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		const Color Color::Black (0, 0, 0, 1);
		const Color Color::White (1, 1, 1, 1);
		const Color Color::Red (1, 0, 0, 1);
		const Color Color::Green (0, 1, 0, 1);
		const Color Color::Blue (0, 0, 1, 1);

		Color::Color(RJNUMBER r, RJNUMBER g, RJNUMBER b, RJNUMBER a)
		{
			this->r = r;
			this->g = g;
			this->b = b;
			this->a = a;
		}

        #ifdef USE_V8
            Color::Color(v8::Local<v8::Object> obj)
            {
                r = V8_JAVASCRIPT_ENGINE->v8GetDecimal (obj, "r");
                g = V8_JAVASCRIPT_ENGINE->v8GetDecimal (obj, "g");
                b = V8_JAVASCRIPT_ENGINE->v8GetDecimal (obj, "b");
                a = V8_JAVASCRIPT_ENGINE->v8GetDecimal (obj, "a");
            }
        
            v8::Local<v8::Object> Color::toV8Object()
            {
                v8::Local<v8::Object> vector = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector4");
                
                V8_JAVASCRIPT_ENGINE->v8SetNumber(vector, "r", r);
                V8_JAVASCRIPT_ENGINE->v8SetNumber(vector, "g", g);
                V8_JAVASCRIPT_ENGINE->v8SetNumber(vector, "b", b);
                V8_JAVASCRIPT_ENGINE->v8SetNumber(vector, "a", a);
                
                return (vector);
            }
        #endif
        #ifdef USE_JAVASCRIPTCORE
            Color::Color(JSObjectRef obj)
            {
                r = JSC_JAVASCRIPT_ENGINE->jscGetDecimal (obj, "r");
                g = JSC_JAVASCRIPT_ENGINE->jscGetDecimal (obj, "g");
                b = JSC_JAVASCRIPT_ENGINE->jscGetDecimal (obj, "b");
                a = JSC_JAVASCRIPT_ENGINE->jscGetDecimal (obj, "a");
            }
        
            JSObjectRef Color::toJSCObject()
            {
                JSObjectRef vector = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector4");
                
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector, "r", r);
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector, "g", g);
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector, "b", b);
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector, "a", a);
                
                return (vector);
            }
        #endif

		String Color::toHex ()
		{
			RJNUMBER red = r;
			RJNUMBER green = g;
			RJNUMBER blue = b;

			red *= 255;
			green *= 255;
			blue *= 255;

			String ored = String::fromInt (red, 16);
			String ogreen = String::fromInt(green, 16);
			String oblue = String::fromInt(blue, 16);

			if (red <= 9)
				ored = "0" + ored;

			if (green <= 9)
				ogreen = "0" + ogreen;

			if (blue <= 9)
				oblue = "0" + oblue;

			return ("0x" + ored + ogreen + oblue);
		}

		String Color::toHTMLColor ()
		{
			String hex = toHex();
			hex = hex.substr (2);

			return ("#" + hex);
		}

		RJINT Color::toHexInt ()
		{
			String hex = toHex();

			return (parseInt(hex));
		}

		Color parseColor(String str)
		{
			Color color;

			if (str == "")
				return (color);

			RJINT iPos = 0;
			iPos = str.find("#");
			str = str.toLowerCase();

			if (str == "black")
				color = Color::Black;

			if (str == "white")
				color = Color::White;

			if (str == "red")
				color = Color::Red;

			if (str == "green")
				color = Color::Green;

			if (str == "blue")
				color = Color::Blue;

			if (iPos > -1)
				iPos++;

			String strR = str.substr((iPos + 0), 2);
			String strG = str.substr((iPos + 2), 2);
			String strB = str.substr((iPos + 4), 2);
			RJINT iR = parseInt(strR, 16);
			RJINT iG = parseInt(strG, 16);
			RJINT iB = parseInt(strB, 16);
			RJNUMBER dR = (iR / 255.0);
			RJNUMBER dG = (iG / 255.0);
			RJNUMBER dB = (iB / 255.0);

			color.r = dR;
			color.g = dG;
			color.b = dB;

			return (color);
		}
	}
}

