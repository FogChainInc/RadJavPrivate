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
#ifndef _RADJAV_CPP_DATABASE_KEYVALUESTORAGE_H_
	#define _RADJAV_CPP_DATABASE_KEYVALUESTORAGE_H_

	#include "RadJavPreprocessor.h"

	#include <string>

	#include "RadJav.h"
	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "RadJavThread.h"

    #ifdef USE_LEVELDB
        namespace leveldb
        {
            class DB;
        };
    #endif

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Database
			{
				// Accepts incoming connections and launches the sessions
				class RADJAV_EXPORT KeyValueStorage
				{
					public:
						KeyValueStorage();
						~KeyValueStorage();

                    public:
						/// The path to the database to open.
						RJBOOL open(String path = "");

						/// Write to a key in the database.
						void write(String key, String value);

						/// Read from a key in the database.
						String read(String key);

						/// Close the database.
						void close();

                    public:
                        String filePath;
                    
                    private:
                        #ifdef USE_LEVELDB
                            leveldb::DB* db;
                        #endif
				};
			}
		}
	}
#endif

