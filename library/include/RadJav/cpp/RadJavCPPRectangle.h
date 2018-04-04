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
#ifndef _RADJAV_GUI_CPP_RECTANGLE_H_
#define _RADJAV_GUI_CPP_RECTANGLE_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"
#include "RadJavHashMap.h"

#include "cpp/RadJavCPPVector2.h"

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		class RADJAV_EXPORT Rectangle
		{
			public:
				Rectangle(RJNUMBER x = 0, RJNUMBER y = 0, RJNUMBER w = 0, RJNUMBER h = 0);
				#ifdef USE_V8
					Rectangle(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> obj);

					static v8::Local<v8::Object> toV8Object(V8JavascriptEngine *jsEngine, Rectangle *obj);
				#endif

				RJNUMBER x;
				RJNUMBER y;
				RJNUMBER width;
				RJNUMBER height;
				RJNUMBER top;
				RJNUMBER bottom;
				RJNUMBER left;
				RJNUMBER right;

				/** @method setPosition
				* Set the position of this object.
				* @param {Number|RadJav.Vector2} x The x coordinate of this object, or the Vector2
				* representing the coordinates of this object.
				* @param {Number} [y=null] The y coordinate of this object.
				*/
				void setPosition(Vector2 pos);
				void setPosition(RJNUMBER x, RJNUMBER y);

				/** @method getPosition
				* Get the position of this object.
				* @return {RadJav.Vector2} The Vector2 representing the position of this object.
				*/
				Vector2 getPosition();

				/** @method setSize
				* Set the size of this object.
				* @param {Number|RadJav.Vector2} w The width of this object, or the Vector2
				* representing the size of this object.
				* @param {Number} [h=null] The height of this object.
				*/
				void setSize(Vector2 size);
				void setSize(RJNUMBER w, RJNUMBER h);

				/** @method getSize
				* Get the size of this object.
				* @return {RadJav.Vector2} The Vector2 representing the size of this object.
				*/
				Vector2 getSize();

				/** @method pointInside
				* Check if a point is inside this object.
				* @param {Number|RadJav.Vector2} x The x coordinate of the point, or the Vector2
				* representing the coordinates of the point to check.
				* @param {Number} [y=null] The y coordinate of the point.
				* @return {Boolean} Returns true if the point is inside the rectangle.
				*/
				RJBOOL pointInside(RJNUMBER x, RJNUMBER y);

				/** @method xInside
				* Check if a x coordinate is inside this rectangle.
				* @param {Number} x The x coordinate of the point to check.
				* @return {Boolean} Returns true if the x coordinate is inside the rectangle.
				*/
				RJBOOL xInside(RJNUMBER x);

				/** @method yInside
				* Check if a y coordinate is inside this rectangle.
				* @param {Number} y The y coordinate of the point to check.
				* @return {Boolean} Returns true if the y coordinate is inside the rectangle.
				*/
				RJBOOL yInside(RJNUMBER y);
		};
	}
}
#endif

