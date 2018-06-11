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
#include "cpp/RadJavCPPIO.h"

#include "RadJav.h"
#include "RadJavString.h"

#include <fstream>

#ifdef USE_BOOST
	#include <boost/filesystem.hpp>
#endif

namespace RadJAV
{
	namespace CPP
	{
		const int IO::SerialComm::oneStopBit = 0;
		const int IO::SerialComm::one5StopBits = 1;
		const int IO::SerialComm::twoStopBits = 2;

		const int IO::SerialComm::noParity = 0;
		const int IO::SerialComm::oddParity = 1;
		const int IO::SerialComm::evenParity = 2;
		const int IO::SerialComm::markParity = 3;
		const int IO::SerialComm::spaceParity = 4;

		const RJINT IO::TextFile::read = 1;
		const RJINT IO::TextFile::write = 2;
		const RJINT IO::TextFile::append = 3;

		IO::SerialComm::SerialComm()
		{
			portName = "";
			baud = 9600;
			byteSize = 8;
			stopBits = IO::SerialComm::oneStopBit;
			parity = IO::SerialComm::noParity;
		}

		RJBOOL IO::SerialComm::open()
		{
			#ifdef WIN32
				hSerialIO = CreateFileA(portName.c_str(), (GENERIC_READ | GENERIC_WRITE), 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

				if (hSerialIO == INVALID_HANDLE_VALUE)
				{
					CloseHandle(hSerialIO);

					return (false);
				}

				DCB dcbSerialParams;

				if (GetCommState(hSerialIO, &dcbSerialParams) == false)
				{
					CloseHandle(hSerialIO);

					return (false);
				}

				dcbSerialParams.BaudRate = baud;
				dcbSerialParams.ByteSize = byteSize;
				dcbSerialParams.StopBits = stopBits;
				dcbSerialParams.Parity = parity;

				if (SetCommState(hSerialIO, &dcbSerialParams) == false)
				{
					CloseHandle(hSerialIO);

					return (false);
				}

				isConnected = true;
			#endif

			return (true);
		}

		String IO::SerialComm::read(RJINT bufferSize)
		{
			String returnValue = "";

			#ifdef WIN32
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

						return ("");
					}

					if (dNumBytesRead == 0)
					{
						delete[]cBuffer;
						cBuffer = 0;

						return ("");
					}

					cBuffer[iSize] = '\0';
					returnValue = cBuffer;

					delete[]cBuffer;
					cBuffer = 0;
				}
			#endif

			return (returnValue);
		}

		RJULONG IO::SerialComm::write(String buffer, RJINT bufferSize)
		{
			RJBOOL result = false;
			RJUINT bufferSizeI = 0;
			RJULONG bytesSent = 0;

			#ifdef WIN32
				char *cBuffer = const_cast<char *> (buffer.c_str());

				if (bufferSize > -1)
					bufferSizeI = bufferSize;
				else
					bufferSizeI = buffer.length();

				result = WriteFile(hSerialIO, (void *)cBuffer, bufferSizeI, &bytesSent, 0);
			#endif

			return (bytesSent);
		}

		void IO::SerialComm::close()
		{
			#ifdef WIN32
				CloseHandle(hSerialIO);
			#endif
		}

		void IO::TextFile::writeTextToFile(String path, String contents, RJINT outputType)
		{
			writeToTextFile(path, contents, outputType);
		}

		String IO::TextFile::readEntireFile(String path)
		{
			String contents = "";

			contents = getFileContents(path);

			return (contents);
		}

		void IO::TextFile::writeToTextFile(String path, String contents, RJINT outputType)
		{
			RJINT type = std::ios_base::out;

			if (outputType == IO::TextFile::append)
				type = std::ios_base::app;

			std::fstream file(path, type);
			file.write(contents.c_str(), contents.size());
			file.close();
		}

		String IO::TextFile::getFileContents(String path)
		{
			std::fstream file(path, std::ios_base::in);
			String contents = "";

			if (file.is_open() == false)
			{
				throw Exception("Unable to open file: " + path);

				return ("");
			}

			while (file.good() == true)
			{
				char cChar = file.get();

				if (file.good() == false)
					break;

				contents += cChar;
			}

			file.close();

			return (contents);
		}

		void IO::copyFile(String src, String dest, RJBOOL overwriteIfExists)
		{
			try
			{
				boost::filesystem::path srcPath(src.c_str());
				boost::filesystem::path destPath(dest.c_str());

				boost::filesystem::copy_option option = boost::filesystem::copy_option::overwrite_if_exists;

				if (overwriteIfExists == false)
					option = boost::filesystem::copy_option::fail_if_exists;

				boost::filesystem::copy_file(srcPath, destPath / srcPath.filename(), option);
			}
			catch (boost::filesystem::filesystem_error ex)
			{
			}
		}

		void IO::copyDir(String src, String dest, RJBOOL recursive)
		{
			/// @todo We want to add a JavaScript function that will allow the developer to see 
			/// the file or folder that's being copied over and be able to prevent it from being 
			/// copied if necessary. So we would need to use listFiles and go through each file/folder 
			/// and report it back to the JavaScript function and wait for a boolean response that 
			/// would determine if the file/folder gets copied.

			#ifdef USE_BOOST
				try
				{
					boost::filesystem::path destPath(dest.c_str());

					if (boost::filesystem::exists(destPath) == false)
						boost::filesystem::create_directory(destPath);

					boost::filesystem::recursive_directory_iterator it(src.c_str());
					boost::filesystem::recursive_directory_iterator end;

					while (it != end)
					{
						boost::filesystem::path file = it->path();

						if (boost::filesystem::is_directory(file) == true)
						{
							if (recursive == true)
							{
								boost::filesystem::path nextFile = destPath / file.filename();

								copyDir(file.string(), nextFile.string());
							}
						}
						else
							boost::filesystem::copy_file(file, destPath / file.filename());

						it++;
					}
				}
				catch (boost::filesystem::filesystem_error ex)
				{
				}
			#endif
		}

		Array<String> IO::listFiles(String path, RJBOOL recursive)
		{
			Array<String> files;

			#ifdef USE_BOOST
				boost::filesystem::path dirPath(path.c_str());

				if (recursive == true)
				{
					boost::filesystem::recursive_directory_iterator it(dirPath);
					boost::filesystem::recursive_directory_iterator end;

					while (it != end)
					{
						boost::filesystem::path file = it->path();
						files.push_back(file.string());

						it++;
					}
				}
				else
				{
					boost::filesystem::directory_iterator it(dirPath);
					boost::filesystem::directory_iterator end;

					while (it != end)
					{
						boost::filesystem::path file = it->path();
						files.push_back(file.string());

						it++;
					}
				}
			#endif

			return (files);
		}
	}
}

