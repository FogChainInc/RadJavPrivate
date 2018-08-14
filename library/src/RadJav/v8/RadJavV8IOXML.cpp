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

#ifdef HAS_XML_SUPPORT

namespace RadJAV
{
	namespace V8B
	{
		// XMLFile
		void IO::XML::XMLFile::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "_init", IO::XML::XMLFile::_init);
			V8_CALLBACK(object, "parseXMLFile", IO::XML::XMLFile::parseXMLFile);
			V8_CALLBACK(object, "parseXML", IO::XML::XMLFile::parseXML);
			V8_CALLBACK(object, "getRoot", IO::XML::XMLFile::getRoot);
		}

		void IO::XML::XMLFile::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			CPP::IO::XML::XMLFile *parser = RJNEW CPP::IO::XML::XMLFile();
			V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "parser", parser);
		}

		void IO::XML::XMLFile::parseXMLFile(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> path = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(path) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("Filename cannot be null!");
				return;
			}

			try
			{
				CPP::IO::XML::XMLFile *parser = (CPP::IO::XML::XMLFile *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "parser");
				parser->parseXMLFile(parseV8Value(path));
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}
		}

		void IO::XML::XMLFile::parseXML(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::String> xmlString = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(xmlString) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("XML string cannot be null!");
				return;
			}

			try
			{
				CPP::IO::XML::XMLFile *parser = (CPP::IO::XML::XMLFile *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "parser");
				parser->parseXML(parseV8Value(xmlString));

				args.This()->Set(String ("loadedFile").toV8String (args.GetIsolate ()), V8_JAVASCRIPT_ENGINE->v8CreateNewObject ("RadJav.XML.XMLParser"));
				//v8::Local<v8::Object> loadedFile = v8::Local<v8::Object>::Cast (args.This()->Get(String("loadedFile").toV8String(args.GetIsolate())));
				//V8_JAVASCRIPT_ENGINE->v8SetExternal(loadedFile, "xmlFile", std::make_shared<tinyxml2::XMLElement> (parser->parser->FirstChildElement()));
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}
		}

		void IO::XML::XMLFile::getRoot(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			//CPP::IO::XML::XMLFile *parser = V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "parser");

			//parser->root->toV8Object();

			//args.GetReturnValue().Set();
		}

		// XMLTag
		void IO::XML::XMLTag::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "_init", IO::XML::XMLFile::_init);
			V8_CALLBACK(object, "getChildren", IO::XML::XMLTag::getChildren);
			V8_CALLBACK(object, "getAttributes", IO::XML::XMLTag::getAttributes);
			V8_CALLBACK(object, "setTag", IO::XML::XMLTag::setTag);
			V8_CALLBACK(object, "getTag", IO::XML::XMLTag::getTag);
			V8_CALLBACK(object, "setValue", IO::XML::XMLTag::setValue);
			V8_CALLBACK(object, "getValue", IO::XML::XMLTag::getValue);
			V8_CALLBACK(object, "setAttribute", IO::XML::XMLTag::setAttribute);
			V8_CALLBACK(object, "hasAttribute", IO::XML::XMLTag::hasAttribute);
			V8_CALLBACK(object, "getAttribute", IO::XML::XMLTag::getAttribute);
		}

		void IO::XML::XMLTag::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			CPP::IO::XML::XMLTag *tag = NULL;

			if (args[0]->IsString() == true)
				tag = RJNEW CPP::IO::XML::XMLTag(parseV8Value (args[0]));

			if (args[0]->IsObject() == true)
			{
				std::shared_ptr<tinyxml2::XMLElement> elm = V8_JAVASCRIPT_ENGINE->v8GetExternal<tinyxml2::XMLElement>(
																		v8::Local<v8::Object>::Cast(args[0]), "xmlFile");
				tag = RJNEW CPP::IO::XML::XMLTag(elm.get ());
			}

			args.This()->Set(String("loadedFile").toV8String(args.GetIsolate()), V8_JAVASCRIPT_ENGINE->v8CreateNewObject("RadJav.XML.XMLParser"));
			v8::Local<v8::Object> loadedFile = v8::Local<v8::Object>::Cast (args.This()->Get(String("loadedFile").toV8String(args.GetIsolate())));
			//V8_JAVASCRIPT_ENGINE->v8SetExternal(loadedFile, "xmlFile", std::make_shared<tinyxml2::XMLElement>(tag->loadedFile->xmlFile));
		}

		void IO::XML::XMLTag::getChildren(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}

		void IO::XML::XMLTag::getAttributes(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}

		void IO::XML::XMLTag::setTag(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}

		void IO::XML::XMLTag::getTag(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}

		void IO::XML::XMLTag::setValue(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}

		void IO::XML::XMLTag::getValue(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}

		void IO::XML::XMLTag::getTags(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			v8::Local<v8::Value> arg1 = args[0];

			if (V8_JAVASCRIPT_ENGINE->v8IsNull(arg1) == true)
			{
				V8_JAVASCRIPT_ENGINE->throwException("XML tag string cannot be null!");

				return;
			}

			try
			{
				if (arg1->IsObject() == true)
				{
					//CPP::IO::XML::XMLTag()
				}

				v8::Local<v8::String> tag = v8::Local<v8::String>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));

				//CPP::IO::XML::XMLTag();
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}
		}

		void IO::XML::XMLTag::setAttribute(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}

		void IO::XML::XMLTag::hasAttribute(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}

		void IO::XML::XMLTag::getAttribute(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
		}
	}
}
#endif // HAS_XML_SUPPORT

#endif

