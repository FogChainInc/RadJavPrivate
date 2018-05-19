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

#include "RadJav.h"

#ifdef USE_DATABASE
	#ifdef USE_LEVELDB
    	#include <leveldb/db.h>
		using namespace leveldb;
	#endif

	#ifdef USE_ROCKSDB
		#include <rocksdb/db.h>
		using namespace rocksdb;
	#endif

	#ifdef USE_NUDB
		#include "cpp/RadJavCPPDBNuDB.h"
		const std::uint64_t KAppID = 1;
		const std::size_t KKeySize = 128;
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
				#if defined USE_LEVELDB || defined USE_ROCKSDB || defined USE_NUDB
					: db(nullptr)
				#endif
			{
                //TODO: Need to add helper class to handle paths in portable way
                //F.e. which can prepend path before file name
                #ifdef WIN32
                    filePath = ".\\db.dat";
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

					#ifdef USE_SNAPPY
						options.OptimizeLevelStyleCompaction();
					#endif
				#endif

				#if defined USE_LEVELDB || defined USE_ROCKSDB
					Status status = DB::Open( options, path, &db);
				
					if(!status.ok())
					{
						RadJav::throwException("Can't open database.");
						return false;
					}
				#endif
				
				#ifdef USE_NUDB
					db = RJNEW NuDB();
					boost::system::error_code ec;
					db->open( path, KAppID, KKeySize, ec);

					if(ec)
					{
						RadJav::throwException("Can't create(open) database.");
						
						RJDELETE db;
						db = nullptr;
						
						return false;
					}
				#endif

				return true;
			}

			/// Write to a key in the database.
			void KeyValueStorage::write(String key, String value)
			{
				#if defined USE_LEVELDB || defined USE_ROCKSDB || defined USE_NUDB
					if(!db)
					{
						RadJav::throwException("Database have not been opened.");
						return;
					}
				#endif
				
				#if defined USE_LEVELDB || defined USE_ROCKSDB
					Status status = db->Put(WriteOptions(), key, value);
				
					if (!status.ok())
					{
						RadJav::throwException("Unable to write data into database.");
						return;
					}
				#endif
				
				#ifdef USE_NUDB
					boost::system::error_code ec;
				
					db->insert( key, value, ec);
					if(ec)
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
				
				#if defined USE_LEVELDB || defined USE_ROCKSDB || defined USE_NUDB
					if(!db)
					{
						RadJav::throwException("Database have not been opened.");
						return value;
					}
				#endif

				#if defined USE_LEVELDB || defined USE_ROCKSDB
                    Status status = db->Get( ReadOptions(), key, &value);
                
                    if (!status.ok())
                    {
                        RadJav::throwException("Unable to read data from database.");
                        return value;
                    }
                #endif
				
				#ifdef USE_NUDB
					boost::system::error_code ec;
					value = db->read( key, ec);
				
					if(ec)
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
                    db = nullptr;
                #endif
				
				#ifdef USE_NUDB
					boost::system::error_code ec;
					if(db)
						db->close(ec);
				
					RJDELETE db;
					db = nullptr;
				#endif
			}
			#endif
		}
	}
}

