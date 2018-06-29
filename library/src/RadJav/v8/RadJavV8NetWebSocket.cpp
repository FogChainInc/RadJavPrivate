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
				V8_CALLBACK(object, "_init", WebSocketServer::_init);

				V8_CALLBACK(object, "listen", WebSocketServer::listen);
				V8_CALLBACK(object, "sendToAll", WebSocketServer::sendToAll);
				V8_CALLBACK(object, "send", WebSocketServer::send);
				V8_CALLBACK(object, "receive", WebSocketServer::receive);
				V8_CALLBACK(object, "close", WebSocketServer::close);

				V8_CALLBACK(object, "onReceive", WebSocketServer::onReceive);
				V8_CALLBACK(object, "onAccept", WebSocketServer::onAccept);

				V8_CALLBACK(object, "on", WebSocketServer::on);
			}

			void WebSocketServer::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = RJNEW CPP::Net::WebSocketServer();
				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_webSocket", webSocket);
			}

			void WebSocketServer::listen(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer * webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				
				v8::Local<v8::Number> port;
				RJINT portI = 0;

				if (args.Length() > 0)
				{
					port = v8::Local<v8::Number>::Cast(args[0]);
					portI = port->IntegerValue();
					V8_JAVASCRIPT_ENGINE->v8SetNumber(args.This(), "port", portI);
				}

				webSocket->listen(portI);
			}

			void WebSocketServer::sendToAll(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer * webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String str = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, val);

				webSocket->sendToAll(str);
			}

			void WebSocketServer::send(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				
				v8::Local<v8::String> val_id = v8::Local<v8::String>::Cast(args[0]);
				String id = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, val_id);

				v8::Local<v8::String> val_msg = v8::Local<v8::String>::Cast(args[1]);
				String message = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, val_msg);

				webSocket->send(id, message);
			}

			void WebSocketServer::receive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				auto resp = webSocket->receive();
				v8::Local<v8::Value> ret;
				v8::Isolate *isolate = args.GetIsolate();

				ret = v8::String::NewFromUtf8(isolate, resp.c_str());
				args.GetReturnValue().Set(ret);
			}

			void WebSocketServer::close(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				
				webSocket->close();
			}

			void WebSocketServer::onAccept(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Persistent<v8::Function> *func = RJNEW v8::Persistent<v8::Function>();
				v8::Local<v8::Function> newEvt = v8::Local<v8::Function>::Cast(args[0]);

				func->Reset(V8_JAVASCRIPT_ENGINE->isolate, newEvt);

				RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent = func;
			}

			void WebSocketServer::onReceive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Persistent<v8::Function> *func = RJNEW v8::Persistent<v8::Function>();
				v8::Local<v8::Function> newEvt = v8::Local<v8::Function>::Cast(args[0]);

				func->Reset(V8_JAVASCRIPT_ENGINE->isolate, newEvt);

				RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent = func;
			}

			void WebSocketServer::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);

				webSocket->on(event, func);
			}

			void WebSocketClient::_init(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String host = parseV8Value(val);
				val = v8::Local<v8::String>::Cast(args[1]);
				String port = parseV8Value(val);

				CPP::Net::WebSocketClient *webSocket = RJNEW CPP::Net::WebSocketClient();
				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This(), "_webSocket", webSocket);
			}

			void WebSocketClient::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "connect", WebSocketClient::connect);
				V8_CALLBACK(object, "send", WebSocketClient::send);
				V8_CALLBACK(object, "receive", WebSocketClient::receive);
				V8_CALLBACK(object, "close", WebSocketClient::close);

				V8_CALLBACK(object, "on", WebSocketServer::on);
			}

			void WebSocketClient::connect(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String host = parseV8Value(val);
				val = v8::Local<v8::String>::Cast(args[1]);
				String port = parseV8Value(val);

				CPP::Net::WebSocketClient *webSocket = RJNEW CPP::Net::WebSocketClient();

				webSocket->connect(host, port);
				V8_JAVASCRIPT_ENGINE->v8SetExternal(args.This (), "_webSocket", webSocket);
			}

			void WebSocketClient::send(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				v8::Local<v8::String> val = v8::Local<v8::String>::Cast(args[0]);
				String message = parseV8Value(val);

				webSocket->send(message);
			}

			void WebSocketClient::receive(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				auto resp = webSocket->receive();
				v8::Local<v8::Value> ret;
				v8::Isolate *isolate = args.GetIsolate();

				ret = v8::String::NewFromUtf8(isolate, resp.c_str());
				args.GetReturnValue().Set(ret);
			}

			void WebSocketClient::close(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");

				webSocket->close();
			}

			void WebSocketClient::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)V8_JAVASCRIPT_ENGINE->v8GetExternal(args.This(), "_webSocket");
				String event = parseV8Value(args[0]);
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(args[1]);

				webSocket->on(event, func);
			}
		}
	}
}
#endif

