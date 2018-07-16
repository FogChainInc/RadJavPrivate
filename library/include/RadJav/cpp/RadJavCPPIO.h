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
#ifndef _RADJAV_CPP_IO_H_
#define _RADJAV_CPP_IO_H_

#include "RadJavPreprocessor.h"
#ifdef USE_BOOST
	#include <boost/asio.hpp>
#endif

#include "RadJavString.h"
#include "RadJavHashMap.h"

#ifdef WIN32
	#include <windows.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace IO
		{
			#ifdef USE_BOOST
				static boost::asio::thread_pool m_ioQueue(4);
			#endif

			class RADJAV_EXPORT FileIO
			{
				public:
					static RJBOOL isDir(String path_);
					static RJBOOL isFile(String path_);
					static RJBOOL isSymLink(String path_);

					static String currentPath();
					static void changePath(String path_);
					static RJBOOL exists(String patch_);

					static void createDir(String path_);
					static void copyDir(String src_, String dest_, RJBOOL recursive_ = true);
					static void renameDir(String src_, String dest_);
					static void deleteDir(String path_);
					static RJBOOL isEmpty(String path_);

					static void createSymLink(String path_, String link_);
					static void copySymLink(String src_, String dest_);
					static void renameSymLink(String src_, String dest_);
					static void deleteSymLink(String path_);

					static void copyFile(String src_, String dest_, RJBOOL overwriteIfExists_ = true);
					static void renameFile(String src_, String dest_);
					static void deleteFile(String path_);

					static Array<String> listFiles(String path_, RJBOOL recursive_ = true);
					static void listFilesAsync(String path_, RJBOOL recursive_ = true);

					static String normalizePath(String path_, String basePath_ = "");

					#ifdef USE_V8
						static v8::Persistent<v8::Function> *m_fileListEvent;
					#endif
			};

			class RADJAV_EXPORT SerialComm
			{
				public:
					static RJBOOL open(String port_, RJINT baud_rate_ = 115200);

					static inline RJBOOL isOpen()
					{
						return  m_serial.is_open();
					}

					static void close();

					static String read();
					static RJULONG write(String buffer_, RJINT bufferSize_ = -1);

				private:
					#ifdef USE_BOOST
						static boost::asio::io_service m_io;
						static boost::asio::serial_port m_serial;
					#endif

					static std::vector<int> m_baudRates;
				};

				class RADJAV_EXPORT TextFile
				{
					public:
						enum class operation : int
						{
							read = 0,
							write,
							append
						};

						static void writeFile(String path_, String contents_, RJINT outputType_ = static_cast<int>(IO::TextFile::operation::write));
						static void writeFileAsync(String path_, String contents_, RJINT outputType_ = static_cast<int>(IO::TextFile::operation::write));

						static String readFile(String path_);
						static void readFileAsync(String path_);
						
						#ifdef USE_V8
							static v8::Persistent<v8::Function> *m_textfileReadEvent;
						#endif
				};
		};
	}
}
#endif

