/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#ifndef _RADJAV_V8_IO_H_
	#define _RADJAV_V8_IO_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#ifdef USE_V8
		#include <v8.h>

		namespace RadJAV
		{
			namespace V8B
			{
				namespace IO
				{
					class RADJAV_EXPORT FileIO
					{
					public:
						static void createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object);
						static void _init(const v8::FunctionCallbackInfo<v8::Value> &args);

						static void isDir(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void isFile(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void isSymLink(const v8::FunctionCallbackInfo<v8::Value> &args);

						static void currentPath(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void changePath(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void exists(const v8::FunctionCallbackInfo<v8::Value> &args);

						static void createDir(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void copyDir(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void renameDir(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void deleteDir(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void isEmpty(const v8::FunctionCallbackInfo<v8::Value> &args);

						static void createSymLink(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void copySymLink(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void renameSymLink(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void deleteSymLink(const v8::FunctionCallbackInfo<v8::Value> &args);

						static void copyFile(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void renameFile(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void deleteFile(const v8::FunctionCallbackInfo<v8::Value> &args);

						static void listFiles(const v8::FunctionCallbackInfo<v8::Value> &args);
						static void listFilesAsync(const v8::FunctionCallbackInfo<v8::Value> &args);

						static void normalizePath(const v8::FunctionCallbackInfo<v8::Value> &args);

						static void onFileList(const v8::FunctionCallbackInfo<v8::Value> &args);
					};

					class RADJAV_EXPORT SerialComm
					{
						public:
							static void createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object);
							static void _init(const v8::FunctionCallbackInfo<v8::Value> &args);

							static void open(const v8::FunctionCallbackInfo<v8::Value> &args);
							static void isOpen(const v8::FunctionCallbackInfo<v8::Value> &args);
							static void read(const v8::FunctionCallbackInfo<v8::Value> &args);
							static void write(const v8::FunctionCallbackInfo<v8::Value> &args);
							static void close(const v8::FunctionCallbackInfo<v8::Value> &args);
					};

					class RADJAV_EXPORT TextFile
					{
						public:
							static void createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object);
							static void _init(const v8::FunctionCallbackInfo<v8::Value> &args);

							static void writeFile(const v8::FunctionCallbackInfo<v8::Value> &args);
							static void writeFileAsync(const v8::FunctionCallbackInfo<v8::Value> &args);

							static void readFile(const v8::FunctionCallbackInfo<v8::Value> &args);
							static void readFileAsync(const v8::FunctionCallbackInfo<v8::Value> &args);

							static void onFileRead(const v8::FunctionCallbackInfo<v8::Value> &args);
					};
				};
			}
		}
	#endif
#endif
