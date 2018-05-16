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
#ifndef _RADJAV_CPP_DATABASE_NUDB_H_
#define _RADJAV_CPP_DATABASE_NUDB_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include <boost/system/error_code.hpp>

#ifdef USE_NUDB
	#include <nudb/store.hpp>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace Database
		{
#ifdef USE_NUDB
			// Accepts incoming connections and launches the sessions
			class RADJAV_EXPORT NuDB
			{
			public:
				NuDB();
				virtual ~NuDB();
				
				NuDB(const NuDB&) = delete;
				NuDB& operator=(const NuDB&) = delete;
				
			public:
				/// The path to the database to open.
				virtual void open( String path,
								   std::uint64_t appid,
								   std::size_t key_size,
								   boost::system::error_code& ec);
				
				/// Insert data into the database.
				virtual void insert(String key, String value, boost::system::error_code& ec);
				
				/// Read from data by key in the database.
				virtual String read(String key, boost::system::error_code& ec);
				
				/// Close the database.
				virtual void close(boost::system::error_code& ec);
				
			protected:
				String path;
				
				nudb::store *db;
			};
#endif
		}
	}
}
#endif

