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

#include "cpp/RadJavCPPDBNuDB.h"

#include "RadJav.h"

#ifdef USE_NUDB
	#include <nudb/nudb.hpp>
	#include <boost/filesystem.hpp>

	using namespace nudb;
	namespace fs = boost::filesystem;

	const char* KDataFileName = "db.dat";
	const char* KKeyFileName = "db.key";
	const char* KLogFileName = "db.log";

	float KLoadFactor = 0.5f;
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace Database
		{
#ifdef USE_NUDB
			NuDB::NuDB()
			: db(nullptr)
			{}
			
			NuDB::~NuDB()
			{
				error_code ec;
				close(ec);
			}

			void NuDB::open( String path,
							 std::uint64_t appid,
							 std::size_t key_size,
							 error_code& ec)
			{
				fs::path dbPath = path;
				//Check if path already exists
				if(!fs::exists(dbPath))
				{
					fs::create_directories(dbPath, ec);
					if(ec)
						return;
				}

				//Check if path is a directory
				if(!fs::is_directory(dbPath, ec))
				{
					if(ec)
						return;
				}

				fs::path dataFilePath = dbPath;
				dataFilePath/=(KDataFileName);
				
				fs::path keyFilePath = dbPath;
				keyFilePath/=(KKeyFileName);

				fs::path logFilePath = dbPath;
				logFilePath/=(KLogFileName);

				if( !fs::exists(dataFilePath) || !fs::exists(keyFilePath))
				{
					//Creating database
					create<xxhasher>( dataFilePath.string(), keyFilePath.string(), logFilePath.string(),
									 appid, make_salt(), key_size, block_size("."), KLoadFactor, ec);
					
					if(ec)
						return;
				}
				
				db = RJNEW store;
				db->open( dataFilePath.string(), keyFilePath.string(), logFilePath.string(), ec);
				
				if(ec)
				{
					RJDELETE db;
					db = nullptr;
				}
			}
			
			void NuDB::insert(String key, String value, error_code& ec)
			{
				if(!db)
				{
					ec = errc::make_error_code(errc::bad_file_descriptor);
					return;
				}
				
				const std::size_t keySize = db->key_size();
				
				char keyBuffer[keySize];
				memset(keyBuffer, 0, keySize);
				memcpy(keyBuffer, key.c_str(), key.size());
				
				db->insert( keyBuffer, value.c_str(), value.size(), ec);
			}
			
			String NuDB::read(String key, error_code& ec)
			{
				String value = "";

				if(!db)
				{
					ec = errc::make_error_code(errc::bad_file_descriptor);
					return value;
				}

				const std::size_t keySize = db->key_size();
				
				char keyBuffer[keySize];
				memset(keyBuffer, 0, keySize);
				memcpy(keyBuffer, key.c_str(), key.size());
				
				db->fetch( keyBuffer,
						  [&](void const* buffer, std::size_t size)
						  {
							  value.append((char*) buffer, size);
						  }, ec);
				
				return value;
			}
			
			void NuDB::close(error_code& ec)
			{
				if(db)
					db->close(ec);
				
				RJDELETE db;
				db = nullptr;
			}
#endif
		}
	}
}
