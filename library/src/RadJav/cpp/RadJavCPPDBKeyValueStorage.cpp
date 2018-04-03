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

#include "cpp/RadJavCPPDBKeyValueStorage.h"


#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

#include "RadJavString.h"
#include "RadJav\v8\RadJavV8JavascriptEngine.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace Database
		{
			KeyValueStorage::KeyValueStorage()
			{
				filePath = "";
			}

			KeyValueStorage::~KeyValueStorage()
			{
			}

			/// The path to the database to open.
			RJBOOL KeyValueStorage::open(String path)
			{
				if (path == "")
					path = filePath;

				if (path == "")
				{
					RadJav::throwException("Can't open database! File path is empty.");

					return (false);
				}

				// Open the database here.

				return (true);
			}

			/// Write to a key in the database.
			void KeyValueStorage::write(String key, String value)
			{
			}

			/// Read from a key in the database.
			String KeyValueStorage::read(String key)
			{
				return ("data");
			}

			/// Close the database.
			void KeyValueStorage::close()
			{
			}
		}
	}
}

