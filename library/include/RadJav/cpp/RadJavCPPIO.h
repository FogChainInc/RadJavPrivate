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
#include "RadJavArray.h"
#include "RadJavHashMap.h"

#include "cpp/RadJavCPPChainedPtr.h"

#ifdef USE_TINYXML2
	#include <tinyxml2.h>
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

			#ifdef USE_V8
				//static v8::Persistent<v8::Function> *m_fileListEvent;
			#endif

			/// Check to see if a directory exists.
			RJBOOL isDir(String path_);
			/// Check to see if a file exists.
			RJBOOL isFile(String path_);
			/// Check to see if the file/directory is a symbolic link.
			RJBOOL isSymLink(String path_);

			/// Get the current directory path.
			String currentPath();
			/// Change the current directory path.
			void changePath(String path_);
			/// Checks if the file/directory exists.
			RJBOOL exists(String path_);

			/// Create a directory.
			void createDir(String path_);
			/// Copy a directory.
			void copyDir(String src_, String dest_, RJBOOL recursive_ = true);
			/// Rename a directory.
			void renameDir(String src_, String dest_);
			/// Delete a directory.
			void deleteDir(String path_);
			/// Check if a directory is empty.
			RJBOOL isEmpty(String path_);

			/// Create a symbolic link.
			void createSymLink(String path_, String link_);
			/// Copy a symbolic link.
			void copySymLink(String src_, String dest_);
			/// Rename a symbolic link.
			void renameSymLink(String src_, String dest_);
			/// Delete a symbolic link.
			void deleteSymLink(String path_);

			/// Copy a file.
			void copyFile(String src_, String dest_, RJBOOL overwriteIfExists_ = true);
			/// Rename a file.
			void renameFile(String src_, String dest_);
			/// Delete a file.
			void deleteFile(String path_);

			/// List files in a directory.
			Array<String> listFiles(String path_, RJBOOL recursive_ = true);
			/// Asychronously list files in a directory.
			void listFilesAsync(String path_, RJBOOL recursive_ = true);

			/// Normalize a file/directory path.
			String normalizePath(String path_, String basePath_ = "");

			/// Handles serial communications.
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

					static Array<int> m_baudRates;
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

				#ifdef HAS_XML_SUPPORT
					namespace XML
					{
						class XMLTag;
						class XMLAttribute;

						class RADJAV_EXPORT XMLFile: public std::enable_shared_from_this<XMLFile>, public ChainedPtr
						{
							public:
								XMLFile();
								~XMLFile();

								/// Open a XML and load it.
								void loadXMLFile(String filePath);
								/// Load XML from a string.
								void loadXML(String xmlString);

								#ifdef USE_TINYXML2
									tinyxml2::XMLDocument *parser;
								#endif

								XMLTag *root;
						};

						class RADJAV_EXPORT XMLTag
						{
							public:
								XMLTag(String tag);
								#ifdef USE_TINYXML2
									XMLTag(tinyxml2::XMLElement *tag);
								#endif

								~XMLTag();

								Array<XMLTag *> getTags(String tag);

								/// Set an attribute for this tag.
								void setAttribute(String attribute, String value);

								/// Get an attribute from this tag.
								XMLAttribute *getAttribute(String attribute);

								/// Get an attribute from this tag.
								String getAttributeString(String attribute);

								/// Get an attribute integer value from this tag.
								RJNUMBER getAttributeInt(String attribute);

								/// Get an attribute float value from this tag.
								RJNUMBER getAttributeFloat(String attribute);

								/// Get a boolean result from an attribute.
								RJBOOL getAttributeBoolean(String attribute);

									/// Convert this tag to a string.
								String toString();

								String tag;
								HashMap<RJCSTR, XMLAttribute *> attributes;
								String value;
								Array<XMLTag *> children;
						};

						class RADJAV_EXPORT XMLAttribute
						{
							public:
								/// The attribute's name.
								String name;
								/// The value of the attribute.
								String value;

								XMLAttribute (String name, String value)
								{
									this->name = name;
									this->value = value;
								}

								/// Get the value of the attribute.
								String getValue()
								{
									return (value);
								}

									/// Get the integer value of the attribute.
								RJNUMBER toInt()
								{
									return (parseInt(value));
								}

								/// Get the float value of the attribute.
								RJNUMBER toFloat()
								{
									return (parseFloat(value));
								}

								/// Get the boolean value of the attribute.
								RJBOOL toBoolean()
								{
									return (parseBoolean(value));
								}

								/// Convert attribute to a string.
								String toString()
								{
									return (name + " = \"" + value + "\"");
								}
						};
					}
				#endif
		};
	}
}
#endif

