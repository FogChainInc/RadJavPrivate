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

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace IO
		{
			namespace XML
			{
				XMLFile::XMLFile()
				{
					#ifdef USE_TINYXML2
						parser = RJNEW tinyxml2::XMLDocument();
					#endif
					root = NULL;
				}

				XMLFile::~XMLFile()
				{
					DELETEOBJ(root);
					#ifdef USE_TINYXML2
						DELETEOBJ(parser);
					#endif
				}

				void XMLFile::loadXMLFile(String filePath)
				{
					String contents = CPP::IO::TextFile::readFile(filePath);

					loadXML(contents);
				}

				void XMLFile::loadXML(String xmlString)
				{
					#ifdef USE_TINYXML2
						tinyxml2::XMLError err = parser->Parse(xmlString.c_str ());

						if (err != tinyxml2::XMLError::XML_SUCCESS)
							throw new Exception("Error while loading XML file! Error: " + (String)parser->ErrorStr ());

						root = RJNEW XMLTag (parser->RootElement ());
					#endif
				}

				XMLTag::XMLTag(String tag)
				{
					this->tag = tag;
					value = "";
					children = NULL;
				}

				XMLTag::~XMLTag()
				{
					auto itBegin = attributes.begin();
					auto itEnd = attributes.end();

					while (itBegin != itEnd)
					{
						XMLAttribute *xmlAttr = (*itBegin).second;
						DELETEOBJ(xmlAttr);

						itBegin++;
					}
				}

				#ifdef USE_TINYXML2
					XMLTag::XMLTag(tinyxml2::XMLElement *tag)
					{
						this->tag = tag->Name ();

						const tinyxml2::XMLAttribute *attr = tag->FirstAttribute();

						while (attr != NULL)
						{
							String name = attr->Name();
							String value = attr->Value();
							XMLAttribute *xmlAttr = RJNEW XMLAttribute (name, value);

							attributes.insert(HashMapPair<RJCSTR, XMLAttribute *> (name.c_str (), xmlAttr));

							attr = attr->Next();
						}

						value = tag->GetText ();

						tinyxml2::XMLElement *elm = tag->FirstChildElement();

						while (elm != NULL)
						{
							XMLTag *newTag = RJNEW XMLTag(elm);

							children.push_back(newTag);

							elm = elm->NextSiblingElement();
						}
					}
				#endif

				Array<XMLTag *> XMLTag::getTags(String tag)
				{
					Array<XMLTag *> tags;

					for (RJINT iIdx = 0; iIdx < children.size (); iIdx++)
					{
						XMLTag *child  = children.at (iIdx);

						if (child->tag == tag)
							tags.push_back (child);
					}

					return (tags);
				}

				/// Set an attribute for this tag.
				void XMLTag::setAttribute(String attribute, String value)
				{
					auto attr = attributes.find(attribute.c_str());
					XMLAttribute *xmlAttr = RJNEW XMLAttribute(attribute, value);

					if (attr == attributes.end())
						attributes.insert(HashMapPair<RJCSTR, XMLAttribute *>(attribute.c_str(), xmlAttr));
					else
					{
						XMLAttribute *oldXMLAttr = (*attr).second;
						DELETEOBJ (oldXMLAttr);
						(*attr).second = xmlAttr;
					}
				}

				/// Get an attribute from this tag.
				XMLAttribute *XMLTag::getAttribute(String attribute)
				{
					auto attr = attributes.find(attribute.c_str());

					if (attr == attributes.end())
						throw new Exception("Attribute does not exist!");

					return ((*attr).second);
				}

				/// Get an attribute from this tag.
				String XMLTag::getAttributeString(String attribute)
				{
					return (getAttribute (attribute)->getValue ());
				}

				/// Get an attribute integer value from this tag.
				RJNUMBER XMLTag::getAttributeInt(String attribute)
				{
					return (getAttribute(attribute)->toInt());
				}

				/// Get an attribute float value from this tag.
				RJNUMBER XMLTag::getAttributeFloat(String attribute)
				{
					return (getAttribute(attribute)->toFloat());
				}

				/// Get a boolean result from an attribute.
				RJBOOL XMLTag::getAttributeBoolean(String attribute)
				{
					return (getAttribute(attribute)->toBoolean());
				}

				/// Convert this tag to a string.
				String XMLTag::toString()
				{
					String result = "<" + tag + " ";

					auto itBegin = attributes.begin();
					auto itEnd = attributes.end();

					while (itBegin != itEnd)
					{
						XMLAttribute *attribute = (*itBegin).second;
						result += attribute->toString() + " ";

						itBegin++;
					}

					if ((value != "") || (children.size () > 0))
					{
						result += ">" + value;

						if (value != "")
							result += " ";

						for (RJINT iIdx = 0; iIdx < children.size (); iIdx++)
						{
							XMLTag *child = children.at (iIdx);
							result += child->toString () + " ";
						}

						result += "</" + tag + ">";
					}
					else
						result += "/>";

					return (result);
				}
			}
		}
	}
}

