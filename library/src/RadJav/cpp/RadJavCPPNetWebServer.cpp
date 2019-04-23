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
#include "cpp/RadJavCPPNetWebServer.h"
#include "cpp/RadJavCPPNetNetworkManager.h"
#include "cpp/RadJavCPPNetHttpConnection.h"

#include "RadJav.h"

#include <boost/asio/deadline_timer.hpp>
#include <boost/beast/websocket.hpp>


namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			namespace http = boost::beast::http;

			namespace
			{
				#ifdef USE_V8
				v8::Local<v8::Object> requestToJSHttpIncomingMessage(const boost::beast::http::request<boost::beast::http::string_body>& request,
																	 bool upgradeRequest = false)
				{
					v8::Local<v8::Object> httpMsgObj = V8_JAVASCRIPT_ENGINE->v8CreateNewObject("RadJav.Net.HttpIncomingMessage");
					
					V8_JAVASCRIPT_ENGINE->v8SetString(httpMsgObj,
													  "method",
													  {request.method_string().data(), request.method_string().size()});
					
					V8_JAVASCRIPT_ENGINE->v8SetString(httpMsgObj,
													  "url",
													  {request.target().data(), request.target().size()});
					
					auto versionMajor = request.version()/10;
					auto versionMinor = request.version()%10;
					
					std::ostringstream version;
					version << versionMajor << "." << versionMinor;
					
					V8_JAVASCRIPT_ENGINE->v8SetString(httpMsgObj, "httpVersion", version.str());
					V8_JAVASCRIPT_ENGINE->v8SetNumber(httpMsgObj, "httpVersionMajor", versionMajor);
					V8_JAVASCRIPT_ENGINE->v8SetNumber(httpMsgObj, "httpVersionMinor", versionMinor);
					
					v8::Local<v8::Object> headersObj = V8_JAVASCRIPT_ENGINE->v8CreateNewObject("Object");
					
					String headerName;
					for(const auto& field: request)
					{
						headerName = {field.name_string().data(), field.name_string().size()};
						V8_JAVASCRIPT_ENGINE->v8SetString(headersObj,
														  headerName.toLowerCase(),
														  {field.value().data(), field.value().size()});
					}
					
					V8_JAVASCRIPT_ENGINE->v8SetObject(httpMsgObj, "headers", headersObj);
					
					auto payloadSize = request.payload_size();
					if (payloadSize)
					{
						auto size = payloadSize.value();
						
						if (size)
						{
							auto dataArrayObj = v8::ArrayBuffer::New(V8_JAVASCRIPT_ENGINE->isolate, size);
							
							std::memcpy(dataArrayObj->GetContents().Data(), request.body().data(), size);
							
							V8_JAVASCRIPT_ENGINE->v8SetObject(httpMsgObj, "payload", dataArrayObj);
						}
					}
					
					// Just store request and disable delete on it
					// It will only be used for creating WebSocketConnection from HttpConnection
					if (upgradeRequest)
					{
						std::shared_ptr<boost::beast::http::request<boost::beast::http::string_body>> requestCopy(RJNEW boost::beast::http::request<boost::beast::http::string_body>(request),
																												  [](boost::beast::http::request<boost::beast::http::string_body>* obj) {
																													  DELETEOBJ(obj);
																												  });
						
						V8_JAVASCRIPT_ENGINE->v8SetExternal(httpMsgObj, "_appObj", requestCopy);
					}
					
					return httpMsgObj;
				}
				
				bool webServerResponseToCppResponse(v8::Local<v8::Object> httpResponseObj,
													boost::beast::http::response<boost::beast::http::string_body>& response)
				{
					RJINT statusCode = V8_JAVASCRIPT_ENGINE->v8GetInt(httpResponseObj, "statusCode");
					auto status = boost::beast::http::int_to_status(statusCode);
					
					if (status == boost::beast::http::status::unknown)
						return false;
					
					RJINT httpVersionMajor = V8_JAVASCRIPT_ENGINE->v8GetInt(httpResponseObj, "httpVersionMajor");
					RJINT httpVersionMinor = V8_JAVASCRIPT_ENGINE->v8GetInt(httpResponseObj, "httpVersionMinor");
					
					int httpVersion = httpVersionMajor*10 + httpVersionMinor;
					if (httpVersion > 100 ||
						httpVersion < 10)
					{
						return false;
					}
					
					response = {status, httpVersion};
					
					String statusMessage = V8_JAVASCRIPT_ENGINE->v8GetString(httpResponseObj, "statusMessage");
					if (!statusMessage.empty())
					{
						response.base().reason(statusMessage);
					}
					
					auto headersJS = V8_JAVASCRIPT_ENGINE->v8GetObject(httpResponseObj, "headers");
					auto properties = headersJS->GetOwnPropertyNames();
					for (int i = 0; i < properties->Length(); i++)
					{
						auto propertyNameJs = properties->Get(i);
						String name = parseV8Value(propertyNameJs);
						
						auto propertyValueJs = V8_JAVASCRIPT_ENGINE->v8GetObject(headersJS, name);
						
						if (propertyValueJs->IsString())
						{
							String value = parseV8Value(propertyValueJs);
							response.set(name, value);
						}
						else if (propertyValueJs->IsArray())
						{
							Array<String> values;
							v8::Local<v8::Array> valuesArrayJs = v8::Local<v8::Array>::Cast(propertyValueJs);
							for (int j = 0; j < valuesArrayJs->Length(); j++)
							{
								auto valueJs = valuesArrayJs->Get(j);
								if (valueJs->IsString())
								{
									response.insert(name, parseV8Value(valueJs));
								}
							}
						}
					}
					
					auto payloadJs = V8_JAVASCRIPT_ENGINE->v8GetValue(httpResponseObj, "payload");
					String data;
					
					if (payloadJs->IsString())
					{
						data = parseV8Value(payloadJs);
					}
					else if (payloadJs->IsObject())
					{
						auto payloadArrayBufferJs = v8::Local<v8::ArrayBuffer>::Cast(payloadJs);
						data = {(char*)payloadArrayBufferJs->GetContents().Data(), payloadArrayBufferJs->ByteLength()};
					}

					if (!data.empty())
					{
						response.body() = data;
						response.prepare_payload();
					}

					return true;
				}

				#elif defined USE_JAVASCRIPTCORE
					#pragma message ("Missing implementation of requestToJSHttpIncomingMessage and webServerResponseToCppResponse for JSC")
				#endif
			}
			
			class HttpConnectionListener : public std::enable_shared_from_this<HttpConnectionListener>
			{
			public:
				HttpConnectionListener(int maxPendingConnections, NetworkManager& networkManager)
				: _networkManager(networkManager)
				, _acceptor(_networkManager.getContext())
				{
					_maxPendingConnections = maxPendingConnections;
					_shutdown = false;
				}
				
				~HttpConnectionListener()
				{
					_networkManager.contextReleased(_acceptor.get_io_context());

					if (_errorCode.value())
					{
						notifyOnError(_errorCode);
					}
					else
					{
						notifyOnStop();
					}
				}

				void start(const std::string& address,
						   unsigned short port,
						   const std::function<void(std::shared_ptr<HttpConnection>)>& onConnectionHandler,
						   const std::function<void(void)>& onStopHandler,
						   const std::function<void(int, const std::string&)>& onErrorHandler)
				{
					_onConnection = onConnectionHandler;
					_onStop = onStopHandler;
					_onError = onErrorHandler;
					
					if (_acceptor.is_open())
						return;
					
					auto const addr = boost::asio::ip::make_address(address);
					const boost::asio::ip::tcp::endpoint endpoint(addr, port);
					
					boost::system::error_code ec;
					boost::system::error_code ec_unused;

					// Open the acceptor
					_acceptor.open(endpoint.protocol(), ec);
					if (ec)
					{
						_errorCode = ec;
						return;
					}
					
					// Allow address reuse
					_acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
					if (ec)
					{
						_acceptor.close(ec_unused);
						
						_errorCode = ec;
						return;
					}
					
					// Bind to the server address
					_acceptor.bind(endpoint, ec);
					if (ec)
					{
						_acceptor.close(ec_unused);
						
						_errorCode = ec;
						return;
					}
					
					// Start listening for connections
					int pendingConnections = _maxPendingConnections;
					if (pendingConnections < 0 ||
						pendingConnections > boost::asio::socket_base::max_listen_connections)
					{
						pendingConnections = boost::asio::socket_base::max_listen_connections;
					}

					_acceptor.listen(pendingConnections, ec);
					if (ec)
					{
						_acceptor.close(ec_unused);
						
						_errorCode = ec;
						return;
					}
					
					_acceptor.async_accept(_networkManager.getContext(),
										   _peerEndpoint,
										   std::bind(&HttpConnectionListener::onConnectionCallback,
													 shared_from_this(),
													 std::placeholders::_1,
													 std::placeholders::_2));
					
					_networkManager.activateContext(_acceptor.get_io_context());
				}
				
				void stop()
				{
					if (_shutdown)
						return;
					
					_shutdown = true;
					
					if (_acceptor.is_open())
					{
						boost::system::error_code ec_unused;
						_acceptor.close(ec_unused);
					}
				}
				
				boost::asio::io_context& getContext()
				{
					return _acceptor.get_io_context();
				}
				
				void clearCallbacks()
				{
					_onConnection = nullptr;
					_onError = nullptr;
					_onStop = nullptr;
				}
				
			private:
				void onConnectionCallback(boost::system::error_code ec,
										  boost::asio::ip::tcp::socket peer)
				{
					if (ec)
					{
						boost::system::error_code ec_unused;
						_acceptor.close(ec_unused);
						
						if (!_shutdown)
							_errorCode = ec;
						
						return;
					}

					auto connection = std::make_shared<HttpConnection>(_networkManager, std::move(peer));
					notifyOnConnection(connection);
					
					// Accept another connection
					_acceptor.async_accept(_networkManager.getContext(),
										   _peerEndpoint,
										   std::bind(&HttpConnectionListener::onConnectionCallback,
													 shared_from_this(),
													 std::placeholders::_1,
													 std::placeholders::_2));
				}
				
				void notifyOnError(boost::system::error_code& ec)
				{
					if (_onError)
						_onError(ec.value(), ec.message());
				}
				
				void notifyOnConnection(std::shared_ptr<HttpConnection> peer)
				{
					if (_onConnection)
						_onConnection(peer);
				}

				void notifyOnStop()
				{
					if (_onStop)
						_onStop();
				}
				
			private:
				std::function<void(std::shared_ptr<HttpConnection>)> _onConnection;
				std::function<void(void)> _onStop;
				std::function<void(int, const std::string&)> _onError;

				NetworkManager& _networkManager;
				boost::asio::ip::tcp::acceptor _acceptor;
				boost::system::error_code _errorCode;
				
				int _maxPendingConnections;
				bool _shutdown;
				boost::asio::ip::tcp::endpoint _peerEndpoint;
			};

			#ifdef USE_V8
				WebServer::WebServer(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: _networkManager(*jsEngine->networkManager)
				, _port(80)
				, _maxPendingConnections(-1)
				{}
			#elif defined USE_JAVASCRIPTCORE
				WebServer::WebServer(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
				: _networkManager(*jsEngine->networkManager)
				, _port(80)
				, _maxPendingConnections(-1)
				{}
			#endif

			WebServer::WebServer(int maxPendingConnections, NetworkManager& networkManager)
			: _networkManager(networkManager)
			, _port(80)
			, _maxPendingConnections(maxPendingConnections)
			{
			}

			WebServer::~WebServer()
			{
				auto listenerShared = _listener.lock();
				if (listenerShared)
				{
					listenerShared->clearCallbacks();
					listenerShared->stop();
				}
				
				for(auto httpConnection : _clients)
				{
					httpConnection->clearCallbacks();
					httpConnection->close();
				}
				
				// Events designed mostly for GUI and they will be freed by GUI library
				// but in our case where we are not relying on GUI here we need to remove events manually
				for(auto it = events->begin(); it != events->end(); it++)
				{
					DELETEOBJ(it->second);
				}
			}

			void WebServer::start(const std::string& address, unsigned short port)
			{
				if (_listener.lock())
					return;
				
				auto listener = std::make_shared<HttpConnectionListener>(_maxPendingConnections, _networkManager);
				_listener = listener;
				
				listener->start(address,
								port,
								std::bind(&WebServer::onListenerNewConnectionCallback, this, std::placeholders::_1),
								std::bind(&WebServer::onListenerStopCallback, this),
								std::bind(&WebServer::onListenerErrorCallback, this, std::placeholders::_1, std::placeholders::_2));
			}

			void WebServer::stop()
			{
				auto listenerShared = _listener.lock();
				if (listenerShared)
					listenerShared->stop();

				for(auto httpConnection : _clients)
				{
					httpConnection->close();
				}
			}
			
			void WebServer::onProcess(const std::function<void(const http::request<http::string_body>&,
															   http::response<http::string_body>&)>& func)
			{
				_onProcess = func;
			}
			
			void WebServer::onUpgrade(const std::function<void(HttpConnection&,
															   const http::request<http::string_body>&)>& func)
			{
				_onUpgrade = func;
			}

			void WebServer::onStop(const std::function<void(void)>& func)
			{
				_onStop = func;
			}
			
			void WebServer::onError(const std::function<void(int, const std::string&)>& func)
			{
				_onError = func;
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void WebServer::on(String event, RJ_FUNC_TYPE func)
				{
					createEvent(event, func);
				}
			#endif

			void WebServer::onListenerNewConnectionCallback(std::shared_ptr<HttpConnection> connection)
			{
				_clients.push_back(connection);
				
				connection->onRequest(std::bind(&WebServer::onHttpProcessCallback, this,
												std::placeholders::_1,
												std::placeholders::_2,
												std::placeholders::_3));
				
				connection->onError(std::bind(&WebServer::onHttpErrorCallback, this,
											  std::placeholders::_1,
											  std::placeholders::_2,
											  std::placeholders::_3));
				
				connection->onUpgrade(std::bind(&WebServer::onHttpUpgradeCallback, this,
												std::placeholders::_1,
												std::placeholders::_2));

				connection->onUpgraded(std::bind(&WebServer::onHttpUpgradedCallback, this,
												 std::placeholders::_1));

				connection->onClose(std::bind(&WebServer::onHttpCloseCallback, this,
											  std::placeholders::_1));
			}
			
			void WebServer::onListenerStopCallback()
			{
				// If we use WebServer from C++ side
				// Then process only C++ callbacks
				if (_onStop)
				{
					_onStop();
					return;
				}
				
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					executeEvent("close");
				#endif
			}
			
			void WebServer::onListenerErrorCallback(int errorCode, const std::string& description)
			{
				// If we use WebServer from C++ side
				// Then process only C++ callbacks
				if (_onError)
				{
					_onError(errorCode, description);
					return;
				}
				
				#ifdef USE_V8
					v8::Local<v8::Value> args[2];
					args[0] = v8::Number::New(V8_JAVASCRIPT_ENGINE->isolate, errorCode);
					args[1] = String(description).toV8String(V8_JAVASCRIPT_ENGINE->isolate);
				
					executeEvent("error", 2, args);
				
				#elif defined USE_JAVASCRIPTCORE
					#pragma message("Missing implementation of WebServer.error callback for JSC")
					//TODO: add JSC implementation
					//executeEvent("error", description);
				#endif
			}
			
			void WebServer::onHttpProcessCallback(HttpConnection& connection,
												  const boost::beast::http::request<boost::beast::http::string_body>& request,
												  boost::beast::http::response<boost::beast::http::string_body>& response)
			{
				// If we use WebServer from C++ side
				// Then process only C++ callbacks
				if (_onProcess)
				{
					_onProcess(request, response);
					return;
				}
				
				#pragma message("Missing implementation of onHttpRequestCallback for Javascript")
				#ifdef USE_V8
					auto httpMsgObj = requestToJSHttpIncomingMessage(request);
				
					auto httpResponseObj = V8_JAVASCRIPT_ENGINE->v8CreateNewObject("RadJav.Net.WebServerResponse");
				
					v8::Local<v8::Value> args[2];
					args[0] = httpMsgObj;
					args[1] = httpResponseObj;
				
					executeEvent("process", 2, args);
				
					if (!webServerResponseToCppResponse(httpResponseObj, response))
					{
						response = {boost::beast::http::status::internal_server_error, request.version()};
						return;
					}
				
				#elif defined USE_JAVASCRIPTCORE
					executeEvent("process");
				#endif
			}
			
			void WebServer::onHttpUpgradeCallback(HttpConnection& connection,
												  const boost::beast::http::request<boost::beast::http::string_body>& request)
			{
				// If we use WebServer from C++ side
				// Then process only C++ callbacks
				if (_onUpgrade)
				{
					_onUpgrade(connection, request);
					return;
				}

				#ifdef USE_V8
					auto httpMsgObj = requestToJSHttpIncomingMessage(request, true);
				
					auto httpConnectionObj = V8_JAVASCRIPT_ENGINE->v8CreateNewObject("RadJav.Net.HttpConnection");
				
					// Do not allow removal of HttpConnection when GC do it's work
					// HttpConnection will be freed after completed request
					std::shared_ptr<HttpConnection> connectionPtr(&connection,
																  [](HttpConnection* p) {
																  });

					V8_JAVASCRIPT_ENGINE->v8SetExternal(httpConnectionObj, "_appObj", connectionPtr);
				
					v8::Local<v8::Value> args[2];
					args[0] = httpConnectionObj;
					args[1] = httpMsgObj;
				
					executeEvent("upgrade", 2, args);
				
				#elif defined USE_JAVASCRIPTCORE
					#pragma message("Missing implementation of WebServer.upgrade callback for JSC")
					executeEvent("upgrade");
				#endif
			}

			void WebServer::onHttpUpgradedCallback(HttpConnection& connection)
			{
				removeClient(connection);
			}

			void WebServer::onHttpCloseCallback(HttpConnection& connection)
			{
				removeClient(connection);
			}
			
			void WebServer::onHttpErrorCallback(HttpConnection& connection,
												int errorCode, const std::string& description)
			{
				removeClient(connection);
			}
			
			void WebServer::removeClient(const HttpConnection& connection)
			{
				auto client = std::find_if(_clients.begin(), _clients.end(),
										   [&](std::shared_ptr<HttpConnection> con) -> bool {
											   return con.get() == &connection;
										   });
				
				if (client != _clients.end())
					_clients.erase(client);
			}
		}
	}
}
