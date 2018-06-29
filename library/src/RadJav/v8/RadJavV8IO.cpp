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
#include "v8/RadJavV8IO.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"

	#include "cpp/RadJavCPPIO.h"

namespace RadJAV
{
	namespace V8B
	{
		void IO::FileIO::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "isDir", IO::FileIO::isDir);
			V8_CALLBACK(object, "isFile", IO::FileIO::isFile);
			V8_CALLBACK(object, "isSymLink", IO::FileIO::isSymLink);

			V8_CALLBACK(object, "currentPath", IO::FileIO::currentPath);
			V8_CALLBACK(object, "changePath", IO::FileIO::changePath);
			V8_CALLBACK(object, "exists", IO::FileIO::exists);

			V8_CALLBACK(object, "mkdir", IO::FileIO::createDir);
			V8_CALLBACK(object, "copyDir", IO::FileIO::copyDir);
			V8_CALLBACK(object, "renameDir", IO::FileIO::renameDir);
			V8_CALLBACK(object, "deleteDir", IO::FileIO::deleteDir);
			V8_CALLBACK(object, "isEmpty", IO::FileIO::isEmpty);

			V8_CALLBACK(object, "createSymLink", IO::FileIO::createSymLink);
			V8_CALLBACK(object, "copySymLink", IO::FileIO::copySymLink);
			V8_CALLBACK(object, "renameSymLink", IO::FileIO::renameSymLink);
			V8_CALLBACK(object, "deleteSymLink", IO::FileIO::deleteSymLink);

			V8_CALLBACK(object, "copyFile", IO::FileIO::copyFile);
			V8_CALLBACK(object, "renameFile", IO::FileIO::renameFile);
			V8_CALLBACK(object, "deleteFile", IO::FileIO::deleteFile);

			V8_CALLBACK(object, "listFiles", IO::FileIO::listFiles);
			V8_CALLBACK(object, "listFilesAsync", IO::FileIO::listFilesAsync);

			V8_CALLBACK(object, "normalizePath", IO::FileIO::normalizePath);
			
			V8_CALLBACK(object, "onFileList", IO::FileIO::onFileList);
		}

		void IO::FileIO::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			CPP::IO::FileIO *io = RJNEW CPP::IO::FileIO();
			V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_fileio", io);
		}

		void IO::SerialComm::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "open", IO::SerialComm::open);
			V8_CALLBACK(object, "isOpen", IO::SerialComm::isOpen);
			V8_CALLBACK(object, "read", IO::SerialComm::read);
			V8_CALLBACK(object, "write", IO::SerialComm::write);
			V8_CALLBACK(object, "close", IO::SerialComm::close);
		}

		void IO::SerialComm::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			CPP::IO::SerialComm *SerialComm = RJNEW CPP::IO::SerialComm();
			V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_SerialComm", SerialComm);
		}

		void IO::TextFile::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "writeFile", IO::TextFile::writeFile);
			V8_CALLBACK(object, "writeFileAsync", IO::TextFile::writeFileAsync);

			V8_CALLBACK(object, "readFile", IO::TextFile::readFile);
			V8_CALLBACK(object, "readFileAsync", IO::TextFile::readFileAsync);
		}

		void IO::TextFile::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			CPP::IO::TextFile *TextFile = RJNEW CPP::IO::TextFile();
			V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_TextFile", TextFile);
		}


		// ###################################################################


		void IO::FileIO::isFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			RJBOOL exists = CPP::IO::FileIO::isFile(parseV8Value(path));

			v8::Local<v8::Boolean> result = v8::Boolean::New(args.GetIsolate(), exists);

			args.GetReturnValue().Set(result);
		}

		void IO::FileIO::isDir(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			RJBOOL exists = CPP::IO::FileIO::isDir(parseV8Value(path));

			v8::Local<v8::Boolean> result = v8::Boolean::New(args.GetIsolate(), exists);

			args.GetReturnValue().Set(result);
		}

		void IO::FileIO::isSymLink(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			RJBOOL exists = CPP::IO::FileIO::isSymLink(parseV8Value(path));

			v8::Local<v8::Boolean> result = v8::Boolean::New(args.GetIsolate(), exists);

			args.GetReturnValue().Set(result);
		}

		void IO::FileIO::currentPath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = CPP::IO::FileIO::currentPath().toV8String(args.GetIsolate());

			args.GetReturnValue().Set(path);
		}

		void IO::FileIO::changePath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			CPP::IO::FileIO::changePath(parseV8Value(path));
		}

		void IO::FileIO::exists(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			RJBOOL exists = CPP::IO::FileIO::exists(parseV8Value(path));

			v8::Local<v8::Boolean> result = v8::Boolean::New(args.GetIsolate(), exists);
		}

		void IO::FileIO::createDir(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			CPP::IO::FileIO::createDir(parseV8Value(path));
		}

		void IO::FileIO::copyDir(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(src) == true || V8_JAVASCRIPT_ENGINE->v8IsNull(dest) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return;
			}

			RJBOOL isRecursive = true;
			if (args.Length() > 2)
			{
				v8::Local<v8::Boolean> recursive = v8::Local<v8::Boolean>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				isRecursive = recursive->Value();
			}

			CPP::IO::FileIO::copyDir(parseV8Value(src), parseV8Value(dest), isRecursive);
		}

		void IO::FileIO::renameDir(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
			
			RJBOOL isRecursive = true;
			if (args.Length() > 2)
			{
				v8::Local<v8::Boolean> recursive = v8::Local<v8::Boolean>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				isRecursive = recursive->Value();
			}

			CPP::IO::FileIO::copyDir(parseV8Value(src), parseV8Value(dest), isRecursive);
		}

		void IO::FileIO::deleteDir(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			CPP::IO::FileIO::deleteDir(parseV8Value(path));

		}

		void IO::FileIO::isEmpty(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			RJBOOL empty = CPP::IO::FileIO::isEmpty(parseV8Value(path));

			v8::Local<v8::Boolean> result = v8::Boolean::New(args.GetIsolate(), empty);
		}

		void IO::FileIO::createSymLink(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(src) == true || V8_JAVASCRIPT_ENGINE->v8IsNull(dest) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return;
			}

			CPP::IO::FileIO::createSymLink(parseV8Value(src), parseV8Value(dest));
		}

		void IO::FileIO::copySymLink(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(src) == true || V8_JAVASCRIPT_ENGINE->v8IsNull(dest) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return;
			}

			CPP::IO::FileIO::copySymLink(parseV8Value(src), parseV8Value(dest));
		}

		void IO::FileIO::renameSymLink(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(src) == true || V8_JAVASCRIPT_ENGINE->v8IsNull(dest) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return;
			}

			CPP::IO::FileIO::renameSymLink(parseV8Value(src), parseV8Value(dest));
		}

		void IO::FileIO::deleteSymLink(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");
				return;
			}

			CPP::IO::FileIO::deleteSymLink(parseV8Value(path));
		}

		void IO::FileIO::copyFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(src) == true || V8_JAVASCRIPT_ENGINE->v8IsNull(dest) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return;
			}

			RJBOOL overwriteIfExists = true;
			if (args.Length() > 2)
			{
				v8::Local<v8::Boolean> overwriteIfExistsV8 = v8::Local<v8::Boolean>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				overwriteIfExists = overwriteIfExistsV8->Value();
			}

			CPP::IO::FileIO::copyFile(parseV8Value(src), parseV8Value(dest), overwriteIfExists);
		}

		void IO::FileIO::renameFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(src) == true || V8_JAVASCRIPT_ENGINE->v8IsNull(dest) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Source and Destination paths cannot be null!");
				return;
			}

			CPP::IO::FileIO::renameFile(parseV8Value(src), parseV8Value(dest));
		}

		void IO::FileIO::deleteFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return;
			}

			CPP::IO::FileIO::deleteFile(parseV8Value(path));
		}

		void IO::FileIO::listFiles(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			RJBOOL isRecursive = true;
			if (args.Length() > 1)
			{
				v8::Local<v8::Boolean> recursive = v8::Local<v8::Boolean>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
				isRecursive = recursive->Value();
			}

			Array<String> files = CPP::IO::FileIO::listFiles(parseV8Value(path), isRecursive);
			v8::Local<v8::Array> newFiles = convertArrayToV8Array(files, args.GetIsolate());

			args.GetReturnValue().Set(newFiles);
		}

		void IO::FileIO::listFilesAsync(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			RJBOOL isRecursive = true;
			if (args.Length() > 1)
			{
				v8::Local<v8::Boolean> recursive = v8::Local<v8::Boolean>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
				isRecursive = recursive->Value();
			}

			CPP::IO::FileIO::listFilesAsync(parseV8Value(path), isRecursive);
		}

		void IO::FileIO::normalizePath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			String basePath = "";

			if (args.Length() > 1)
			{
				v8::Local<v8::String> basePathV8 = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
				basePath = parseV8Value(basePathV8);
			}

			path = CPP::IO::FileIO::normalizePath(parseV8Value(path), basePath).toV8String (args.GetIsolate ());

			args.GetReturnValue().Set(path);
		}

		void IO::FileIO::onFileList(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Persistent<v8::Function> *func = RJNEW v8::Persistent<v8::Function>();
			v8::Local<v8::Function> newEvt = v8::Local<v8::Function>::Cast(args[0]);

			func->Reset(V8_JAVASCRIPT_ENGINE->isolate, newEvt);

			RadJAV::CPP::IO::FileIO::m_fileListEvent = func;
		}


		// ###################################################################


		void IO::SerialComm::open(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());
			v8::Local<v8::String> port_str = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::Integer> baud_val = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(port_str) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Port cannot be null!");
				return;
			}

			String port = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, port_str);

			RJBOOL isConnected;
			if (baud_val.IsEmpty() == true)
				isConnected = RadJAV::CPP::IO::SerialComm::open(port);
			else
				isConnected = RadJAV::CPP::IO::SerialComm::open(port, baud_val->IntegerValue());

			args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), isConnected));
		}

		void IO::SerialComm::isOpen(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());

			RJBOOL isOpen = RadJAV::CPP::IO::SerialComm::isOpen();
			args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), isOpen));
		}

		void IO::SerialComm::read(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Integer> bufferSizeV8 = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());
			String returnValue;
			RJUINT bufferSize = bufferSizeV8->IntegerValue();

			returnValue = RadJAV::CPP::IO::SerialComm::read();

			args.GetReturnValue().Set(returnValue.toV8String(args.GetIsolate()));
		}

		void IO::SerialComm::write(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());
			v8::Local<v8::String> bufferV8 = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			String buffer = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, bufferV8);

			RJULONG bytesSent = RadJAV::CPP::IO::SerialComm::write(buffer, buffer.size());

			args.GetReturnValue().Set(v8::Number::New(args.GetIsolate(), bytesSent));
		}

		void IO::SerialComm::close(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());

			RadJAV::CPP::IO::SerialComm::close();
		}


		// ###################################################################


		void IO::TextFile::writeFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");

				return;
			}

			v8::Local<v8::String> text = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
			RJINT fileType = static_cast<int>(CPP::IO::TextFile::operation::write);

			if (args.Length() > 2)
			{
				v8::Local<v8::Integer> type = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				fileType = type->Value();
			}

			try
			{
				CPP::IO::TextFile::writeFile(parseV8Value(path), parseV8Value(text), fileType);
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}
		}

		void IO::TextFile::writeFileAsync(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");

				return;
			}

			v8::Local<v8::String> text = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
			RJINT fileType = static_cast<int>(CPP::IO::TextFile::operation::write);

			if (args.Length() > 2)
			{
				v8::Local<v8::Integer> type = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				fileType = type->Value();
			}

			try
			{
				CPP::IO::TextFile::writeFileAsync(parseV8Value(path), parseV8Value(text), fileType);
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}
		}

		void IO::TextFile::readFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return;
			}

			String contents;
			try
			{
				contents = CPP::IO::TextFile::readFile(parseV8Value(path));
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}

			args.GetReturnValue().Set(contents.toV8String(V8_JAVASCRIPT_ENGINE->isolate));
		}

		void IO::TextFile::readFileAsync(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return;
			}

			try
			{
				CPP::IO::TextFile::readFileAsync(parseV8Value(path));
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}
		}

		void IO::TextFile::onFileRead(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Persistent<v8::Function> *func = RJNEW v8::Persistent<v8::Function>();
			v8::Local<v8::Function> newEvt = v8::Local<v8::Function>::Cast(args[0]);

			func->Reset(V8_JAVASCRIPT_ENGINE->isolate, newEvt);

			RadJAV::CPP::IO::TextFile::m_textfileReadEvent = func;
		}
	}
}
#endif

