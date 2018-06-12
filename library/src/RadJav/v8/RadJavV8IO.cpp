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
		void IO::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "isDir", IO::isDir);
			V8_CALLBACK(object, "isFile", IO::isFile);
			V8_CALLBACK(object, "mkdir", IO::createDir);
			V8_CALLBACK(object, "deleteFile", IO::deleteFile);
			V8_CALLBACK(object, "copyFile", IO::copyFile);
			V8_CALLBACK(object, "copyDir", IO::copyDir);
			V8_CALLBACK(object, "listFiles", IO::listFiles);
			V8_CALLBACK(object, "normalizePath", IO::normalizePath);
		}

		void IO::SerialComm::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "getPort", IO::SerialComm::getPort);
			V8_CALLBACK(object, "getBaud", IO::SerialComm::getBaud);
			V8_CALLBACK(object, "getByteSize", IO::SerialComm::getByteSize);
			V8_CALLBACK(object, "getStopBits", IO::SerialComm::getStopBits);
			V8_CALLBACK(object, "getParity", IO::SerialComm::getParity);
			V8_CALLBACK(object, "open", IO::SerialComm::open);
			V8_CALLBACK(object, "isOpen", IO::SerialComm::isOpen);
			V8_CALLBACK(object, "read", IO::SerialComm::read);
			V8_CALLBACK(object, "write", IO::SerialComm::write);
			V8_CALLBACK(object, "close", IO::SerialComm::close);
		}

		void IO::TextFile::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "writeTextToFile", IO::TextFile::writeTextToFile);
			V8_CALLBACK(object, "readEntireFile", IO::TextFile::readEntireFile);
		}

		void IO::SerialComm::getPort(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			args.GetReturnValue().Set(args.This()->Get(String("_port").toV8String(args.GetIsolate())));
		}

		void IO::SerialComm::getBaud(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			args.GetReturnValue().Set(args.This()->Get(String("_baud").toV8String(args.GetIsolate())));
		}

		void IO::SerialComm::getByteSize(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			args.GetReturnValue().Set(args.This()->Get(String("_byteSize").toV8String(args.GetIsolate())));
		}

		void IO::SerialComm::getStopBits(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			args.GetReturnValue().Set(args.This()->Get(String("_stopBits").toV8String(args.GetIsolate())));
		}

		void IO::SerialComm::getParity(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			args.GetReturnValue().Set(args.This()->Get(String("_parity").toV8String(args.GetIsolate())));
		}

		void IO::SerialComm::open(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());
			v8::Local<v8::String> port = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::Integer> baud = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
			v8::Local<v8::Integer> byteSize = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
			v8::Local<v8::Integer> stopBits = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 3));
			v8::Local<v8::Integer> parity = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 4));

			if (baud.IsEmpty() == true)
				baud = v8::Local<v8::Integer>::Cast(obj->Get(String("_baud").toV8String(args.GetIsolate())));

			if (byteSize.IsEmpty() == true)
				byteSize = v8::Local<v8::Integer>::Cast(obj->Get(String("_byteSize").toV8String(args.GetIsolate())));

			if (stopBits.IsEmpty() == true)
				stopBits = v8::Local<v8::Integer>::Cast(obj->Get(String("_stopBits").toV8String(args.GetIsolate())));

			if (parity.IsEmpty() == true)
				parity = v8::Local<v8::Integer>::Cast(obj->Get(String("_parity").toV8String(args.GetIsolate())));

			obj->Set(String("_port").toV8String(args.GetIsolate()), port);
			obj->Set(String("_baud").toV8String(args.GetIsolate()), baud);
			obj->Set(String("_byteSize").toV8String(args.GetIsolate()), byteSize);
			obj->Set(String("_stopBits").toV8String(args.GetIsolate()), stopBits);
			obj->Set(String("_parity").toV8String(args.GetIsolate()), parity);

			RJBOOL isConnected = false;

#ifdef WIN32
			String portName = parseV8Value(port);
			RJINT baudI = baud->IntegerValue();
			RJINT byteSizeI = byteSize->IntegerValue();
			RJINT stopBitsI = stopBits->IntegerValue();
			RJINT parityI = parity->IntegerValue();

			HANDLE hSerialIO = CreateFileA(portName.c_str(), (GENERIC_READ | GENERIC_WRITE), 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

			if (hSerialIO == INVALID_HANDLE_VALUE)
			{
				CloseHandle(hSerialIO);
				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), false));

				return;
			}

			DCB dcbSerialParams;

			if (GetCommState(hSerialIO, &dcbSerialParams) == false)
			{
				CloseHandle(hSerialIO);
				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), false));

				return;
			}

			dcbSerialParams.BaudRate = baudI;
			dcbSerialParams.ByteSize = byteSizeI;
			dcbSerialParams.StopBits = stopBitsI;
			dcbSerialParams.Parity = parityI;

			if (SetCommState(hSerialIO, &dcbSerialParams) == false)
			{
				CloseHandle(hSerialIO);
				args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), false));

				return;
			}

			isConnected = true;
			V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", hSerialIO);
#endif

			args.GetReturnValue().Set(v8::Boolean::New(args.GetIsolate(), isConnected));
		}

		void IO::SerialComm::isOpen(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());

			args.GetReturnValue().Set(obj->Get(String("_isOpen").toV8String(args.GetIsolate())));
		}

		void IO::SerialComm::read(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Integer> bufferSizeV8 = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());
			String returnValue = "";
			RJUINT bufferSize = bufferSizeV8->IntegerValue();

#ifdef WIN32
			HANDLE hSerialIO = (HANDLE)V8_JAVASCRIPT_ENGINE->v8GetExternal(obj, "_appObj");
			DWORD dErrors = 0;
			DWORD dNumBytesRead = 0;
			COMSTAT cStatus;

			ClearCommError(hSerialIO, &dErrors, &cStatus);

			if (cStatus.cbInQue > 0)
			{
				unsigned int iSize = 0;

				if (cStatus.cbInQue > bufferSize)
					iSize = bufferSize;
				else
					iSize = cStatus.cbInQue;

				char *cBuffer = new char[iSize + 1];
				if (ReadFile(hSerialIO, cBuffer, iSize, &dNumBytesRead, 0) == false)
				{
					delete[]cBuffer;
					cBuffer = 0;

					args.GetReturnValue().Set(String("").toV8String(args.GetIsolate()));

					return;
				}

				if (dNumBytesRead == 0)
				{
					delete[]cBuffer;
					cBuffer = 0;

					args.GetReturnValue().Set(String("").toV8String(args.GetIsolate()));

					return;
				}

				cBuffer[iSize] = '\0';
				returnValue = cBuffer;

				delete[]cBuffer;
				cBuffer = 0;
			}
#endif

			args.GetReturnValue().Set(returnValue.toV8String(args.GetIsolate()));
		}

		void IO::SerialComm::write(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());
			v8::Local<v8::String> bufferV8 = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			RJBOOL result = false;
			RJUINT bufferSize = 0;
			RJULONG bytesSent = 0;

#ifdef WIN32
			HANDLE hSerialIO = (HANDLE)V8_JAVASCRIPT_ENGINE->v8GetExternal(obj, "_appObj");
			String buffer = parseV8Value(bufferV8);
			char *cBuffer = const_cast<char *> (buffer.c_str());

			if (args.Length() > 1)
			{
				v8::Local<v8::Integer> bufSize = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
				bufferSize = bufSize->IntegerValue();
			}
			else
				bufferSize = buffer.length();

			result = WriteFile(hSerialIO, (void *)cBuffer, bufferSize, &bytesSent, 0);
#endif

			args.GetReturnValue().Set(v8::Number::New(args.GetIsolate(), bytesSent));
		}

		void IO::SerialComm::close(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast(args.This());

#ifdef WIN32
			HANDLE hSerialIO = (HANDLE)V8_JAVASCRIPT_ENGINE->v8GetExternal(obj, "_appObj");
			CloseHandle(hSerialIO);
#endif
		}

		void IO::TextFile::writeTextToFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> str = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(str) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");

				return;
			}

			v8::Local<v8::String> text = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
			RJINT fileType = CPP::IO::TextFile::write;

			if (args.Length() > 2)
			{
				v8::Local<v8::Integer> type = v8::Local<v8::Integer>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				fileType = type->Value();
			}

			String path = parseV8Value(str);
			String contents = parseV8Value(text);

			try
			{
				CPP::IO::TextFile::writeToTextFile(path, contents, fileType);
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());

				return;
			}
		}

		void IO::TextFile::readEntireFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> str = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(str) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");

				return;
			}

			String path = parseV8Value(str);
			String contents = "";

			try
			{
				contents = CPP::IO::TextFile::getFileContents(path);
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());

				return;
			}

			args.GetReturnValue().Set(contents.toV8String(V8_JAVASCRIPT_ENGINE->isolate));
		}

		void IO::isDir(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> str = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(str) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");

				return;
			}

			String path = parseV8Value(str);
			RJBOOL exists = false;

#ifdef GUI_USE_WXWIDGETS
			exists = wxDirExists(path.towxString());
#endif

			v8::Local<v8::Boolean> result = v8::Boolean::New(args.GetIsolate(), exists);

			args.GetReturnValue().Set(result);
		}

		void IO::isFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> str = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(str) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");

				return;
			}

			String path = parseV8Value(str);
			RJBOOL exists = false;

#ifdef GUI_USE_WXWIDGETS
			exists = wxFileExists(path.towxString());
#endif

			v8::Local<v8::Boolean> result = v8::Boolean::New(args.GetIsolate(), exists);

			args.GetReturnValue().Set(result);
		}

		void IO::createDir(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> str = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(str) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Path cannot be null!");

				return;
			}

			String path = parseV8Value(str);

#ifdef GUI_USE_WXWIDGETS
			wxMkDir(path.towxString(), wxS_DIR_DEFAULT);
			//if (wxMkDir(path.towxString ()) == false)
				//V8_JAVASCRIPT_ENGINE->throwException("Unable to create directory.");
#endif
		}

		void IO::deleteFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> str = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(str) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");

				return;
			}

			String path = parseV8Value(str);

#ifdef GUI_USE_WXWIDGETS
			wxRemoveFile(path.towxString());
			//if (wxRemoveFile(path.towxString()) == false)
				//V8_JAVASCRIPT_ENGINE->throwException("Unable to delete file.");
#endif
		}

		void IO::copyFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
			RJBOOL overwriteIfExists = true;
			
			if (args.Length () > 2)
			{
				v8::Local<v8::Boolean> overwriteIfExistsV8 = v8::Local<v8::Boolean>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				overwriteIfExists = overwriteIfExistsV8->Value();
			}

			CPP::IO::copyFile(parseV8Value(src), parseV8Value(dest), overwriteIfExists);
		}

		void IO::copyDir(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> src = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			v8::Local<v8::String> dest = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
			RJBOOL isRecursive = true;
			
			if (args.Length () > 2)
			{
				v8::Local<v8::Boolean> recursive = v8::Local<v8::Boolean>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				isRecursive = recursive->Value();
			}

			CPP::IO::copyDir(parseV8Value(src), parseV8Value(dest), isRecursive);
		}

		void IO::listFiles(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			RJBOOL isRecursive = true;

			if (args.Length () > 1)
			{
				v8::Local<v8::Boolean> recursive = v8::Local<v8::Boolean>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
				isRecursive = recursive->Value();
			}

			Array<String> files = CPP::IO::listFiles(parseV8Value(path), isRecursive);
			v8::Local<v8::Array> newFiles = convertArrayToV8Array(files, args.GetIsolate ());

			args.GetReturnValue().Set(newFiles);
		}

		void IO::normalizePath(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
			String basePath = "";

			if (args.Length() > 1)
			{
				v8::Local<v8::String> basePathV8 = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
				basePath = parseV8Value(basePathV8);
			}

			path = CPP::IO::normalizePath(parseV8Value(path), basePath).toV8String (args.GetIsolate ());

			args.GetReturnValue().Set(path);
		}
	}
}
#endif

