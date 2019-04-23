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

namespace RadJAV
{
	namespace V8B
	{
		namespace Net
		{
			using CppWebServer = CPP::Net::WebServer;
			
			void WebServer::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_init", WebServer::init);
				V8_CALLBACK(object, "_start", WebServer::start);
				V8_CALLBACK(object, "_stop", WebServer::stop);
				V8_CALLBACK(object, "_on", WebServer::on);
			}

			void WebServer::init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<CppWebServer> webServer(RJNEW CppWebServer(V8_JAVASCRIPT_ENGINE, args),
														[](CppWebServer* p) {
															DELETEOBJ(p);
														});

				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_appObj", webServer);
			}

			void WebServer::start(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<CppWebServer> webServer = V8_JAVASCRIPT_ENGINE->v8GetExternal<CppWebServer>(args.This(), "_appObj");
				
				if (!webServer)
				{
					V8_JAVASCRIPT_ENGINE->throwException("WebServer not initialized");
					return;
				}

				if (args.Length() < 2 ||
					!args[0]->IsString() ||
					!args[1]->IsNumber())
				{
					V8_JAVASCRIPT_ENGINE->throwException("Port argument is required");
					return;
				}
				
				RJINT port = V8_JAVASCRIPT_ENGINE->v8ParseInt(args[1]);
				
				webServer->start(parseV8Value(args[0]), port);
			}

			void WebServer::stop(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<CppWebServer> webServer = V8_JAVASCRIPT_ENGINE->v8GetExternal<CppWebServer>(args.This(), "_appObj");
				
				if (!webServer)
				{
					V8_JAVASCRIPT_ENGINE->throwException("WebServer not initialized");
					return;
				}
				
				webServer->stop();
			}

			void WebServer::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				std::shared_ptr<CppWebServer> webServer = V8_JAVASCRIPT_ENGINE->v8GetExternal<CppWebServer>(args.This(), "_appObj");
				
				if (!webServer)
				{
					V8_JAVASCRIPT_ENGINE->throwException("WebServer not initialized");
					return;
				}
				
				if( args.Length() < 2 ||
				   !args[0]->IsString() ||
				   !args[1]->IsFunction())
				{
					V8_JAVASCRIPT_ENGINE->throwException("Event name and function is required");
					return;
				}
				
				webServer->on(parseV8Value(args[0]), v8::Local<v8::Function>::Cast(args[1]));
			}
		}
	}
}
#endif
