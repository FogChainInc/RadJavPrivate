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

#ifndef _XML_PARSER_H_
	#define _XML_PARSER_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"
	#include "RadJavArray.h"

	#ifdef USE_TINY_XML
		#include <tinyxml2.h>
	#endif

	namespace RadJAV
	{
		class XMLParser;

		class XMLElement
		{
			public:
				XMLElement();
				~XMLElement();

				XMLElement *findTag(String tagToFind, XMLElement *inElement = 0);
				Array<XMLElement *> findAllTags(String tagToFind, XMLElement *inElement = 0, bool traverseDown = true);
				String getAttributeValue(String attributeName);

				XMLElement *parent;
				Array<XMLElement *> *children;

				String tag;
				Array<String> aryAttributeNames;
				Array<String> aryAttributeValues;
				String content;

				XMLParser *parentParser;
		};

		class XMLParser
		{
			public:
				XMLParser();
				XMLParser(String file);
				~XMLParser();

				bool parseFile(String file);
				XMLElement *findTag(String tagToFind);
				Array<XMLElement *> findAllTags(String tagToFind);

				inline String getFilename()
				{
					return (strFilename);
				}

			protected:
				#ifdef USE_TINY_XML
					void navigateChildren(int depth, XMLElement *parent, tinyxml2::XMLElement *element);

					tinyxml2::XMLDocument tdFile;
				#endif

				String strFilename;
				Array<XMLElement *> *aryElements;
		};
	}
#endif

