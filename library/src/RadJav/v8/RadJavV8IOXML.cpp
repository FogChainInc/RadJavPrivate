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
		void IO::XML::XMLFile::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
		{
			V8_CALLBACK(object, "_init", IO::XML::XMLFile::_init);
			V8_CALLBACK(object, "loadXMLFile", IO::XML::XMLFile::loadXMLFile);
			V8_CALLBACK(object, "loadXML", IO::XML::XMLFile::loadXML);
		}

		void IO::XML::XMLFile::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
		{
			CPP::IO::XML::XMLFile *parser = RJNEW CPP::IO::XML::XMLFile();
			V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "parser", parser);
		}

		void IO::XML::XMLFile::loadXMLFile(const v8::FunctionCallbackInfo<v8::Value> &args)
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
				parser->loadXMLFile(parseV8Value(path));
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}
		}

		void IO::XML::XMLFile::loadXML(const v8::FunctionCallbackInfo<v8::Value> &args)
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
				parser->loadXML(parseV8Value(xmlString));
			}
			catch (Exception ex)
			{
				V8_JAVASCRIPT_ENGINE->throwException(ex.getMessage());
				return;
			}
		}
	}
}
#endif

