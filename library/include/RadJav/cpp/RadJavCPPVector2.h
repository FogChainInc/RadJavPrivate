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
#ifndef _RADJAV_GUI_CPP_VECTOR2_H_
#define _RADJAV_GUI_CPP_VECTOR2_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"
#include "RadJavHashMap.h"

#include "cpp/RadJavCPPMath.h"

namespace RadJAV
{
	namespace CPP
	{
		class RADJAV_EXPORT Vector2
		{
			public:
				inline Vector2(RJNUMBER x = 0, RJNUMBER y = 0)
				{
					this->x = x;
					this->y = y;
				}

				/** Convert this object to a string.
				* @return {String} The string representing this object.
				*/
				inline String toString()
				{
					return (String::fromInt (x) + "," + String::fromInt (y));
				}

				/** Set the X component of this object.
				* @param {Number} n The new X component of this object.
				*/
				inline void setX (RJNUMBER n)
				{
					x = n;
				}

				/** Set the X component of this object.
				* @param {Number} n The new X component of this object.
				*/
				inline void setY (RJNUMBER n)
				{
					y = n;
				}

				/** Set the X,Y component of this object.
				* @param {Number} x The new X component of this object.
				* @param {Number} y The new Y component of this object.
				*/
				inline void set (RJNUMBER x = 0, RJNUMBER y = 0)
				{
					x = x;
					y = y;
				}

				/** Add X and Y values to the X and Y components of this object.
				* @param {Number} x The X component to add.
				* @param {Number} y The Y component to add.
				*/
				inline void add (RJNUMBER x, RJNUMBER y)
				{
					x += x;
					y += y;
				}

				/** Subtract X and Y values from the X and Y components of this object.
				* @param {Number} x The X component to subtract.
				* @param {Number} y The Y component to subtract.
				*/
				inline void sub (Vector2 subVector2)
				{
					x -= subVector2.x;
					y -= subVector2.y;
				}

				/** Multiply X and Y values to the X and Y components of this object.
				* @param {Number} x The X component to subtract.
				* @param {Number} y The Y component to subtract.
				*/
				inline void mult (RJNUMBER n)
				{
					x *= n;
					y *= n;
				}

				/** Divide this object by another Vector2 object or number.
				* @param {RadJav.Vector2} vector2 The Vector2 or Number to divide by.
				*/
				inline Vector2 divide (Vector2 vector2)
				{
					Vector2 result;

					result.x = x / vector2.x;
					result.y = y / vector2.y;

					return (result);
				}

				/** Divide this object by another Vector2 object or number.
				* @param {RadJav.Vector2} vector2 The Vector2 or Number to divide by.
				*/
				inline Vector2 divide(RJNUMBER num)
				{
					Vector2 result;

					result.x = x / num;
					result.y = y / num;

					return (result);
				}

				/** Perform a dot product on this object.
				* @param {RadJav.Vector2} dotVector2 The Vector2 to perform the dot product.
				* @return {Number} The result of the dot product.
				*/
				inline RJNUMBER dot (Vector2 dotVector2)
				{
					return (x * dotVector2.x + y * dotVector2.y);
				}

				/** Get the length of this object using a square root. This will use Math.sqrt.
				* @return {Number} The length of this object.
				*/
				inline RJNUMBER length ()
				{
					return (Math::sqrt((x * x) + (y * y)));
				}

				/** Normalize this object, this will use this object's length method.
				* @return {Vector2} The normalized length of this object.
				*/
				inline Vector2 normalize ()
				{
					return divide(length());
				}

				/// The X component.
				RJNUMBER x;
				/// The Y component.
				RJNUMBER y;

				/** Parse a Vector2 string and create a Vector2 object from it.
				* @param {String} str The string to parse.
				* @return {RadJav.Vector2} The new Vector2 created from this string.
				*/
				static Vector2 parseVector2(String str);
		};
	}
}
#endif

