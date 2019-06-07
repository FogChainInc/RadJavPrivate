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

//#include "cpp/RadJavCPPNetWebSocketServer.h"
#include "cpp/RadJavCPPNetWebSocketConnection.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace Net
		{
			using CppWebSocketConnection = CPP::Net::WebSocketConnection;

//			void WebSocketServer::createV8Callbacks(JSContextRef context, JSObjectRef object)
//			{
//				JSC_CALLBACK(object, "_init", WebSocketServer::_init);
//
//				JSC_CALLBACK(object, "listen", WebSocketServer::listen);
//				JSC_CALLBACK(object, "sendToAll", WebSocketServer::sendToAll);
//				JSC_CALLBACK(object, "send", WebSocketServer::send);
//				JSC_CALLBACK(object, "receive", WebSocketServer::receive);
//				JSC_CALLBACK(object, "close", WebSocketServer::close);
//
//				JSC_CALLBACK(object, "onReceive", WebSocketServer::onReceive);
//				JSC_CALLBACK(object, "onAccept", WebSocketServer::onAccept);
//
//				JSC_CALLBACK(object, "on", WebSocketServer::on);
//			}
//
//			JSValueRef WebSocketServer::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				CPP::Net::WebSocketServer *webSocket = RJNEW CPP::Net::WebSocketServer();
//				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_webSocket", webSocket);
//
//				return JSValueMakeUndefined(ctx);
//			}
//
//			JSValueRef WebSocketServer::listen(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				JSValueRef undefined = JSValueMakeUndefined(ctx);
//
//				CPP::Net::WebSocketServer * webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
//
//				JSValueRef portArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
//
//				if (!portArg)
//				{
//					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Port cannot be null!");
//					return undefined;
//				}
//
//				RJINT port = JSC_JAVASCRIPT_ENGINE->jscValueToInt(ctx, portArg);
//
//				JSC_JAVASCRIPT_ENGINE->jscSetNumber(thisObject, "port", port);
//
//				if (webSocket)
//					webSocket->listen(port);
//
//				return undefined;
//			}
//
//			JSValueRef WebSocketServer::sendToAll(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				JSValueRef undefined = JSValueMakeUndefined(ctx);
//
//				CPP::Net::WebSocketServer * webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
//
//				if (!webSocket)
//					return undefined;
//
//				JSValueRef val = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
//				if (!val)
//					return undefined;
//
//				String str = parseJSCValue(ctx, val);
//
//				webSocket->sendToAll(str);
//
//				return undefined;
//			}
//
//			JSValueRef WebSocketServer::send(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				JSValueRef undefined = JSValueMakeUndefined(ctx);
//
//				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
//
//				if (!webSocket)
//					return undefined;
//
//				JSValueRef valId = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
//				JSValueRef valMsg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
//
//				if (!valId || !valMsg)
//					return undefined;
//
//				String id = parseJSCValue(ctx, valId);
//				String message = parseJSCValue(ctx, valMsg);
//
//				webSocket->send(id, message);
//
//				return undefined;
//			}
//
//			JSValueRef WebSocketServer::receive(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
//
//				if (webSocket)
//				{
//					auto resp = webSocket->receive();
//					JSValueRef ret = resp.toJSCValue(ctx);
//					return ret;
//				}
//
//				return String("").toJSCValue(ctx);
//			}
//
//			JSValueRef WebSocketServer::close(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
//
//				if (webSocket)
//					webSocket->close();
//
//				if (RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent)
//				{
//					JSValueUnprotect(ctx, RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent);
//					RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent = nullptr;
//				}
//
//				if (RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent)
//				{
//					JSValueUnprotect(ctx, RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent);
//					RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent = nullptr;
//				}
//
//				return JSValueMakeUndefined(ctx);
//			}
//
//			JSValueRef WebSocketServer::onAccept(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				JSValueRef undefined = JSValueMakeUndefined(ctx);
//
//				JSValueRef func = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
//
//				if (!func)
//					return undefined;
//
//				JSValueProtect(ctx, func);
//
//				RadJAV::CPP::Net::WebSocketServer::m_serverAcceptEvent = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(func);
//
//				return undefined;
//			}
//
//			JSValueRef WebSocketServer::onReceive(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				JSValueRef undefined = JSValueMakeUndefined(ctx);
//
//				JSValueRef func = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
//
//				if (!func)
//					return undefined;
//
//				JSValueProtect(ctx, func);
//
//				RadJAV::CPP::Net::WebSocketServer::m_serverReceiveEvent = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(func);
//
//				return undefined;
//			}
//
//			JSValueRef WebSocketServer::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
//			{
//				JSValueRef undefined = JSValueMakeUndefined(ctx);
//
//				CPP::Net::WebSocketServer *webSocket = (CPP::Net::WebSocketServer *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webSocket");
//
//				JSValueRef eventArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
//				JSValueRef funcArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
//
//				if (JSC_JAVASCRIPT_ENGINE->jscIsNull(eventArg) ||
//					JSC_JAVASCRIPT_ENGINE->jscIsNull(funcArg))
//				{
//					return undefined;
//				}
//
//				String event = parseJSCValue(ctx, eventArg);
//
//				if (webSocket)
//					webSocket->on(event, JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(funcArg));
//
//				return undefined;
//			}

			void WebSocketConnection::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", WebSocketConnection::init);
				JSC_CALLBACK(object, "_connect", WebSocketConnection::connect);
				JSC_CALLBACK(object, "_send", WebSocketConnection::send);
				JSC_CALLBACK(object, "_close", WebSocketConnection::close);
				JSC_CALLBACK(object, "_on", WebSocketConnection::on);
			}

			JSValueRef WebSocketConnection::init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<CppWebSocketConnection> webSocket;
				
				if (argumentCount >= 2 &&
					JSValueIsObject(ctx, arguments[0]) &&
					JSValueIsObject(ctx, arguments[1]))
				{
					auto httpConnectionObj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(arguments[0]);
					auto httpRequestObj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(arguments[1]);
					
					auto httpConnection = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CPP::Net::HttpConnection>(ctx, httpConnectionObj, "_appObj");
					auto httpRequest = JSC_JAVASCRIPT_ENGINE->jscGetExternal<boost::beast::http::request<boost::beast::http::string_body>>(ctx, httpRequestObj, "_appObj");
					
					if (httpConnection && httpRequest)
					{
						auto webSocketConnection = CppWebSocketConnection::handleUpgrade(httpConnection.get(), *httpRequest.get());
						if (webSocketConnection)
						{
							webSocket.reset( webSocketConnection,
											[](CppWebSocketConnection* p) {
												DELETEOBJ(p);
											});
						}
					}
					
					if (!webSocket)
					{
						JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Unable to handle WebSocket upgrade");
						return undefined;
					}
				}
				else
				{
					webSocket.reset(RJNEW CppWebSocketConnection(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments),
									[](CppWebSocketConnection* p) {
										DELETEOBJ(p);
									});
				}
				
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", webSocket);
				
				return undefined;
			}

			JSValueRef WebSocketConnection::connect(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<CppWebSocketConnection> webSocket = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppWebSocketConnection>(ctx, thisObject, "_appObj");
				
				if (!webSocket)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "WebSocketConnection not initialized");
					return undefined;
				}
				
				JSValueRef urlJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!urlJs ||
					!JSValueIsString(ctx, urlJs))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "URL is required");
					return undefined;
				}
				
				webSocket->connect(parseJSCValue(ctx, urlJs));

				return undefined;
			}

			JSValueRef WebSocketConnection::send(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				std::shared_ptr<CppWebSocketConnection> webSocket = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppWebSocketConnection>(ctx, thisObject, "_appObj");
				
				if (!webSocket)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "WebSocketConnection not initialized");
					return undefined;
				}
				
				JSValueRef dataJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);

				if (!dataJs)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Argument data is required");
					return undefined;
				}
				
				String data = JSC_JAVASCRIPT_ENGINE->jscGetArgumentAsString(ctx, arguments, argumentCount, 0);
				
				if (data.empty())
					return undefined;
				
				webSocket->send(data);
				
				return undefined;
			}

			JSValueRef WebSocketConnection::close(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				std::shared_ptr<CppWebSocketConnection> webSocket = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppWebSocketConnection>(ctx, thisObject, "_appObj");
				
				if (!webSocket)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "WebSocketConnection not initialized");
					return undefined;
				}
				
				webSocket->close();
				
				return undefined;
			}

			JSValueRef WebSocketConnection::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				std::shared_ptr<CppWebSocketConnection> webSocket = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppWebSocketConnection>(ctx, thisObject, "_appObj");
				
				if (!webSocket)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "WebSocketConnection not initialized");
					return undefined;
				}
				
				JSValueRef eventJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef funcJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				JSObjectRef funcObjJs = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, funcJs);
				
				if (!eventJs ||
					!funcObjJs ||
					!JSValueIsString(ctx, eventJs) ||
					!JSObjectIsFunction(ctx, funcObjJs))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Event name and function is required");
					return undefined;
				}
				
				webSocket->on(parseJSCValue(ctx, eventJs), funcObjJs);
				
				return undefined;
			}
		}
	}
}
