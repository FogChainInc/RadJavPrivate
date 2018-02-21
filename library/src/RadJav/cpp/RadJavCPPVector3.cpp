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
#include "cpp/RadJavCPPVector3.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		Vector3 Vector3::add(Vector3 vector3)
		{
			Vector3 result;

			result.x = x + vector3.x;
			result.y = y + vector3.y;
			result.z = z + vector3.z;

			return (result);
		}

		Vector3 Vector3::add(RJNUMBER vector3)
		{
			Vector3 result;

			result.x = x + vector3;
			result.y = y + vector3;
			result.z = z + vector3;

			return (result);
		}

		Vector3 Vector3::subtract(Vector3 vector3)
		{
			Vector3 result;

			result.x = x - vector3.x;
			result.y = y - vector3.y;
			result.z = z - vector3.z;

			return (result);
		}

		Vector3 Vector3::subtract(RJNUMBER vector3)
		{
			Vector3 result;

			result.x = x - vector3;
			result.y = y - vector3;
			result.z = z - vector3;

			return (result);
		}

		Vector3 Vector3::multiply(Vector3 vector3)
		{
			Vector3 result;

			result.x = x * vector3.x;
			result.y = y * vector3.y;
			result.z = z * vector3.z;

			return (result);
		}

		Vector3 Vector3::multiply(RJNUMBER vector3)
		{
			Vector3 result;

			result.x = x * vector3;
			result.y = y * vector3;
			result.z = z * vector3;

			return (result);
		}

		Vector3 Vector3::divide(Vector3 vector3)
		{
			Vector3 result;

			result.x = x / vector3.x;
			result.y = y / vector3.y;
			result.z = z / vector3.z;

			return (result);
		}

		Vector3 Vector3::divide(RJNUMBER vector3)
		{
			Vector3 result;

			result.x = x / vector3;
			result.y = y / vector3;
			result.z = z / vector3;

			return (result);
		}

		RJNUMBER Vector3::dot(Vector3 vector3)
		{
			RJNUMBER dReturn = ((x * vector3.x) + (y * vector3.y) + (z * vector3.z));

			return (dReturn);
		}

		Vector3 Vector3::cross(Vector3 vector3)
		{
			Vector3 result;

			result.x = (y * vector3.z - z * vector3.y);
			result.y = (z * vector3.x - x * vector3.z);
			result.z = (x * vector3.y - y * vector3.x);

			return (result);
		}

		RJNUMBER Vector3::length()
		{
			RJNUMBER dReturn = Math::sqrt((x * x) + (y * y) + (z * z));

			return (dReturn);
		}

		Vector3 Vector3::normalize()
		{
			Vector3 dReturn = divide(length());

			return (dReturn);
		}

		RJNUMBER Vector3::squaredLength()
		{
			RJNUMBER dReturn = ((x * x) + (y * y) + (z * z));

			return (dReturn);
		}

		RJNUMBER Vector3::absDotProduct(Vector3 vector3)
		{
			RJNUMBER  dReturn = Math::abs(dot(vector3));

			return (dReturn);
		}

		RJNUMBER Vector3::angleBetween(Vector3 vector3)
		{
			RJNUMBER dTheta = dot(vector3) / (length() * vector3.length());
			RJNUMBER dReturn = Math::acos(Math::clamp(dTheta, -1, 1));

			return (dReturn);
		}

		RJNUMBER Vector3::distance(Vector3 vector3)
		{
			RJNUMBER dX = (x - vector3.x);
			RJNUMBER dY = (y - vector3.y);
			RJNUMBER dZ = (z - vector3.z);
			RJNUMBER dReturn = Math::sqrt((dX * dX) + (dY * dY) + (dZ * dZ));

			return (dReturn);
		}

		Vector3 Vector3::parseVector3(String str)
		{
			Vector3 obj;

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

			return (obj);
		}
	}
}

