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

/// <reference path="RadJav.ts" />

namespace RadJav
{
	/** @class RadJav.Vector4
	* A Vector4 class.
	* Available on platforms: Windows,Linux,OSX,HTML5
	*/
	export class Vector4
	{
		constructor(x: number | RadJav.Vector2 | RadJav.Vector3 | RadJav.Vector4 = 0, y: number = 0, z: number = 0, w: number = 0)
		{
			if (x instanceof RadJav.Vector2)
			{
				var temp = x;
				x = temp.x;
				y = temp.y;
			}

			if (x instanceof RadJav.Vector3)
			{
				let temp = x;
				x = temp.x;
				y = temp.y;
				z = temp.z;
			}

			if (x instanceof RadJav.Vector4)
			{
				let temp = x;
				x = temp.x;
				y = temp.y;
				z = temp.z;
				w = temp.w;
			}

			this.x = x;
			this.y = y;
			this.z = z;
			this.w = w;
		}

		/** @property {number} [x=0]
		* The X component.
		*/
		x: number;
		/** @property {number} [y=0]
		* The Y component.
		*/
		y: number;
		/** @property {number} [z=0]
		* The Z component.
		*/
		z: number;
		/** @property {number} [w=0]
		* The W component.
		*/
		w: number;

		/** @method tostring
		* Convert this object to a string.
		* @return {string} The string representing this object.
		*/
		tostring(): string
		{
			return this.x + "," + this.y + "," + this.z + "," + this.w;
		}

		lerp (start: Vector4, end: Vector4, time: number): Vector4
		{
			let result: Vector4 = new Vector4 ();

			result.x = Math.lerp (start.x, end.x, time);
			result.y = Math.lerp (start.y, end.y, time);
			result.z = Math.lerp (start.z, end.z, time);

			return (result);
		}
	}
}

function parseVector4(string: string): RadJav.Vector4
{
	var v4dReturn = new RadJav.Vector4();

	if (string == "") return v4dReturn;

	string = string.toLowerCase();
	string = string.replace(/ /g, "");
	var arystrings = string.split(",");

	v4dReturn.x = parseFloat(arystrings[0]);
	v4dReturn.y = parseFloat(arystrings[1]);
	v4dReturn.z = parseFloat(arystrings[2]);
	v4dReturn.w = parseFloat(arystrings[3]);

	return v4dReturn;
}
