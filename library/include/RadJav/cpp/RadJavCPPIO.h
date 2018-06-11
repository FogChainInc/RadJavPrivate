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

#include "RadJavString.h"
#include "RadJavHashMap.h"

#ifdef WIN32
	#include <windows.h>
#endif

namespace RadJAV
{
	namespace CPP
	{
		class RADJAV_EXPORT IO
		{
			public:
				static void isDir(const v8::FunctionCallbackInfo<v8::Value> &args);
				static void isFile(const v8::FunctionCallbackInfo<v8::Value> &args);
				static void createDir(const v8::FunctionCallbackInfo<v8::Value> &args);
				static void deleteFile(const v8::FunctionCallbackInfo<v8::Value> &args);
				static void copyFile(String src, String dest, RJBOOL overwriteIfExists = true);
				static void copyDir(String src, String dest, RJBOOL recursive = true);
				static Array<String> listFiles(String path, RJBOOL recursive = true);

				class RADJAV_EXPORT SerialComm
				{
					public:
						SerialComm();

						inline String getPort()
						{
							return (portName);
						}

						inline RJINT getBaud(const v8::FunctionCallbackInfo<v8::Value> &args)
						{
							return (baud);
						}

						inline RJINT getByteSize(const v8::FunctionCallbackInfo<v8::Value> &args)
						{
							return (byteSize);
						}

						inline RJINT getStopBits(const v8::FunctionCallbackInfo<v8::Value> &args)
						{
							return (stopBits);
						}

						inline RJINT getParity(const v8::FunctionCallbackInfo<v8::Value> &args)
						{
							return (parity);
						}

						RJBOOL open();

						inline RJBOOL isOpen()
						{
							return (isConnected);
						}

						String read(RJINT bufferSize);
						RJULONG write(String buffer, RJINT bufferSize = -1);
						void close();

						String portName;
						RJINT baud;
						RJINT byteSize;
						RJINT stopBits;
						RJINT parity;
						RJBOOL isConnected;

						#ifdef WIN32
							HANDLE hSerialIO;
						#endif

						static const int oneStopBit;
						static const int one5StopBits;
						static const int twoStopBits;

						static const int noParity;
						static const int oddParity;
						static const int evenParity;
						static const int markParity;
						static const int spaceParity;
				};

				class RADJAV_EXPORT TextFile
				{
					public:
						static const RJINT read;
						static const RJINT write;
						static const RJINT append;

						static void writeTextToFile(String path, String contents, RJINT outputType = IO::TextFile::write);
						static String readEntireFile(String path);

						static void writeToTextFile(String path, String contents, RJINT outputType = IO::TextFile::write);
						static String getFileContents(String path);
				};
		};
	}
}
#endif

