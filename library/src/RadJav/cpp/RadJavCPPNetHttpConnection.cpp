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
#include "cpp/RadJavCPPNetHttpConnection.h"
#include "cpp/RadJavCPPContextManager.h"

#include "RadJav.h"

#include <boost/asio/deadline_timer.hpp>
#include <boost/beast/websocket.hpp>


namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			using tcp = boost::asio::ip::tcp;
			namespace http = boost::beast::http;
			
			class HttpConnectionImpl : public std::enable_shared_from_this<HttpConnectionImpl>
			{
			public:
				HttpConnectionImpl(ContextManager& contextManager,
								   boost::asio::ip::tcp::socket&& socket,
								   int timeoutInMilliseconds)
				: _contextManager(contextManager)
				, _socket(std::move(socket))
				, _inactivityTimer(_socket.get_io_context())
				{
					_connectionTimeout = timeoutInMilliseconds;
					_shutdown = false;
					_upgradeRequested = false;
				}
				
				~HttpConnectionImpl()
				{
					_contextManager.contextReleased(_socket.get_io_context());
					
					int ec = _errorCode.value();
					if (ec == 0)
					{
						//Normal close
						if (_onClosed)
							_onClosed();
					}
					else
					{
						//Close with error
						if (_onError)
							_onError(_errorCode.value(), _errorCode.message());
					}
				}
				
				void run()
				{
					if (_shutdown)
						return;
					
					// Make the request empty before reading,
					// otherwise the operation behavior is undefined.
					_request = {};
					
					
					// Read a request
					http::async_read(_socket, _buffer, _request,
									 std::bind(
											   &HttpConnectionImpl::internalOnRead,
											   shared_from_this(),
											   std::placeholders::_1,
											   std::placeholders::_2));
					
					startTimeout(_connectionTimeout);
					
					_contextManager.activateContext(_socket.get_io_context());
				}
				
				void close()
				{
					if (!_shutdown)
					{
						_shutdown = true;
						
						cancelTimeout();
						
						//_socket.shutdown(boost::asio::socket_base::shutdown_both, ec_unused);
						_socket.close();
					}
				}
				
				void onRequest(const std::function<void(const http::request<http::string_body>&, http::response<http::string_body>&)>& func)
				{
					_onRequest = func;
				}
				
				void onUpgrade(const std::function<void(const http::request<http::string_body>&)>& func)
				{
					_onUpgrade = func;
				}
				
				void onUpgraded(const std::function<void(void)>& func)
				{
					_onUpgraded = func;
				}
				
				void onError(const std::function<void(int, const std::string&)>& func)
				{
					_onError = func;
				}
				
				void onClosed(const std::function<void(void)>& func)
				{
					_onClosed = func;
				}
				
				void upgradeHandled()
				{
					if (_shutdown)
						return;
					
					_upgradeRequested = false;
				}
				
				boost::asio::ip::tcp::socket* getSocket()
				{
					if (_shutdown)
						return nullptr;
					
					return &_socket;
				}
				
				void clearCallbacks()
				{
					_onUpgraded = nullptr;
					_onUpgrade = nullptr;
					_onRequest = nullptr;
					_onError = nullptr;
					_onClosed = nullptr;
				}
				
			private:
				void startTimeout(int timeoutInMilliseconds)
				{
					cancelTimeout();
					
					if (timeoutInMilliseconds < 0)
						return;
					
					_inactivityTimer.expires_from_now(boost::posix_time::milliseconds(timeoutInMilliseconds));
					_inactivityTimer.async_wait(std::bind(&HttpConnectionImpl::internalOnTimeout,
														  shared_from_this(),
														  std::placeholders::_1));
				}
				
				void cancelTimeout()
				{
					boost::system::error_code ec_unused;
					_inactivityTimer.cancel(ec_unused);
				}
				
				void send(http::response<http::string_body>& response)
				{
					// Send the response
					http::async_write(_socket,
									  response,
									  std::bind(&HttpConnectionImpl::internalOnWrite,
												shared_from_this(),
												std::placeholders::_1,
												std::placeholders::_2));
				}
				
				void close(boost::system::error_code ec)
				{
					if (_shutdown ||
						_errorCode.value() != 0)
						return;
					
					_shutdown = true;
					
					cancelTimeout();
					
					if (ec == http::error::end_of_stream)
					{
						_errorCode = ec;
						return;
					}
					
					_errorCode = ec;
					
					//_socket.shutdown(boost::asio::socket_base::shutdown_both, ec_unused);
					_socket.close();
				}
				
				void internalOnRead(boost::system::error_code ec,
									std::size_t bytes_transferred)
				{
					cancelTimeout();

					// Prepare a bad request response
					auto const badRequestResponse = [](const http::request<http::string_body>& request,
													   boost::string_view reason) {
						http::response<http::string_body> res{ http::status::bad_request, request.version() };
						res.set(http::field::content_type, "text/html");
						res.keep_alive(false);
						
						std::ostringstream body;
						body << "<html><head><title>400 Bad Request</title></head><body bgcolor=\"white\"><center><h1>";
						body << reason;
						body << "</h1></center></body></html>";
						res.body() =  body.str();

						res.prepare_payload();
						return res;
					};

					if(ec)
					{
						if (std::string(ec.category().name()) == "beast.http")
						{
							boost::beast::http::error errorCode = (boost::beast::http::error)ec.value();
							
							if (errorCode != boost::beast::http::error::end_of_stream)
							{
								_response = badRequestResponse(_request, ec.message());

								// By setting _errorCode we will close socket
								// on next call to internalOnWrite
								_errorCode = ec;
								send(_response);
								return;
							}
						}
						
						close(ec);
						return;
					}
					
					// See if it is a WebSocket Upgrade
					if (boost::beast::websocket::is_upgrade(_request))
					{
						if (_onUpgrade)
						{
							_upgradeRequested = true;
							
							_onUpgrade(_request);
							
							if (!_upgradeRequested)
							{
								if (_onUpgraded)
									_onUpgraded();
								
								_buffer.consume(bytes_transferred);
								return;
							}
						}
						
						_response = badRequestResponse(_request, "Not supported");

						send(_response);
						
						_buffer.consume(bytes_transferred);
						return;
					}
					
					if (_onRequest)
					{
						// Bad request response by default
						_response = badRequestResponse(_request, "Not supported");

						_onRequest(_request, _response);
						
						send(_response);
					}
					
					_buffer.consume(bytes_transferred);
				}
				
				void internalOnWrite(boost::system::error_code ec,
									 std::size_t bytes_transferred)
				{
					if (ec)
					{
						close(ec);
						return;
					}
					
					_response = {};
					
					// Make the request empty before reading,
					// otherwise the operation behavior is undefined.
					_request = {};

					// If we sent 400 error due to parsing issues
					// close the connection
					if (_errorCode.value())
					{
						close(_errorCode);
						return;
					}
					
					// Read a request
					http::async_read(_socket, _buffer, _request,
									 std::bind(&HttpConnectionImpl::internalOnRead,
											   shared_from_this(),
											   std::placeholders::_1,
											   std::placeholders::_2));
					
					startTimeout(_connectionTimeout);
				}
				
				void internalOnTimeout(const boost::system::error_code& ec)
				{
					if (ec)
					{
						return;
					}
					
					close();
				}
				
			private:
				std::function<void(void)> _onClosed;
				std::function<void(const http::request<http::string_body>&, http::response<http::string_body>&)> _onRequest;
				std::function<void(const http::request<http::string_body>&)> _onUpgrade;
				std::function<void(void)> _onUpgraded;
				std::function<void(int, const std::string&)> _onError;
				
				ContextManager& _contextManager;
				boost::asio::ip::tcp::socket _socket;
				http::request<http::string_body> _request;
				http::response<http::string_body> _response;
				boost::asio::deadline_timer _inactivityTimer;
				boost::beast::multi_buffer _buffer;
				boost::system::error_code _errorCode;
				int _connectionTimeout;
				bool _shutdown;
				bool _upgradeRequested;
			};
			
			HttpConnection::HttpConnection(ContextManager& contextManager,
										   boost::asio::ip::tcp::socket&& socket,
										   int timeoutInMilliseconds)
			: _contextManager(contextManager)
			{
				auto impl = std::make_shared<HttpConnectionImpl>(_contextManager, std::move(socket), timeoutInMilliseconds);
				_impl = impl;
				
				impl->onRequest(std::bind(&HttpConnection::onRequestCallback, this,
										  std::placeholders::_1,
										  std::placeholders::_2));
				
				impl->onUpgrade(std::bind(&HttpConnection::onUpgradeCallback, this,
										  std::placeholders::_1));
				
				impl->onUpgraded(std::bind(&HttpConnection::onUpgradedCallback, this));
				
				impl->onError(std::bind(&HttpConnection::onErrorCallback, this,
										std::placeholders::_1,
										std::placeholders::_2));
				
				impl->onClosed(std::bind(&HttpConnection::onClosedCallback, this));
				
				impl->run();
			}
			
			HttpConnection::~HttpConnection()
			{
				releaseImpl();
				
				// Events designed mostly for GUI and they will be freed by GUI library
				// but in our case where we are not relying on GUI here we need to remove events manually
				for(auto it = events->begin(); it != events->end(); it++)
				{
					DELETEOBJ(it->second);
				}
			}
			
			void HttpConnection::close()
			{
				auto impl = _impl.lock();
				
				if (impl)
					impl->close();
			}
			
			void HttpConnection::onRequest(const std::function<void(HttpConnection&,
																	const http::request<http::string_body>&,
																	http::response<http::string_body>&)>& func)
			{
				_onRequest = func;
			}
			
			void HttpConnection::onUpgrade(const std::function<void(HttpConnection&,
																	const http::request<http::string_body>&)>& func)
			{
				_onUpgrade = func;
			}
			
			void HttpConnection::onUpgraded(const std::function<void(HttpConnection&)>& func)
			{
				_onUpgraded = func;
			}
			
			void HttpConnection::onClose(const std::function<void(HttpConnection&)>& func)
			{
				_onClose = func;
			}
			
			void HttpConnection::onError(const std::function<void(HttpConnection&,
																  int, const std::string&)>& func)
			{
				_onError = func;
			}
			
			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void HttpConnection::on(String event, RJ_FUNC_TYPE func)
				{
					
				}
			#endif
			
			ContextManager& HttpConnection::getContextManager()
			{
				return _contextManager;
			}
			
			void HttpConnection::clearCallbacks()
			{
				_onRequest = nullptr;
				_onUpgrade = nullptr;
				_onUpgraded = nullptr;
				_onError = nullptr;
				_onClose = nullptr;
			}
			
			void HttpConnection::releaseImpl()
			{
				auto impl = _impl.lock();
				if (impl)
				{
					impl->clearCallbacks();
				}
			}
			
			void HttpConnection::upgradeHandled()
			{
				auto impl = _impl.lock();
				
				if (impl)
					impl->upgradeHandled();
			}
			
			boost::asio::ip::tcp::socket* HttpConnection::getSocket()
			{
				auto impl = _impl.lock();
				if (impl)
					return impl->getSocket();
				
				return nullptr;
			}
			
			void HttpConnection::onRequestCallback(const http::request<http::string_body>& request, http::response<http::string_body>& response)
			{
//				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
//				#pragma message("Missing implementation of onRequestCallback for Javascript side")
//					//executeEvent("request");
//				#endif
				
				if (_onRequest)
					_onRequest(*this, request, response);
			}
			
			void HttpConnection::onUpgradeCallback(const http::request<http::string_body>& request)
			{
				if (_onUpgrade)
					_onUpgrade(*this, request);
			}
			
			void HttpConnection::onUpgradedCallback()
			{
				releaseImpl();
				
				if (_onUpgraded)
					_onUpgraded(*this);
			}
			
			void HttpConnection::onClosedCallback()
			{
				releaseImpl();
				
//				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
//					executeEvent("close");
//				#endif
				
				if (_onClose)
					_onClose(*this);
			}
			
			void HttpConnection::onErrorCallback(int errorCode, const std::string& description)
			{
				releaseImpl();
				
//				#ifdef USE_V8
//					v8::Local<v8::Value> args[2];
//					args[0] = v8::Number::New(V8_JAVASCRIPT_ENGINE->isolate, errorCode);
//					args[1] = String(description).toV8String(V8_JAVASCRIPT_ENGINE->isolate);
//				
//					executeEvent("error", 2, args);
//				#elif defined USE_JAVASCRIPTCORE
//				#pragma message("Missing implementation of onErrorCallback for JSC")
//					//TODO: add JSC implementation
//					//executeEvent("error", description);
//				#endif
				
				if (_onError)
					_onError(*this, errorCode, description);
			}
		}
	}
}
