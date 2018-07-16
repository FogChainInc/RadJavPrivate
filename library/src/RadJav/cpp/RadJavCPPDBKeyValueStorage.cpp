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
#include "v8/RadJavV8JavascriptEngine.h"

#ifdef USE_LEVELDB
    #include <leveldb/db.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace Database
		{
			#ifdef USE_DATABASE
			KeyValueStorage::KeyValueStorage()
				: db(NULL)
			{
                //TODO: Need to add helper class to handle paths in portable way
                //F.e. which can prepend path before file name
                #ifdef WIN32
                    filePath = ".\leveldb.dat";
                #else
                    filePath = "./leveldb.dat";
                #endif
			}

			KeyValueStorage::~KeyValueStorage()
			{
                close();
			}

			/// The path to the database to open.
			RJBOOL KeyValueStorage::open(String path)
			{
                close();

                if (path == "")
					path = filePath;

				if (path == "")
				{
					RadJav::throwException("Can't open database! File path is empty.");

					return false;
				}

				// Open the database here.
                #ifdef USE_LEVELDB
                    leveldb::Options options;
                    options.create_if_missing = true;
                
                    leveldb::Status status = leveldb::DB::Open( options, path, &db);
                
                    if(!status.ok())
                    {
                        RadJav::throwException("Can't open database.");
                        return false;
                    }
                #endif

				return true;
			}

			/// Write to a key in the database.
			void KeyValueStorage::write(String key, String value)
			{
				#ifdef USE_LEVELDB
					if(!db)
					{
						RadJav::throwException("Database have not been opened.");
						return;
					}
                
                    leveldb::Status status = db->Put(leveldb::WriteOptions(), key, value);
                
                    if (!status.ok())
                    {
                        RadJav::throwException("Unable to write data into database.");
                        return;
                    }
                #endif
			}

			/// Read from a key in the database.
			String KeyValueStorage::read(String key)
			{
                String value = "";

				#ifdef USE_LEVELDB
                if(!db)
                {
                    RadJav::throwException("Database have not been opened.");
                    return value;
                }
                
                    leveldb::Status status = db->Get( leveldb::ReadOptions(), key, &value);
                
                    if (!status.ok())
                    {
                        RadJav::throwException("Unable to read data from database.");
                        return value;
                    }
                #endif
                
                return value;
			}

			/// Close the database.
			void KeyValueStorage::close()
			{
                #ifdef USE_LEVELDB
                    delete db;
                    db = NULL;
                #endif
			}
			#endif
		}
	}
}

