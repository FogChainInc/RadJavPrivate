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
#include "v8/RadJavV8NetWebServer.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPNetWebServer.h"

#define NETTYPE CPP::Net::WebServer

namespace RadJAV
{
	namespace V8B
	{
		namespace Net
		{
			void WebServer::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_init", WebServer::_init);
				V8_CALLBACK(object, "listen", WebServer::listen);
				V8_CALLBACK(object, "serve", WebServer::serve);
				V8_CALLBACK(object, "stop", WebServer::stop);
			}

			void WebServer::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				NETTYPE *webServer = RJNEW NETTYPE();
				V8_JAVASCRIPT_ENGINE->v8SetInternalField<NETTYPE>(args.This(), "_webServer", webServer);
			}

			void WebServer::listen(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Number> val = v8::Local<v8::Number>::Cast(args[0]);
				RJINT port = val->IntegerValue();

				NETTYPE *webServer = (NETTYPE *)V8_JAVASCRIPT_ENGINE->v8GetInternalField(args.This(), "_webServer");
				webServer->port = port;
				webServer->listen();
			}

			void WebServer::serve(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				NETTYPE *webServer = (NETTYPE *)V8_JAVASCRIPT_ENGINE->v8GetInternalField(args.This(), "_webServer");

				if (webServer == NULL)
				{
					V8_JAVASCRIPT_ENGINE->throwException(" web server not listening");
					return;
				}

				v8::Local<v8::Function> val = v8::Local<v8::Function>::Cast(args[0]);

				webServer->serve(val);
			}

			void WebServer::stop(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				NETTYPE *webServer = (NETTYPE *)V8_JAVASCRIPT_ENGINE->v8GetInternalField(args.This(), "_webServer");

				if ((webServer == NULL) || (webServer->isAlive == false))
				{
					V8_JAVASCRIPT_ENGINE->throwException(" web server not listening");
					return;
				}

				webServer->stop();
			}
		}
	}
}
#endif

