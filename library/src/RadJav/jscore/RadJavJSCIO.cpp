/*
	MIT-LICENSE
	Copyright (c) 2018 Higher Edge Software, LLC

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
#include "jscore/RadJavJSCIO.h"

#include "RadJav.h"
#include "RadJavString.h"

#include "jscore/RadJavJSCJavascriptEngine.h"
#include "cpp/RadJavCPPIO.h"

namespace RadJAV
{
	namespace JSC
	{
		void IO::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
			JSC_CALLBACK(object, "isDir", IO::isDir);
			JSC_CALLBACK(object, "isFile", IO::isFile);
			JSC_CALLBACK(object, "isSymLink", IO::isSymLink);

			JSC_CALLBACK(object, "currentPath", IO::currentPath);
			JSC_CALLBACK(object, "changePath", IO::changePath);
			JSC_CALLBACK(object, "exists", IO::exists);

			JSC_CALLBACK(object, "mkdir", IO::createDir);
			JSC_CALLBACK(object, "copyDir", IO::copyDir);
			JSC_CALLBACK(object, "renameDir", IO::renameDir);
			JSC_CALLBACK(object, "deleteDir", IO::deleteDir);
			JSC_CALLBACK(object, "isEmpty", IO::isEmpty);

			JSC_CALLBACK(object, "createSymLink", IO::createSymLink);
			JSC_CALLBACK(object, "copySymLink", IO::copySymLink);
			JSC_CALLBACK(object, "renameSymLink", IO::renameSymLink);
			JSC_CALLBACK(object, "deleteSymLink", IO::deleteSymLink);

			JSC_CALLBACK(object, "copyFile", IO::copyFile);
			JSC_CALLBACK(object, "renameFile", IO::renameFile);
			JSC_CALLBACK(object, "deleteFile", IO::deleteFile);

			JSC_CALLBACK(object, "listFiles", IO::listFiles);
			JSC_CALLBACK(object, "listFilesAsync", IO::listFilesAsync);

			JSC_CALLBACK(object, "normalizePath", IO::normalizePath);
			JSC_CALLBACK(object, "normalizeAndVerifyPath", IO::normalizeAndVerifyPath);
			JSC_CALLBACK(object, "normalizeCurrentPath", IO::normalizeCurrentPath);
			
			JSC_CALLBACK(object, "onFileList", IO::onFileList);
		}

		void IO::SerialComm::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
			JSC_CALLBACK(object, "open", IO::SerialComm::open);
			JSC_CALLBACK(object, "isOpen", IO::SerialComm::isOpen);
			JSC_CALLBACK(object, "read", IO::SerialComm::read);
			JSC_CALLBACK(object, "write", IO::SerialComm::write);
			JSC_CALLBACK(object, "close", IO::SerialComm::close);
		}

		void IO::StreamFile::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
			JSC_CALLBACK(object, "writeStream", IO::StreamFile::writeStream);
			JSC_CALLBACK(object, "writeStreamAsync", IO::StreamFile::writeStreamAsync);

			JSC_CALLBACK(object, "readStream", IO::StreamFile::readStream);
			JSC_CALLBACK(object, "readStreamAsync", IO::StreamFile::readStreamAsync);
		}

		void IO::TextFile::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
			JSC_CALLBACK(object, "writeFile", IO::TextFile::writeFile);
			JSC_CALLBACK(object, "writeFileAsync", IO::TextFile::writeFileAsync);

			JSC_CALLBACK(object, "readFile", IO::TextFile::readFile);
			JSC_CALLBACK(object, "readFileAsync", IO::TextFile::readFileAsync);
		}

		// ###################################################################


		JSValueRef IO::isFile(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return JSValueMakeUndefined(ctx);
			}

			RJBOOL exists = CPP::IO::isFile(parseJSCValue(ctx, arguments[0]));

			return JSValueMakeBoolean(ctx, exists);
		}

		JSValueRef IO::isDir(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return JSValueMakeUndefined(ctx);
			}

			RJBOOL exists = CPP::IO::isDir(parseJSCValue(ctx, arguments[0]));

			return JSValueMakeBoolean(ctx, exists);
		}

		JSValueRef IO::isSymLink(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return JSValueMakeUndefined(ctx);
			}

			RJBOOL exists = CPP::IO::isSymLink(parseJSCValue(ctx, arguments[0]));

			return JSValueMakeBoolean(ctx, exists);
		}

		JSValueRef IO::currentPath(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSStringRef path = CPP::IO::currentPath().toJSCString();

			JSValueRef pathObject = JSValueMakeString(ctx, path);
			JSStringRelease(path);
			
			return pathObject;
		}

		JSValueRef IO::changePath(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return undefined;
			}

			CPP::IO::changePath(parseJSCValue(ctx, arguments[0]));
			
			return undefined;
		}

		JSValueRef IO::exists(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return JSValueMakeUndefined(ctx);
			}

			RJBOOL exists = CPP::IO::exists(parseJSCValue(ctx, arguments[0]));

			return JSValueMakeBoolean(ctx, exists);
		}

		JSValueRef IO::createDir(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return undefined;
			}

			CPP::IO::createDir(parseJSCValue(ctx, arguments[0]));
			
			return undefined;
		}

		JSValueRef IO::copyDir(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[1]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return undefined;
			}

			RJBOOL isRecursive = true;
			if (argumentCount > 2)
			{
				isRecursive = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(ctx, arguments[2]);
			}

			CPP::IO::copyDir(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]), isRecursive);
			
			return undefined;
		}

		JSValueRef IO::renameDir(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]) ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[1]))
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return undefined;
			}
			
			RJBOOL isRecursive = true;
			if (argumentCount > 2)
			{
				isRecursive = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(ctx, arguments[2]);
			}

			CPP::IO::copyDir(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]), isRecursive);
			
			return undefined;
		}

		JSValueRef IO::deleteDir(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return undefined;
			}

			CPP::IO::deleteDir(parseJSCValue(ctx, arguments[0]));

			return undefined;
		}

		JSValueRef IO::isEmpty(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return JSValueMakeUndefined(ctx);
			}

			RJBOOL empty = CPP::IO::isEmpty(parseJSCValue(ctx, arguments[0]));

			return JSValueMakeBoolean(ctx, empty);
		}

		JSValueRef IO::createSymLink(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[1]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return undefined;
			}

			CPP::IO::createSymLink(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]));
			
			return undefined;
		}

		JSValueRef IO::copySymLink(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[1]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return undefined;
			}

			CPP::IO::copySymLink(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]));
			
			return undefined;
		}

		JSValueRef IO::renameSymLink(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[1]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return undefined;
			}

			CPP::IO::renameSymLink(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]));
			
			return undefined;
		}

		JSValueRef IO::deleteSymLink(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);

			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return undefined;
			}

			CPP::IO::deleteSymLink(parseJSCValue(ctx, arguments[0]));
			
			return undefined;
		}

		JSValueRef IO::copyFile(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);

			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[1]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return undefined;
			}

			RJBOOL overwriteIfExists = true;
			if (argumentCount > 2)
			{
				overwriteIfExists =JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(ctx, arguments[2]);
			}

			CPP::IO::copyFile(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]), overwriteIfExists);
			
			return undefined;
		}

		JSValueRef IO::renameFile(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);

			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[1]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return undefined;
			}

			CPP::IO::renameFile(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]));
			
			return undefined;
		}

		JSValueRef IO::deleteFile(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);

			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return undefined;
			}

			CPP::IO::deleteFile(parseJSCValue(ctx, arguments[0]));
			
			return undefined;
		}

		JSValueRef IO::listFiles(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return undefined;
			}

			RJBOOL isRecursive = true;
			if (argumentCount > 1)
			{
				isRecursive = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(ctx, arguments[1]);
			}

			Array<String> files = CPP::IO::listFiles(parseJSCValue(ctx, arguments[0]), isRecursive);
			JSObjectRef newFiles = convertArrayToJSCArray(files, ctx);

			return newFiles;
		}

		JSValueRef IO::listFilesAsync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]) ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[2]))
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Callback and Path cannot be null!");
				return undefined;
			}
			
			JSObjectRef asyncCallback = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
			JSValueRef path = arguments[1];
			
			RJBOOL isRecursive = true;

			if (argumentCount > 2)
			{
				isRecursive = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(ctx, arguments[2]);
			}

			CPP::IO::listFilesAsync([asyncCallback, ctx, thisObject](String newFileOrDir){
							JSStringRef fileOrDir = newFileOrDir.toJSCString();
							JSValueRef strs[1];
							strs[0] = JSValueMakeString(ctx, fileOrDir);
				
							JSStringRelease(fileOrDir);

							JSValueRef result = JSObjectCallAsFunction(ctx, asyncCallback, thisObject, 1, strs, nullptr);

							if (JSC_JAVASCRIPT_ENGINE->jscIsNull(result))
							{
								return true;
							}

							return JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(ctx, result);
					}, parseJSCValue(ctx, path), isRecursive);
			
			return undefined;
		}

		JSValueRef IO::normalizePath(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return JSValueMakeUndefined(ctx);
			}
			
			String path = parseJSCValue(ctx, arguments[0]);
			String basePath = "";

			if (argumentCount > 1)
			{
				basePath = parseJSCValue(ctx, arguments[1]);
			}

			JSStringRef updatedPath = CPP::IO::normalizePath( path, basePath).toJSCString();
			JSValueRef result = JSValueMakeString(ctx, updatedPath);
			JSStringRelease(updatedPath);
			
			return result;
		}

		JSValueRef IO::normalizeAndVerifyPath(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return JSValueMakeUndefined(ctx);
			}

			String path = parseJSCValue(ctx, arguments[0]);
			String basePath = "";

			if (argumentCount > 1)
			{
				basePath = parseJSCValue(ctx, arguments[1]);
			}

			JSStringRef updatedPath = CPP::IO::normalizeAndVerifyPath(path, basePath).toJSCString();
			JSValueRef result = JSValueMakeString(ctx, updatedPath);
			JSStringRelease(updatedPath);
			
			return result;
		}

		JSValueRef IO::normalizeCurrentPath(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return JSValueMakeUndefined(ctx);
			}
			
			JSStringRef updatedPath = CPP::IO::normalizeCurrentPath(parseJSCValue(ctx, arguments[0])).toJSCString();
			JSValueRef result = JSValueMakeString(ctx, updatedPath);
			JSStringRelease(updatedPath);
			
			return result;
		}

		JSValueRef IO::onFileList(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			/* TODO: add implementation for JavaScriptCore
			v8::Persistent<v8::Function> *func = RJNEW v8::Persistent<v8::Function>();
			v8::Local<v8::Function> newEvt = v8::Local<v8::Function>::Cast(args[0]);

			func->Reset(JSC_JAVASCRIPT_ENGINE->isolate, newEvt);

			//RadJAV::CPP::IO::m_fileListEvent = func;
			 */
			
			return JSValueMakeUndefined(ctx);
		}


		// ###################################################################


		JSValueRef IO::SerialComm::open(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Port cannot be null!");
				return JSValueMakeUndefined(ctx);
			}
			
			String port = parseJSCValue(ctx, arguments[0]);
			
			RJINT baud_val = 115200;
			if (argumentCount > 1)
			{
				 baud_val = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, arguments[1]);
			}

			RJBOOL isConnected;
			isConnected = RadJAV::CPP::IO::SerialComm::open(port, baud_val);

			return JSValueMakeBoolean(ctx,  isConnected);
		}

		JSValueRef IO::SerialComm::isOpen(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			RJBOOL isOpen = RadJAV::CPP::IO::SerialComm::isOpen();
			return JSValueMakeBoolean(ctx, isOpen);
		}

		JSValueRef IO::SerialComm::read(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			//TODO: Add usage for bufferSize
			RJUINT bufferSize = 0;
			
			if (argumentCount > 0)
			{
				bufferSize = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, arguments[0]);
			}
			
			String data = RadJAV::CPP::IO::SerialComm::read();
			JSStringRef str = data.toJSCString();
			JSValueRef strObject = JSValueMakeString(ctx, str);
			JSStringRelease(str);
			
			return strObject;
		}

		JSValueRef IO::SerialComm::write(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			if (argumentCount == 0)
			{
				return JSValueMakeNumber(ctx, 0.0);
			}
			
			String buffer = parseJSCValue(ctx, arguments[0]);

			RJULONG bytesSent = RadJAV::CPP::IO::SerialComm::write(buffer, buffer.size());

			return JSValueMakeNumber(ctx, bytesSent);
		}

		JSValueRef IO::SerialComm::close(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			RadJAV::CPP::IO::SerialComm::close();
			
			return JSValueMakeUndefined(ctx);
		}


		// ###################################################################


		JSValueRef IO::StreamFile::writeStream(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return undefined;
			}

			/* TODO: add JavaScriptCore implementation
			auto view = v8::Local<v8::ArrayBufferView>::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 1));
			auto data = view->Buffer();
			RJINT fileType = static_cast<int>(CPP::IO::StreamFile::operation::write);

			if (argumentCount > 2)
			{
				v8::Local<v8::Integer> type = v8::Local<v8::Integer>::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 2));
				fileType = type->Value();
			}

			try
			{
				CPP::IO::StreamFile::writeStream(parseJSCValue(ctx, path), data, fileType);
			}
			catch (Exception ex)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return undefined;
			}*/
			
			return undefined;
		}

		JSValueRef IO::StreamFile::writeStreamAsync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);

			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return undefined;
			}

			/* TODO: add JavaScriptCore implementation
			auto data = v8::Local<v8::ArrayBuffer>::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 1));
			RJINT fileType = static_cast<int>(CPP::IO::TextFile::operation::write);

			if (argumentCount > 2)
			{
				v8::Local<v8::Integer> type = v8::Local<v8::Integer>::Cast(JSC_JAVASCRIPT_ENGINE->jscGetArgument(args, 2));
				fileType = type->Value();
			}

			try
			{
				CPP::IO::StreamFile::writeStreamAsync(parseJSCValue(ctx, path), data, fileType);
			}
			catch (Exception ex)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return undefined;
			}
			 */
			
			return undefined;
		}

		JSValueRef IO::StreamFile::readStream(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);

			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return undefined;
			}

			/* TODO: add JavaScriptCore implementation
			try
			{
				auto contents = CPP::IO::StreamFile::readStream(parseJSCValue(ctx, arguments[0]));
				auto ab = v8::ArrayBuffer::New(JSC_JAVASCRIPT_ENGINE->isolate, (void*)contents.c_str(), contents.size());

				return JSValueMake(ctx, ab);
			}
			catch (Exception ex)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
			}
			*/
			
			return undefined;
		}

		JSValueRef IO::StreamFile::readStreamAsync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return undefined;
			}

			/* TODO: add JavaScriptCore implementation
			try
			{
				CPP::IO::StreamFile::readStreamAsync(parseJSCValue(ctx, path));
			}
			catch (Exception ex)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return undefined;
			}
			 */
			
			return undefined;
		}

		JSValueRef IO::StreamFile::onFileRead(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
			{
				return undefined;
			}
			
			JSObjectRef functionObject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
			if (JSObjectIsFunction(ctx, functionObject))
			{
				JSValueProtect(ctx, arguments[0]);
				RadJAV::CPP::IO::StreamFile::m_streamfileReadEvent = functionObject;
			}
			
			return undefined;
		}


		// ###################################################################


		JSValueRef IO::TextFile::writeFile(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]) == true ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[1]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename and text cannot be null!");

				return undefined;
			}

			RJINT fileType = static_cast<int>(CPP::IO::TextFile::operation::write);

			if (argumentCount > 2)
			{
				fileType = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, arguments[2]);
			}

			try
			{
				CPP::IO::TextFile::writeFile(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]), fileType);
			}
			catch (Exception ex)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
			}
			
			return undefined;
		}

		JSValueRef IO::TextFile::writeFileAsync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount < 2 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]) == true ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[1]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename and text cannot be null!");
				
				return undefined;
			}
			
			RJINT fileType = static_cast<int>(CPP::IO::TextFile::operation::write);
			
			if (argumentCount > 2)
			{
				fileType = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, arguments[2]);
			}
			
			try
			{
				CPP::IO::TextFile::writeFileAsync(parseJSCValue(ctx, arguments[0]), parseJSCValue(ctx, arguments[1]), fileType);
			}
			catch (Exception ex)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
			}

			return undefined;
		}

		JSValueRef IO::TextFile::readFile(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return undefined;
			}

			String contents;
			try
			{
				contents = CPP::IO::TextFile::readFile(parseJSCValue(ctx, arguments[0]));
			}
			catch (Exception ex)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return undefined;
			}
			
			JSStringRef contentStr = contents.toJSCString();
			JSValueRef contentObject = JSValueMakeString(ctx, contentStr);
			JSStringRelease(contentStr);
			
			return contentObject;
		}

		JSValueRef IO::TextFile::readFileAsync(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]) == true)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return undefined;
			}

			try
			{
				CPP::IO::TextFile::readFileAsync(parseJSCValue(ctx, arguments[0]));
			}
			catch (Exception ex)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
			}

			return undefined;
		}

		JSValueRef IO::TextFile::onFileRead(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			
			if (argumentCount == 0 ||
				JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
			{
				return undefined;
			}
			
			JSObjectRef functionObject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
			if (JSObjectIsFunction(ctx, functionObject))
			{
				JSValueProtect(ctx, arguments[0]);
				RadJAV::CPP::IO::TextFile::m_textfileReadEvent = functionObject;
			}
			
			return undefined;
		}
	}
}
