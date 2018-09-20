/*
	MIT-LICENSE
	Copyright (c) 2018 Higher Edge Software, LLC

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
#include "jscore/RadJavJSCNetWebSocket.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"
#include "cpp/RadJavCPPNetWebSocketServer.h"
#include "cpp/RadJavCPPNetWebSocketClient.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace Net
		{
			void WebSocketServer::createV8Callbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", WebSocketServer::_init);

				JSC_CALLBACK(object, "listen", WebSocketServer::listen);
				JSC_CALLBACK(object, "sendToAll", WebSocketServer::sendToAll);
				JSC_CALLBACK(object, "send", WebSocketServer::send);
				JSC_CALLBACK(object, "receive", WebSocketServer::receive);
				JSC_CALLBACK(object, "close", WebSocketServer::close);

				JSC_CALLBACK(object, "onReceive", WebSocketServer::onReceive);
				JSC_CALLBACK(object, "onAccept", WebSocketServer::onAccept);

				JSC_CALLBACK(object, "on", WebSocketServer::on);
			}

			JSValueRef WebSocketServer::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Net::WebSocketServer *webSocket = RJNEW CPP::Net::WebSocketServer();
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_webSocket", webSocket);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebSocketServer::listen(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CPP::Net::WebSocketServer * webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
				
				JSValueRef portArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!portArg)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Port cannot be null!");
					return undefined;
				}
				
				RJINT port = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, portArg);
				
				JSC_JAVASCRIPT_ENGINE->jscSetNumber(thisObject, "port", port);

				if (webSocket)
					webSocket->listen(port);
				
				return undefined;
			}

			JSValueRef WebSocketServer::sendToAll(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CPP::Net::WebSocketServer * webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
				
				if (!webSocket)
					return undefined;

				JSValueRef val = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				if (!val)
					return undefined;
				
				String str = parseJSCValue(ctx, val);

				webSocket->sendToAll(str);
				
				return undefined;
			}

			JSValueRef WebSocketServer::send(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
				
				if (!webSocket)
					return undefined;
				
				JSValueRef valId = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef valMsg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!valId || !valMsg)
					return undefined;
				
				String id = parseJSCValue(ctx, valId);
				String message = parseJSCValue(ctx, valMsg);

				webSocket->send(id, message);
				
				return undefined;
			}

			JSValueRef WebSocketServer::receive(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");

				if (webSocket)
				{
					auto resp = webSocket->receive();
					JSValueRef ret = resp.toJSCValue(ctx);
					return ret;
				}
				
				return String("").toJSCValue(ctx);
			}

			JSValueRef WebSocketServer::close(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
				
				if (webSocket)
					webSocket->close();

				if (RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent)
				{
					JSValueUnprotect(ctx, RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent);
					RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent = nullptr;
				}
				
				if (RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent)
				{
					JSValueUnprotect(ctx, RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent);
					RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent = nullptr;
				}

				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebSocketServer::onAccept(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				JSValueRef func = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!func)
					return undefined;

				JSValueProtect(ctx, func);
				
				RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(func);
				
				return undefined;
			}

			JSValueRef WebSocketServer::onReceive(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				JSValueRef func = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!func)
					return undefined;
				
				JSValueProtect(ctx, func);

				RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(func);
				
				return undefined;
			}

			JSValueRef WebSocketServer::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
				
				JSValueRef eventArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef funcArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);

				if (JSC_JAVASCRIPT_ENGINE->jscIsNull(eventArg) ||
					JSC_JAVASCRIPT_ENGINE->jscIsNull(funcArg))
				{
					return undefined;
				}
				
				String event = parseJSCValue(ctx, eventArg);

				if (webSocket)
					webSocket->on(event, JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(funcArg));
				
				return undefined;
			}

			JSValueRef WebSocketClient::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef val = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				String host;
				if (val)
					host = parseJSCValue(ctx, val);
				
				val = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				String port;
				if (val)
					port = parseJSCValue(ctx, val);

				CPP::Net::WebSocketClient *webSocket = RJNEW CPP::Net::WebSocketClient();
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_webSocket", webSocket);
				
				return JSValueMakeUndefined(ctx);
			}

			void WebSocketClient::createV8Callbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "connect", WebSocketClient::connect);
				JSC_CALLBACK(object, "send", WebSocketClient::send);
				JSC_CALLBACK(object, "receive", WebSocketClient::receive);
				JSC_CALLBACK(object, "close", WebSocketClient::close);

				JSC_CALLBACK(object, "on", WebSocketServer::on);
			}

			JSValueRef WebSocketClient::connect(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				JSValueRef hostArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef portArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);

				String host;
				String port;
				
				if (!hostArg || !portArg)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Host and port cannot be null!");
					return undefined;
				}
				
				host = parseJSCValue(ctx, hostArg);
				port = parseJSCValue(ctx, portArg);

				CPP::Net::WebSocketClient *webSocket = RJNEW CPP::Net::WebSocketClient();

				webSocket->connect(host, port);
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_webSocket", webSocket);
				
				return undefined;
			}

			JSValueRef WebSocketClient::send(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
				
				JSValueRef msgArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (msgArg)
				{
					String message = parseJSCValue(ctx, msgArg);
					if (webSocket)
						webSocket->send(message);
				}
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebSocketClient::receive(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");

				if (webSocket)
				{
					auto resp = webSocket->receive();
					return resp.toJSCValue(ctx);
				}
				
				return String("").toJSCValue(ctx);
			}

			JSValueRef WebSocketClient::close(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");

				if (webSocket)
					webSocket->close();
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebSocketClient::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CPP::Net::WebSocketClient *webSocket = (CPP::Net::WebSocketClient *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
				
				JSValueRef eventArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef funcArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				
				if (eventArg &&
					funcArg &&
					webSocket)
				{
					String event = parseJSCValue(ctx, eventArg);
					JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(funcArg);

					webSocket->on(event, func);
				}
				
				return JSValueMakeUndefined(ctx);
			}
		}
	}
}
