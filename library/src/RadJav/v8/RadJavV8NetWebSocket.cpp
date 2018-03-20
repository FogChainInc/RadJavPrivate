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
#include "v8/RadJavV8NetWebSocket.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPNetWebSocketServer.h"
#include "cpp/RadJavCPPNetWebSocketClient.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace Net
		{
			void WebSocketServer::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				CPP::Net::WebSocketServer *webSocket = RJNEW CPP::Net::WebSocketServer();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(object, "_webSocket", webSocket);

				V8_CALLBACK(object, "sendToAll", WebSocketServer::sendToAll);
				V8_CALLBACK(object, "send", WebSocketServer::send);
				V8_CALLBACK(object, "listen", WebSocketServer::listen);
				V8_CALLBACK(object, "close", WebSocketServer::close);

				V8_CALLBACK(object, "on", WebSocketServer::on);
			}

			void WebSocketServer::sendToAll(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String message = parseV8Value(val);

				webSocket->sendToAll(message);
			}

			void WebSocketServer::send(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String message = parseV8Value(val);

				webSocket->send(message);
			}

			void WebSocketServer::listen(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Local<v8::Number> port;
				RJINT portI = webSocket->port;

				if (args.Length() > 0)
				{
					port = v8::Local<v8::Number>::Cast(args[0]);
					portI = port->IntegerValue();
					V8_JAVASCRIPT_ENGINE->v8SetNumber(args.This(), "port", portI);
				}

				webSocket->port = portI;
				webSocket->listen();
			}

			void WebSocketServer::close(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				
				webSocket->close();
			}

			void WebSocketServer::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				//webSocket->on(event, func);
			}

			void WebSocketClient::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				CPP::Net::WebSocketClient *webSocket = RJNEW CPP::Net::WebSocketClient();

				V8_JAVASCRIPT_ENGINE->v8SetExternal(object, "_webSocket", webSocket);

				V8_CALLBACK(object, "send", WebSocketClient::send);
				V8_CALLBACK(object, "connect", WebSocketClient::connect);
				V8_CALLBACK(object, "close", WebSocketClient::close);

				V8_CALLBACK(object, "on", WebSocketServer::on);
			}

			void WebSocketClient::send(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String message = parseV8Value(val);

				webSocket->send(message);
			}

			void WebSocketClient::connect(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String url = parseV8Value(val);

				webSocket->connect(url);
			}

			void WebSocketClient::close(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				webSocket->close();
			}

			void WebSocketClient::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				//webSocket->on(event, func);
			}
		}
	}
}
#endif

