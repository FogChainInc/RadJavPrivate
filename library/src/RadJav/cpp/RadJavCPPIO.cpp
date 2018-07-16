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
	#include <boost/filesystem/operations.hpp>
	#include <boost/filesystem/path.hpp>
	#include <boost/asio.hpp>

	namespace fs = boost::filesystem;

	boost::asio::io_service RadJAV::CPP::IO::SerialComm::m_io;
	boost::asio::serial_port RadJAV::CPP::IO::SerialComm::m_serial(RadJAV::CPP::IO::SerialComm::m_io);
#endif

RadJAV::Array<int> RadJAV::CPP::IO::SerialComm::m_baudRates = RadJAV::Array<int> ({ 110, 300, 600, 1200, 2400, 4800, 9600, 14400, 19200, 38400, 57600, 115200, 128000, 256000 });

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"

	v8::Persistent<v8::Function>* RadJAV::CPP::IO::TextFile::m_textfileReadEvent = nullptr;
	//v8::Persistent<v8::Function>* RadJAV::CPP::IO::m_fileListEvent = nullptr;
#endif

namespace RadJAV
{
	namespace CPP
	{
		RJBOOL IO::isDir(String path_)
		{
			#ifdef USE_BOOST
				try {
					return fs::is_directory(fs::status(path_.c_str()));
				}
				catch (fs::filesystem_error ex)
				{
					return false;
				}
			#endif
		}

		RJBOOL IO::isFile(String path_)
		{
			#ifdef USE_BOOST
				try {
					return fs::is_regular_file(fs::status(path_.c_str()));
				}
				catch (fs::filesystem_error ex)
				{
					return false;
				}
			#endif
		}

		RJBOOL IO::isSymLink(String path_)
		{
			#ifdef USE_BOOST
				try {
					return fs::is_symlink(fs::status(path_.c_str()));
				}
				catch (fs::filesystem_error ex)
				{
					return false;
				}
			#endif
		}

		String IO::currentPath()
		{
			#ifdef USE_BOOST
				try {
					return fs::current_path().string();
				}
				catch (fs::filesystem_error ex)
				{
					return String();
				}
			#endif
		}

		void IO::changePath(String path_)
		{
			#ifdef USE_BOOST
				try {
					fs::current_path(path_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		RJBOOL IO::exists(String path_)
		{
			#ifdef USE_BOOST
				try {
					return fs::exists(path_.c_str());
				}
				catch (fs::filesystem_error ex)
				{
					return false;
				}
			#endif
		}

		void IO::createDir(String path_)
		{
			if (exists(path_) == true) return;

			#ifdef USE_BOOST
				try {
					fs::create_directory(path_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		void IO::copyDir(String src_, String dest_, RJBOOL recursive_)
		{
			/// @todo We want to add a JavaScript function that will allow the developer to see 
			/// the file or folder that's being copied over and be able to prevent it from being 
			/// copied if necessary. So we would need to use listFiles and go through each file/folder 
			/// and report it back to the JavaScript function and wait for a boolean response that 
			/// would determine if the file/folder gets copied.

			#ifdef USE_BOOST
				try
				{
					fs::path destPath(dest_.c_str());

					if (exists(dest_) == false)
						fs::create_directory(destPath);

					fs::recursive_directory_iterator it(src_.c_str());
					fs::recursive_directory_iterator end;

					while (it != end)
					{
						fs::path file = it->path();

						if (fs::is_directory(file) == true)
						{
							if (recursive_ == true)
							{
								fs::path nextFile = destPath / file.filename();

								copyDir(file.string(), nextFile.string());
							}
						}
						else
							fs::copy_file(file, destPath / file.filename());

						it++;
					}
				}
				catch (fs::filesystem_error ex)
				{
				}
			#endif
		}

		void IO::renameDir(String src_, String dest_)
		{
			// test for the source being there
			if (exists(src_) == false)
			{
				throw Exception("Unable to rename directory (missing): " + src_);
				return;
			}

			#ifdef USE_BOOST
				try {
					fs::rename(src_.c_str(), dest_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		void IO::deleteDir(String path_)
		{
			// test for the source being there
			if (exists(path_) == false)
			{
				throw Exception("Unable to delete directory (missing): " + path_);
				return;
			}

			#ifdef USE_BOOST
				try {
					fs::remove_all(path_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		RJBOOL IO::isEmpty(String path_)
		{
			// test for the source being there
			if (exists(path_) == false)
			{
				throw Exception("Directory missing: " + path_);
				return false;
			}

			#ifdef USE_BOOST
				try {
					return fs::is_empty(path_.c_str());
				}
				catch (fs::filesystem_error ex)
				{
					return false;
				}
			#endif
		}

		void IO::createSymLink(String path_, String link_)
		{
			#ifdef USE_BOOST
				try {
					fs::create_symlink(path_.c_str(), link_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		void IO::copySymLink(String src_, String dest_)
		{
			// test for the source being there
			if (exists(src_) == false)
			{
				throw Exception("Unable to copy symlink (missing): " + src_);
				return;
			}

			// test for the destination being there
			if (exists(dest_) == true)
			{
				throw Exception("Unable to copy symlink (already there): " + dest_);
				return;
			}

			#ifdef USE_BOOST
				try {
					fs::copy_symlink(src_.c_str(), dest_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		void IO::renameSymLink(String src_, String dest_)
		{
			// test for the source being there
			if (exists(src_) == false)
			{
				throw Exception("Unable to rename symlink (missing): " + src_);
				return;
			}

			// test for the destination being there
			if (exists(dest_) == true)
			{
				throw Exception("Unable to rename symlink (already there): " + dest_);
				return;
			}

			#ifdef USE_BOOST
				try {
					fs::rename(src_.c_str(), dest_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		void IO::deleteSymLink(String path_)
		{
			// test for the source being there
			if (exists(path_) == false)
			{
				throw Exception("Unable to delete synlink (missing): " + path_);
				return;
			}

			#ifdef USE_BOOST
				try {
					fs::remove(path_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}


		void IO::copyFile(String src_, String dest_, RJBOOL overwriteIfExists_)
		{
			// test for the source being there
			if (exists(src_) == false)
			{
				throw Exception("Unable to copy file (missing): " + src_);
				return;
			}

			#ifdef USE_BOOST
				try {
					fs::copy_option co = fs::copy_option::fail_if_exists;
					if (overwriteIfExists_) co = fs::copy_option::overwrite_if_exists;

					fs::copy_file(src_.c_str(), dest_.c_str(), co);
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		void IO::renameFile(String src_, String dest_)
		{
			// test for the source being there
			if (exists(src_) == false)
			{
				throw Exception("Unable to rename file (missing): " + src_);
				return;
			}

			#ifdef USE_BOOST
				try {
					fs::rename(src_.c_str(), dest_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		void IO::deleteFile(String path_)
		{
			#ifdef USE_BOOST
				try {
					fs::remove(path_.c_str());
				}
				catch (fs::filesystem_error ex) {}
			#endif
		}

		RJBOOL IO::SerialComm::open(String port_, RJINT baudRate_)
		{
			// make sure the baud rate is valid
			auto res = std::find(std::begin(m_baudRates), std::end(m_baudRates), baudRate_);
			if (res == std::end(m_baudRates))
				return false;


			#ifdef USE_BOOST
				m_serial.set_option(boost::asio::serial_port_base::baud_rate(baudRate_));

				// these can become configurable but they need accessors
				// currently set at 8,N,1,No flow control
				m_serial.set_option(boost::asio::serial_port_base::character_size(8));
				m_serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
				m_serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
				m_serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

				m_serial.open(port_);

				return m_serial.is_open();
			#endif

			return false;
		}

		String IO::SerialComm::read()
		{
			String returnValue = "";
			char c;

			for (;;)
			{
				#ifdef USE_BOOST
					boost::asio::read(m_serial, boost::asio::buffer(&c, 1));
				#endif

				switch (c)
				{
				case '\r':
					break;
				case '\n':
					return returnValue;
				default:
					returnValue += c;
				}
			}
		}

		RJULONG IO::SerialComm::write(String buffer_, RJINT bufferSize_)
		{
			#ifdef USE_BOOST
				size_t bytesRead = boost::asio::write(m_serial, boost::asio::buffer(buffer_.c_str(), bufferSize_));
			#endif

			return bytesRead;
		}

		void IO::SerialComm::close()
		{
			#ifdef USE_BOOST
				m_serial.close();
			#endif
		}

		void IO::TextFile::writeFile(String path_, String contents_, RJINT outputType_)
		{
			RJINT type = std::ios_base::out;

			if (static_cast<IO::TextFile::operation>(outputType_) == IO::TextFile::operation::append)
				type = std::ios_base::app;

			std::fstream file(path_, type);
			file.write(contents_.c_str(), contents_.size());
			file.close();
		}

		void IO::TextFile::writeFileAsync(String path_, String contents_, RJINT outputType_)
		{
			boost::asio::post(m_ioQueue, [=]()
			{
				writeFile(path_, contents_, outputType_);
			});
		}

		String IO::TextFile::readFile(String path_)
		{
			std::fstream file(path_, std::ios_base::in);
			String contents = "";

			if (file.is_open() == false)
			{
				throw Exception("Unable to open file: " + path_);

				return String();
			}

			while (file.good() == true)
			{
				char cChar = file.get();

				if (file.good() == false)
					break;

				contents += cChar;
			}

			file.close();

			return contents;
		}

		void IO::TextFile::readFileAsync(String path_)
		{
			boost::asio::post(m_ioQueue, [=]()
			{
				auto contents = readFile(path_);

				#ifdef USE_V8
					if (RadJAV::CPP::IO::TextFile::m_textfileReadEvent != nullptr)
					{
						v8::Local<v8::Function> evt = v8::Local<v8::Function>::Cast(RadJAV::CPP::IO::TextFile::m_textfileReadEvent->Get(V8_JAVASCRIPT_ENGINE->isolate));

						v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[1];
						args[0] = contents.toV8String(V8_JAVASCRIPT_ENGINE->isolate);

						if (V8_JAVASCRIPT_ENGINE->v8IsNull(evt) == false)
							evt->Call(V8_JAVASCRIPT_ENGINE->globalContext->Global(), 1, args);
						DELETE_ARRAY(args);
					}
				#endif

			});
		}

		template<class I, class E, class T>
		T buildFileList(I it, E end)
		{
			T files;

			while (it != end)
			{
				files.push_back(it->path().string());
				it++;
			}

			return files;
		}

		Array<String> IO::listFiles(String path_, RJBOOL recursive_)
		{
			Array<String> files;

			#ifdef USE_BOOST
				fs::path dirPath(path_.c_str());

				if (recursive_ == true)
				{
					using iterator = fs::recursive_directory_iterator;
					iterator end, it(dirPath);
					return buildFileList<iterator, iterator, Array<String>>(it, end);
				}
				else
				{
					using iterator = fs::directory_iterator; 
					iterator end, it(dirPath);
					return buildFileList<iterator, iterator, Array<String>>(it, end);
				}
			#endif

			return files;
		}

		void IO::listFilesAsync(String path_, RJBOOL recursive_)
		{
			boost::asio::post(m_ioQueue, [=]()
			{
				auto files = listFiles(path_, recursive_);

				#ifdef USE_V8
					/*if (RadJAV::CPP::IO::m_fileListEvent != nullptr)
					{
						v8::Local<v8::Function> evt = v8::Local<v8::Function>::Cast(RadJAV::CPP::IO::m_fileListEvent->Get(V8_JAVASCRIPT_ENGINE->isolate));

						v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[1];
						args[0] = convertArrayToV8Array(files, V8_JAVASCRIPT_ENGINE->isolate);

						if (V8_JAVASCRIPT_ENGINE->v8IsNull(evt) == false)
							evt->Call(V8_JAVASCRIPT_ENGINE->globalContext->Global(), 1, args);
						DELETE_ARRAY(args);
					}*/
				#endif
			});
		}

		String IO::normalizePath(String path_, String basePath_)
		{
			String normalizedPath = path_;

			#ifdef USE_BOOST
				path_ = basePath_ + fs::path::preferred_separator + path_;
				fs::path newPath(path_.c_str());

				newPath = newPath.lexically_normal();
				normalizedPath = newPath.string ();
			#endif

			return (normalizedPath);
		}
	}
}

