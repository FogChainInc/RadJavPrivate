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
#include "cpp/RadJavCPPVector4.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
        #ifdef USE_V8
            Vector4::Vector4(v8::Local<v8::Object> obj)
            {
                x = V8_JAVASCRIPT_ENGINE->v8GetDecimal (obj, "x");
                y = V8_JAVASCRIPT_ENGINE->v8GetDecimal (obj, "y");
                z = V8_JAVASCRIPT_ENGINE->v8GetDecimal (obj, "z");
                w = V8_JAVASCRIPT_ENGINE->v8GetDecimal (obj, "w");
            }
        
            v8::Local<v8::Object> Vector4::toV8Object()
            {
                v8::Local<v8::Object> vector = V8_JAVASCRIPT_ENGINE->v8GetObject(V8_RADJAV, "Vector4");
                
                V8_JAVASCRIPT_ENGINE->v8SetNumber(vector, "x", x);
                V8_JAVASCRIPT_ENGINE->v8SetNumber(vector, "y", y);
                V8_JAVASCRIPT_ENGINE->v8SetNumber(vector, "z", z);
                V8_JAVASCRIPT_ENGINE->v8SetNumber(vector, "w", w);
                
                return (vector);
            }
        #endif
        #ifdef USE_JAVASCRIPTCORE
            Vector4::Vector4(JSObjectRef obj)
            {
                x = JSC_JAVASCRIPT_ENGINE->jscGetDecimal (obj, "x");
                y = JSC_JAVASCRIPT_ENGINE->jscGetDecimal (obj, "y");
                z = JSC_JAVASCRIPT_ENGINE->jscGetDecimal (obj, "z");
                w = JSC_JAVASCRIPT_ENGINE->jscGetDecimal (obj, "w");
            }
        
            JSObjectRef Vector4::toJSCObject()
            {
                JSObjectRef vector = JSC_JAVASCRIPT_ENGINE->jscGetObject(JSC_RADJAV, "Vector4");
                
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector, "x", x);
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector, "y", y);
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector, "z", z);
                JSC_JAVASCRIPT_ENGINE->jscSetNumber(vector, "w", w);
                
                return (vector);
            }
        #endif

		Vector4 Vector4::parseVector4(String str)
		{
			Vector4 obj;

			if (str == "")
				return (obj);

			str = str.toLowerCase();
			str = str.replaceAll(" ", "");
			Array<String> strs = str.split(",");

			if (strs.size() > 0)
				obj.x = parseFloat(strs[0]);

			if (strs.size() > 1)
				obj.y = parseFloat(strs[1]);

			if (strs.size() > 2)
				obj.z = parseFloat(strs[2]);

			if (strs.size() > 3)
				obj.w = parseFloat(strs[3]);

			return (obj);
		}
	}
}

