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
#ifndef _RADJAV_GUI_CPP_VECTOR3_H_
#define _RADJAV_GUI_CPP_VECTOR3_H_

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
		 * @brief Vector3 class.
		 */
		class RADJAV_EXPORT Vector3
		{
			public:
                #ifdef USE_V8
                    Vector3(v8::Local<v8::Object> obj);
            
                    v8::Local<v8::Object> toV8Object();
                #endif
                #ifdef USE_JAVASCRIPTCORE
                    Vector3(JSObjectRef obj);
            
                    JSObjectRef toJSCObject();
                #endif

				inline Vector3(RJNUMBER x = 0, RJNUMBER y = 0, RJNUMBER z = 0)
				{
					this->x = x;
					this->y = y;
					this->z = z;
				}

				/** Convert this object to a string.
				* @return {String} The string representing this object.
				*/
				inline String toString()
				{
					return (String::fromInt (x) + "," + String::fromInt (y) + "," + String::fromInt(z));
				}

				/** Set the X component of this object.
				* @param {Number} n The new X component of this object.
				*/
				inline void setX (RJNUMBER n)
				{
					x = n;
				}

				/** Set the Y component of this object.
				* @param {Number} n The new Y component of this object.
				*/
				inline void setY (RJNUMBER n)
				{
					y = n;
				}

				/** Set the Z component of this object.
				* @param {Number} n The new Z component of this object.
				*/
				inline void setZ(RJNUMBER n)
				{
					y = n;
				}

				/** Set the X,Y,Z component of this object.
				* @param {Number} x The new X component of this object.
				* @param {Number} y The new Y component of this object.
				* @param {Number} z The new Z component of this object.
				*/
				inline void set (RJNUMBER x = 0, RJNUMBER y = 0, RJNUMBER z = 0)
				{
					this->x = x;
					this->y = y;
					this->z = z;
				}

				/** @method add
				* Add x,y,z values to the x,y,z components of this object.
				* @param {Number} x The X component to add.
				* @param {Number} y The Y component to add.
				* @param {Number} z The Z component to add.
				*/
				Vector3 add (Vector3 vector3);
				Vector3 add(RJNUMBER num);

				/** @method sub
				* Subtract X and Y values from the X and Y components of this object.
				* @param {Number} x The X component to subtract.
				* @param {Number} y The Y component to subtract.
				* @param {Number} z The Z component to subtract.
				*/
				Vector3 subtract(Vector3 vector3);
				Vector3 subtract(RJNUMBER num);

				/** @method multiply
				* Multiply x,y,z values to the x,y,z components of this object.
				* @param {Number} x The X component to subtract.
				* @param {Number} y The Y component to subtract.
				* @param {Number} z The Z component to subtract.
				*/
				Vector3 multiply(Vector3 vector3);
				Vector3 multiply(RJNUMBER num);

				/** @method divide
				* Divide this object by another Vector3 object or number.
				* @param {RadJav.Vector3|Number} Vector3 The Vector3 or Number to divide by.
				*/
				Vector3 divide(Vector3 vector3);
				Vector3 divide(RJNUMBER num);

				/** @method dot
				* Perform a dot product on this object.
				* @param {RadJav.Vector3} vector3 The Vector3 to perform the dot product.
				* @return {Number} The result of the dot product.
				*/
				RJNUMBER dot(Vector3 vector3);

				/** @method cross
				* Perform a cross product on this object.
				* @param {RadJav.Vector3} vector3 The Vector3 to perform the dot product.
				* @return {Number} The result of the dot product.
				*/
				Vector3 cross(Vector3 vector3);

				/** @method length
				* Get the length of this object using a square root. This will use Math.sqrt.
				* @return {Number} The length of this object.
				*/
				RJNUMBER length();

				/** @method normalize
				* Normalize this object, this will use this object's length method.
				* @return {Vector3} The normalized length of this object.
				*/
				Vector3 normalize();

				/** @method squaredLength
				* Normalize this object, this will use this object's length method.
				* @return {Number} The normalized length of this object.
				*/
				RJNUMBER squaredLength();

				/** @method absDotProduct
				* Get the dot product as an absolute value.
				* @param {RadJav.Vector3} vector3 The Vector3 to perform the dot product.
				* @return {Number} The absolute value of the dot product.
				*/
				RJNUMBER absDotProduct(Vector3 vector3);

				/** @method angleBetween
				* Get the angle between two vectors.
				* @param {RadJav.Vector3} vector3 The Vector3 to get the angle from.
				* @return {Number} The angle.
				*/
				RJNUMBER angleBetween(Vector3 vector3);

				/** @method distance
				* Get the distance between two vectors.
				* @param {RadJav.Vector3} vector3 The Vector3 to get the distance from.
				* @return {Number} The distance.
				*/
				RJNUMBER distance(Vector3 vector3);

				/// The X component.
				RJNUMBER x;
				/// The Y component.
				RJNUMBER y;
				/// The Z component.
				RJNUMBER z;

				/** Parse a Vector3 string and create a Vector3 object from it.
				* @param {String} str The string to parse.
				* @return {RadJav.Vector3} The new Vector3 created from this string.
				*/
				static Vector3 parseVector3(String str);
		};
	}
}
#endif

