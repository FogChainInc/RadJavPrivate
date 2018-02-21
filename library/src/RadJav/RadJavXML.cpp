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

#include "RadJavXML.h"

namespace RadJAV
{
	XMLElement::XMLElement()
	{
		parent = 0;
		parentParser = 0;
		children = RJNEW Array<XMLElement *>();

		tag = "";
		content = "";
	}

	XMLElement::~XMLElement()
	{
		if (children != 0)
		{
			for (unsigned int iIdx = 0; iIdx < children->size(); iIdx++)
			{
				RJDELETE children->at(iIdx);
				children->at(iIdx) = 0;
			}

			RJDELETE children;
			children = 0;
		}
	}

	XMLElement *XMLElement::findTag(String tagToFind, XMLElement *inElement)
	{
		XMLElement *xmlSearch = inElement;

		if (xmlSearch == 0)
			xmlSearch = this;

		if (xmlSearch->tag == tagToFind)
			return (xmlSearch);
		else
		{
			for (unsigned int iIdx = 0; iIdx < xmlSearch->children->size(); iIdx++)
			{
				XMLElement *xmlTemp = findTag(tagToFind, xmlSearch->children->at(iIdx));

				if (xmlTemp != 0)
					return (xmlTemp);
			}
		}

		return (0);
	}

	Array<XMLElement *> XMLElement::findAllTags(String tagToFind, XMLElement *inElement, bool traverseDown)
	{
		Array<XMLElement *> aryFound;
		XMLElement *xmlSearch = inElement;

		if (xmlSearch == 0)
			xmlSearch = this;

		for (unsigned int iIdx = 0; iIdx < xmlSearch->children->size(); iIdx++)
		{
			XMLElement *xmlCurrent = xmlSearch->children->at(iIdx);

			if (traverseDown == true)
			{
				XMLElement *xmlElement = xmlCurrent->findTag(tagToFind, xmlCurrent);

				if (xmlElement != 0)
					aryFound.push_back(xmlElement);
			}
			else
			{
				if (xmlCurrent->tag == tagToFind)
					aryFound.push_back(xmlCurrent);
			}
		}

		return (aryFound);
	}

	String XMLElement::getAttributeValue(String attributeName)
	{
		for (unsigned int iIdx = 0; iIdx < aryAttributeNames.size(); iIdx++)
		{
			String strAttributeName = aryAttributeNames.at(iIdx);
			String strAttributeValue = aryAttributeValues.at(iIdx);

			if (strAttributeName == attributeName)
				return (strAttributeValue);
		}

		return ("");
	}

	// XMLParser
	XMLParser::XMLParser()
	{
		aryElements = RJNEW Array<XMLElement *>();
		strFilename = "";
	}
	XMLParser::XMLParser(String file)
	{
		aryElements = RJNEW Array<XMLElement *>();
		parseFile(file);
	}

	XMLParser::~XMLParser()
	{
		if (aryElements != 0)
		{
			for (unsigned int iIdx = 0; iIdx < aryElements->size(); iIdx++)
			{
				RJDELETE aryElements->at(iIdx);
				aryElements->at(iIdx) = 0;
			}

			RJDELETE aryElements;
			aryElements = 0;
		}
	}

	XMLElement *XMLParser::findTag(String tagToFind)
	{
		for (unsigned int iIdx = 0; iIdx < aryElements->size(); iIdx++)
		{
			XMLElement *xmlElement = aryElements->at(iIdx)->findTag(tagToFind);

			if (xmlElement != 0)
				return (xmlElement);
		}

		return (0);
	}

	Array<XMLElement *> XMLParser::findAllTags(String tagToFind)
	{
		Array<XMLElement *> aryFound;

		for (unsigned int iIdx = 0; iIdx < aryElements->size(); iIdx++)
		{
			XMLElement *xmlCurrent = aryElements->at(iIdx);
			XMLElement *xmlElement = xmlCurrent->findTag(tagToFind);

			if (xmlElement != 0)
				aryFound.push_back(xmlElement);
		}

		return (aryFound);
	}

	bool XMLParser::parseFile(String file)
	{
		strFilename = file;

#ifdef USE_TINY_XML
		if (tdFile.LoadFile(file.c_str()) == false)
			return (false);

		navigateChildren(0, 0, tdFile.RootElement());
#endif

		return (true);
	}

	void XMLParser::navigateChildren(int depth, XMLElement *parent, tinyxml2::XMLElement *element)
	{
#ifdef USE_TINY_XML
		tinyxml2::XMLElement *xeElm = element;
		xeElm = xeElm->FirstChildElement();

		while (xeElm != 0)
		{
			XMLElement *xmlElment = RJNEW XMLElement();
			const char *cParentValue = (const char *)element->Value();
			const char *cValue = (const char *)xeElm->Value();
			const char *cContent = 0;
			String strParentValue = "";
			String strValue = "";
			String strContent = "";

			if (xeElm->NoChildren() == false)
			{
				tinyxml2::XMLNode *xeChild = xeElm->FirstChild();

				if (xeChild != 0)
					cContent = xeChild->Value();
			}

			if (cParentValue != 0)
				strParentValue = cParentValue;

			if (cValue != 0)
				strValue = cValue;

			if (cContent != 0)
				strContent = cContent;

			Array<String> aryAttributeNames;
			Array<String> aryAttributeValues;
			const tinyxml2::XMLAttribute *xaAtt = xeElm->FirstAttribute();

			while (xaAtt != 0)
			{
				const char *cAttributeName = (const char *)xaAtt->Name();
				String strAttributeName = "";
				const char *cAttributeValue = (const char *)xaAtt->Value();
				String strAttributeValue = "";

				if (cAttributeName != 0)
					strAttributeName = cAttributeName;

				if (cAttributeValue != 0)
					strAttributeValue = cAttributeValue;

				aryAttributeNames.push_back(strAttributeName);
				aryAttributeValues.push_back(strAttributeValue);

				xaAtt = xaAtt->Next();
			}

			xmlElment->parent = parent;
			xmlElment->tag = strValue;

			for (unsigned int iIdx = 0; iIdx < aryAttributeNames.size(); iIdx++)
				xmlElment->aryAttributeNames.push_back(aryAttributeNames.at(iIdx));

			for (unsigned int iIdx = 0; iIdx < aryAttributeValues.size(); iIdx++)
				xmlElment->aryAttributeValues.push_back(aryAttributeValues.at(iIdx));

			xmlElment->content = strContent;
			xmlElment->parentParser = this;

			if (parent == 0)
				aryElements->push_back(xmlElment);
			else
				parent->children->push_back(xmlElment);

			if (xeElm->NoChildren() == false)
				navigateChildren(depth++, xmlElment, xeElm);

			xeElm = xeElm->NextSiblingElement();
		}
#endif
	}
}

