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
#ifndef _RADJAV_CPP_NET_WEBSERVER_H_
	#define _RADJAV_CPP_NET_WEBSERVER_H_

	#include "RadJavPreprocessor.h"

	#include "RadJavString.h"
	#include "cpp/RadJavCPPEvent.h"

	#include <boost/beast/http.hpp>

	#include <functional>
	#include <memory>
	#include <vector>

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				class HttpConnectionListener;
				class HttpConnection;

				/**
				 * @ingroup group_net_cpp
				 * @brief WebServer that handle HTTP requests and has the ability to notify on WebSocket
				 * connections
				 */
				class RADJAV_EXPORT WebServer: public Events
				{
					public:
						#ifdef USE_V8
							WebServer(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#elif defined USE_JAVASCRIPTCORE
							WebServer(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[]);
						#endif

						/**
						 * @brief A constructor
						 * @param[in] maxPendingConnections maximum size of queue of pending connections. Negative for unlimited size.
						 * @param[in] contextManager context manager to be used for handling contexts
						 */
						WebServer(int maxPendingConnections, ContextManager& contextManager);

						/**
						 * @brief A destructor
						 * @details All active connections will be closed gracefully except WebSocket connections
						 * ownership of which transferred to the user
						 */
						~WebServer();

						/**
						 * @brief Start listening for connections and process requests
						 * @param[in] address address on which listener will be bound
						 * @param[in] port port on which listener will be bound
						 */
						void start(const std::string& address, unsigned short port);

						/**
						 * @brief Stop the webserver and close all active HTTP connections
						 */
						void stop();

						/**
						 * @brief Callback for processing HTTP requests
						 * @details If user will not fill the response then server will respond with 400 error message to the client
						 * automatically
						 */
						void onProcess(const std::function<void(const boost::beast::http::request<boost::beast::http::string_body>&,
																boost::beast::http::response<boost::beast::http::string_body>&)>& func);

						/**
						 * @brief Callback for handling WebSocket upgrade request
						 * @details To make a WebSocketConnection from HttpConnection user can use WebSocketConnection::handleUpgrade method
						 * @note If user will not create a WebSocketConnection server will automatically reply with 400 error message to the client
						 */
						void onUpgrade(const std::function<void(HttpConnection&,
																const boost::beast::http::request<boost::beast::http::string_body>&)>& func);
					
						/**
						 * @brief Callback for handling when WebServer has been stopped
						 */
						void onStop(const std::function<void(void)>& func);
					
						/**
						 * @brief Callback for handling the error
						 */
						void onError(const std::function<void(int, const std::string&)>& func);
					
						/**
						 * @brief Register Javascript callback
						 * @details Set the Javascript handler function for specific notification
						 * @param[in] event name of the event. Can be "process", "upgrade", "error" and "stop"
						 * @param[in] func Javascript callback function
						 */
						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
							void on(String event, RJ_FUNC_TYPE func);
						#endif

					private:
						void removeClient(const HttpConnection& connection);
					
						void onListenerNewConnectionCallback(std::shared_ptr<HttpConnection> connection);
						void onListenerStopCallback();
						void onListenerErrorCallback(int errorCode, const std::string& description);
					
						void onHttpProcessCallback(HttpConnection& connection,
												   const boost::beast::http::request<boost::beast::http::string_body>& request,
												   boost::beast::http::response<boost::beast::http::string_body>& response);
					
						void onHttpUpgradeCallback(HttpConnection& connection,
												   const boost::beast::http::request<boost::beast::http::string_body>& request);
					
						void onHttpUpgradedCallback(HttpConnection& connection);
						void onHttpCloseCallback(HttpConnection& connection);
						void onHttpErrorCallback(HttpConnection& connection,
												 int errorCode, const std::string& description);

					private:
						std::function<void(const boost::beast::http::request<boost::beast::http::string_body>&,
										   boost::beast::http::response<boost::beast::http::string_body>&)> _onProcess;
					
						std::function<void(HttpConnection&,
										   const boost::beast::http::request<boost::beast::http::string_body>&)> _onUpgrade;
						
						std::function<void(void)> _onStop;
						std::function<void(int, const std::string&)> _onError;
					
						ContextManager& _contextManager;
						std::string _address;
						unsigned short _port;
						int _maxPendingConnections;
					
						std::weak_ptr<HttpConnectionListener> _listener;
						std::vector<std::shared_ptr<HttpConnection>> _clients;
				};
			}
		}
	}

#endif // _RADJAV_CPP_NET_WEBSERVER_H_
