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

#ifdef USE_DATABASE

#ifdef USE_LEVELDB
    #include <leveldb/db.h>
	using namespace leveldb;
#endif

#ifdef USE_ROCKSDB
	#include <rocksdb/db.h>
	using namespace rocksdb;
#endif

#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace Database
		{
			#ifdef USE_DATABASE
			KeyValueStorage::KeyValueStorage()
				#if defined USE_LEVELDB || defined USE_ROCKSDB
					: db(NULL)
				#endif
			{
                //TODO: Need to add helper class to handle paths in portable way
                //F.e. which can prepend path before file name
                #ifdef WIN32
                    filePath = ".\db.dat";
                #else
                    filePath = "./db.dat";
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
				
				#if defined USE_LEVELDB || defined USE_ROCKSDB
					Options options;
				
					// create the DB if it's not already present
					options.create_if_missing = true;
				#endif
				
				#ifdef USE_ROCKSDB
					// Optimize RocksDB. This is the easiest way to get RocksDB to perform well
					options.IncreaseParallelism();
					options.OptimizeLevelStyleCompaction();
				#endif

				#if defined USE_LEVELDB || defined USE_ROCKSDB
					Status status = DB::Open( options, path, &db);
				
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
				#if defined USE_LEVELDB || defined USE_ROCKSDB
					if(!db)
					{
						RadJav::throwException("Database have not been opened.");
						return;
					}
				
					Status status = db->Put(WriteOptions(), key, value);
				
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

				#if defined USE_LEVELDB || defined USE_ROCKSDB
                	if(!db)
                	{
                    	RadJav::throwException("Database have not been opened.");
                    	return value;
                	}
                
                    Status status = db->Get( ReadOptions(), key, &value);
                
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
                #if defined USE_LEVELDB || defined USE_ROCKSDB
                    delete db;
                    db = NULL;
                #endif
			}
			#endif
		}
	}
}

