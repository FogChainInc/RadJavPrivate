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
	/** @class RadJav.Vector3
	* A Vector3 class.
	* Available on platforms: Windows,Linux,OSX,HTML5
	*/
	export class Vector3
	{
		constructor(x: number | RadJav.Vector2 | RadJav.Vector3 = 0, y: number = 0, z: number = 0)
		{
			if (x instanceof RadJav.Vector2)
			{
				let temp = x;
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

			this.x = x;
			this.y = y;
			this.z = z;
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

		/** @method tostring
		* Convert this object to a string.
		* @return {string} The string representing this object.
		*/
		tostring(): string
		{
			return (this.x + "," + this.y + "," + this.z);
		}

		/** @method add
		* Add x,y,z values to the x,y,z components of this object.
		* @param {number} x The X component to add.
		* @param {number} y The Y component to add.
		* @param {number} z The Z component to add.
		*/
		add(vector3: Vector3 | number): Vector3
		{
			let result = new RadJav.Vector3();

			if (vector3 instanceof RadJav.Vector3)
			{
				result.x = this.x + vector3.x;
				result.y = this.y + vector3.y;
				result.z = this.z + vector3.z;
			}
			else
			{
				result.x = this.x + (<number>vector3);
				result.y = this.y + (<number>vector3);
				result.z = this.z + (<number>vector3);
			}

			return result;
		}

		/** @method sub
		* Subtract X and Y values from the X and Y components of this object.
		* @param {number} x The X component to subtract.
		* @param {number} y The Y component to subtract.
		* @param {number} z The Z component to subtract.
		*/
		subtract(vector3: Vector3 | number): Vector3
		{
			let result = new RadJav.Vector3();

			if (vector3 instanceof RadJav.Vector3)
			{
				result.x = this.x - vector3.x;
				result.y = this.y - vector3.y;
				result.z = this.z - vector3.z;
			}
			else
			{
				result.x = this.x - (<number>vector3);
				result.y = this.y - (<number>vector3);
				result.z = this.z - (<number>vector3);
			}

			return result;
		}

		/** @method multiply
		* Multiply x,y,z values to the x,y,z components of this object.
		* @param {number} x The X component to subtract.
		* @param {number} y The Y component to subtract.
		* @param {number} z The Z component to subtract.
		*/
		multiply(vector3: Vector3 | number): Vector3
		{
			let result = new RadJav.Vector3();

			if (vector3 instanceof RadJav.Vector3)
			{
				result.x = this.x * vector3.x;
				result.y = this.y * vector3.y;
				result.z = this.z * vector3.z;
			}
			else
			{
				result.x = this.x * (<number>vector3);
				result.y = this.y * (<number>vector3);
				result.z = this.z * (<number>vector3);
			}

			return result;
		}

		/** @method divide
		* Divide this object by another Vector3 object or number.
		* @param {RadJav.Vector3 | number} Vector3 The Vector3 or number to divide by.
		*/
		divide(vector3: Vector3 | number): Vector3
		{
			let result = new RadJav.Vector3();

			if (vector3 instanceof RadJav.Vector3)
			{
				result.x = this.x / vector3.x;
				result.y = this.y / vector3.y;
				result.z = this.z / vector3.z;
			}
			else
			{
				result.x = this.x / (<number>vector3);
				result.y = this.y / (<number>vector3);
				result.z = this.z / (<number>vector3);
			}

			return result;
		}

		/** @method dot
		* Perform a dot product on this object.
		* @param {RadJav.Vector3} vector3 The Vector3 to perform the dot product.
		* @return {number} The result of the dot product.
		*/
		dot(vector3: Vector3): number
		{
			let dReturn = this.x * vector3.x + this.y * vector3.y + this.z * vector3.z;

			return dReturn;
		}

		/** @method cross
		* Perform a cross product on this object.
		* @param {RadJav.Vector3} vector3 The Vector3 to perform the dot product.
		* @return {number} The result of the dot product.
		*/
		cross(vector3: Vector3): Vector3
		{
			let result = new RadJav.Vector3();

			result.x = this.y * vector3.z - this.z * vector3.y;
			result.y = this.z * vector3.x - this.x * vector3.z;
			result.z = this.x * vector3.y - this.y * vector3.x;

			return result;
		}

		/** @method length
		* Get the length of this object using a square root. This will use Math.sqrt.
		* @return {number} The length of this object.
		*/
		length(): number
		{
			let dReturn = Math.sqrt(this.x * this.x + this.y * this.y + this.z * this.z);

			return dReturn;
		}

		/** @method normalize
		* Normalize this object, this will use this object's length method.
		* @return {number} The normalized length of this object.
		*/
		normalize(): Vector3
		{
			let dReturn: Vector3 = this.divide(this.length());

			return dReturn;
		}

		/** @method squaredLength
		* Normalize this object, this will use this object's length method.
		* @return {number} The normalized length of this object.
		*/
		squaredLength(): number
		{
			let dReturn = this.x * this.x + this.y * this.y + this.z * this.z;

			return dReturn;
		}

		/** @method absDotProduct
		* Get the dot product as an absolute value.
		* @param {RadJav.Vector3} vector3 The Vector3 to perform the dot product.
		* @return {number} The absolute value of the dot product.
		*/
		absDotProduct(vector3: Vector3): number
		{
			let dReturn = Math.abs(this.dot(vector3));

			return dReturn;
		}

		/** @method angleBetween
		* Get the angle between two vectors.
		* @param {RadJav.Vector3} vector3 The Vector3 to get the angle from.
		* @return {number} The angle.
		*/
		angleBetween(vector3: Vector3): number
		{
			let dTheta = this.dot(vector3) / (this.length() * vector3.length());
			let dReturn = Math.acos(Math.clamp(dTheta, -1, 1));

			return dReturn;
		}

		/** @method distance
		* Get the distance between two vectors.
		* @param {RadJav.Vector3} vector3 The Vector3 to get the distance from.
		* @return {number} The distance.
		*/
		distance(vector3: Vector3): number
		{
			let dX = this.x - vector3.x;
			let dY = this.y - vector3.y;
			let dZ = this.z - vector3.z;
			let dReturn = Math.sqrt(dX * dX + dY * dY + dZ * dZ);

			return dReturn;
		}

		static lerp (start: Vector3, end: Vector3, alpha: number): Vector3
		{
			let temp1: Vector3 = end.subtract (start);
			let temp2: Vector3 = temp1.multiply (alpha);
			let result: Vector3 = start.add (temp2);

			return (result);
		}
	}
}
