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
#include "cpp/RadJavCPPNetWebSocketConnection.h"

#include "RadJav.h"
#include "RadJavString.h"
#include "cpp/RadJavCPPContextManager.h"
#include "cpp/RadJavCPPNetHttpConnection.h"

#include <boost/beast/websocket.hpp>

#include <queue>


namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			class WebSocketConnectionImpl : public std::enable_shared_from_this<WebSocketConnectionImpl>
			{
				enum class State
				{
					Idle,
					ResolvingHost,
					Connecting,
					Handshaking,
					Active,
					Closing,
					ClosingDueToError
				};
			public:
				WebSocketConnectionImpl(ContextManager& contextManager)
				: _contextManager(contextManager)
				, _context(_contextManager.getContext())
				, _resolver(_context)
				, _webSocket(_context)
				, _state(State::Idle)
				{
				}
				
				WebSocketConnectionImpl(ContextManager& contextManager,
										boost::asio::ip::tcp::socket&& socket)
				: _contextManager(contextManager)
				, _context(socket.get_io_context())
				, _resolver(_context)
				, _webSocket(std::move(socket))
				, _state(State::Idle)
				{
				}

				~WebSocketConnectionImpl()
				{
					if (_errorCode.value())
					{
						notifyOnError(_errorCode);
					}
					else
					{
						notifyOnClosed();
					}
				}
				
				void connect(const std::string& host,
							 const std::string& port,
							 const std::string& resource)
				{
					if (_state != State::Idle)
						return;
					
					_host = host;
					_port = port;
					_resource = resource;
					
					_resolver.async_resolve(_host,
											_port,
											std::bind(&WebSocketConnectionImpl::internalOnResolve,
													  shared_from_this(),
													  std::placeholders::_1,
													  std::placeholders::_2));
					
					_state = State::ResolvingHost;
					
					_contextManager.activateContext(_context);
				}
				
				void accept(const http::request<http::string_body>& request)
				{
					if (_state != State::Idle)
						return;
					
					_upgradeRequest = request;
					_webSocket.async_accept(_upgradeRequest,
											std::bind(&WebSocketConnectionImpl::internalOnHandshake,
													  shared_from_this(),
													  std::placeholders::_1));

					_state = State::Handshaking;
					
					_contextManager.activateContext(_context);
				}

				void send(const std::string& data)
				{
					if (_state != State::Active)
						return;
					
					std::vector<unsigned char> message(data.size());
					std::copy(data.c_str(), data.c_str()+data.size(), message.begin());
					
					_outMessages.push(std::make_tuple(std::move(message), false));

					// Start write operation only if we have one message in queue
					// otherwise write operation will continue in internalOnWrite callback
					if (_outMessages.size() == 1)
					{
						_webSocket.binary(std::get<1>(_outMessages.front()));
						
						// Send the message
						_webSocket.async_write(boost::asio::buffer(std::get<0>(_outMessages.front())),
											   std::bind(&WebSocketConnectionImpl::internalOnWrite,
														 shared_from_this(),
														 std::placeholders::_1,
														 std::placeholders::_2));
					}
				}
				
				void send(const unsigned char* data, unsigned int size)
				{
					if (_state != State::Active)
						return;
					
					std::vector<unsigned char> message(size);
					std::copy(data, data+size, message.begin());
					
					_outMessages.push(std::make_tuple(std::move(message), true));
					
					// Start write operation only if we have one message in queue
					// otherwise write operation will continue in internalOnWrite callback
					if (_outMessages.size() == 1)
					{
						_webSocket.binary(std::get<1>(_outMessages.front()));
						
						// Send the message
						_webSocket.async_write(boost::asio::buffer(std::get<0>(_outMessages.front())),
											   std::bind(&WebSocketConnectionImpl::internalOnWrite,
														 shared_from_this(),
														 std::placeholders::_1,
														 std::placeholders::_2));
					}
				}
				
				void close()
				{
					switch(_state)
					{
						case State::Idle:
						case State::Closing:
						case State::ClosingDueToError:
							return;
						case State::ResolvingHost:
						{
							_state = State::Closing;
							_resolver.cancel();
							return;
						}
						default:;
					}
					
					// Close the WebSocket connection
					_webSocket.async_close(boost::beast::websocket::close_code::normal,
										   std::bind(&WebSocketConnectionImpl::internalOnClose,
													 shared_from_this(),
													 std::placeholders::_1));

					_state = State::Closing;
				}
				
				void onConnected(const std::function<void(void)>& func)
				{
					_onConnected = func;
				}
				
				void onClosed(const std::function<void(void)>& func)
				{
					_onClosed = func;
				}
				
				void onRead(const std::function<void(const std::string&)>& func,
							const std::function<void(const unsigned char*, std::size_t)>& funcBinary)
				{
					_onRead = func;
					_onReadBinary = funcBinary;
				}

				void onError(const std::function<void(int, const std::string&)>& func)
				{
					_onError = func;
				}
				
				void setAutoFragmentMessages(bool autoFragment)
				{
					_webSocket.auto_fragment(autoFragment);
				}
				
				bool getAutoFragmentMessages() const
				{
					return _webSocket.auto_fragment();
				}
				
				void clearCallbacks()
				{
					_onConnected = nullptr;
					_onRead = nullptr;
					_onReadBinary = nullptr;
					_onError = nullptr;
					_onClosed = nullptr;
				}
				
			protected:
				void close(boost::system::error_code ec)
				{
					if (_errorCode.value() ||
						_state == State::Closing)
						return;
					
					_errorCode = ec;
					
					switch(_state)
					{
						case State::ResolvingHost:
						{
							_state = State::ClosingDueToError;
							return;
						}
						default:;
					}
					
					// Close the WebSocket connection
					_webSocket.async_close(boost::beast::websocket::close_code::normal,
										   std::bind(&WebSocketConnectionImpl::internalOnClose,
													 shared_from_this(),
													 std::placeholders::_1));

					_state = State::ClosingDueToError;
				}
				
				void internalOnResolve(boost::system::error_code ec,
									   boost::asio::ip::tcp::resolver::results_type results)
				{
					if(ec)
					{
						close(ec);
						return;
					}
					
					// Make the connection on the IP address we get from a lookup
					boost::asio::async_connect(_webSocket.next_layer(),
											   results.begin(),
											   results.end(),
											   std::bind(
														 &WebSocketConnectionImpl::internalOnConnect,
														 shared_from_this(),
														 std::placeholders::_1));
					
					_state = State::Connecting;
				}
				
				void internalOnConnect(boost::system::error_code ec)
				{
					if(ec)
					{
						close(ec);
						return;
					}
					
					// Perform the websocket handshake
					_webSocket.async_handshake(_host, _resource,
											   std::bind(
														 &WebSocketConnectionImpl::internalOnHandshake,
														 shared_from_this(),
														 std::placeholders::_1));

					_state = State::Handshaking;
				}
				
				void internalOnHandshake(boost::system::error_code ec)
				{
					_upgradeRequest = {};
					
					if(ec)
					{
						close(ec);
						return;
					}
					
					// Read a message into our buffer
					_webSocket.async_read(_buffer,
										  std::bind(
													&WebSocketConnectionImpl::internalOnRead,
													shared_from_this(),
													std::placeholders::_1,
													std::placeholders::_2));

					_state = State::Active;
					
					notifyOnConnected();
				}
				
				void internalOnWrite(boost::system::error_code ec,
									 std::size_t bytes_transferred)
				{
					if(ec)
					{
						close(ec);
						return;
					}
					
					auto message = std::get<0>(_outMessages.front());
					auto messageSize = message.size();
					
					if (bytes_transferred == messageSize)
					{
						_outMessages.pop();
					}
					else
					{
						message.erase(message.begin(), message.begin()+bytes_transferred);
					}
					
					if (!_outMessages.empty())
					{
						_webSocket.binary(std::get<1>(_outMessages.front()));
						
						// Send next message or the rest of last message
						_webSocket.async_write(boost::asio::buffer(std::get<0>(_outMessages.front())),
											   std::bind(&WebSocketConnectionImpl::internalOnWrite,
														 shared_from_this(),
														 std::placeholders::_1,
														 std::placeholders::_2));
					}
				}
				
				void internalOnRead(boost::system::error_code ec,
									std::size_t bytes_transferred)
				{
					if(ec)
					{
						close(ec);
						return;
					}
					
					if (_webSocket.got_text())
					{
						notifyOnRead(boost::beast::buffers_to_string(_buffer.data()));
					}
					else
					{
						if (_onReadBinary)
						{
							unsigned char* data = RJNEW unsigned char[_buffer.size()];
							std::size_t offset = 0;
							for(auto buffer: _buffer.data())
							{
								std::copy((unsigned char*)buffer.data(), (unsigned char*)buffer.data()+buffer.size(), data+offset);
								offset += buffer.size();
							}
							
							notifyOnReadBinary(data, offset);
						}
					}

					_buffer.consume(bytes_transferred);
					
					// Read a message into our buffer
					_webSocket.async_read(_buffer,
										  std::bind(
													&WebSocketConnectionImpl::internalOnRead,
													shared_from_this(),
													std::placeholders::_1,
													std::placeholders::_2));
				}
				
				void internalOnClose(boost::system::error_code ec)
				{
					// Clear input buffer
					_buffer.consume(_buffer.size());
					
					// Delete queue of output messages by assigning to empty
					// queue of the same type
					_outMessages = decltype(_outMessages){};
					
					// Close notification will be done on destructor
				}
				
				void notifyOnConnected()
				{
					if (_onConnected)
						_onConnected();
				}
				
				void notifyOnRead(const std::string& data)
				{
					if (_onRead)
						_onRead(data);
				}
				
				void notifyOnReadBinary(const unsigned char* data, std::size_t size)
				{
					if (_onReadBinary)
						_onReadBinary(data, size);
				}
				
				void notifyOnClosed()
				{
					_contextManager.contextReleased(_context);
					
					if (_onClosed)
						_onClosed();
				}
				
				void notifyOnError(const boost::system::error_code& errorCode)
				{
					_contextManager.contextReleased(_context);
					
					if (_onError)
						_onError(errorCode.value(), errorCode.message());
				}
				
			protected:
				std::function<void(void)> _onConnected;
				std::function<void(void)> _onClosed;
				std::function<void(const std::string&)> _onRead;
				std::function<void(const unsigned char*, std::size_t)> _onReadBinary;
				std::function<void(int, const std::string&)> _onError;
				
				ContextManager& _contextManager;
				boost::asio::io_context& _context;
				boost::asio::ip::tcp::resolver _resolver;
				boost::beast::websocket::stream<boost::asio::ip::tcp::socket> _webSocket;
				boost::beast::http::request<boost::beast::http::string_body> _upgradeRequest;
				boost::beast::multi_buffer _buffer;
				boost::system::error_code _errorCode;
				std::queue<std::tuple<std::vector<unsigned char>, bool>> _outMessages;
				std::string _host;
				std::string _port;
				std::string _resource;
				State _state;
			};

			#ifdef USE_V8
				WebSocketConnection::WebSocketConnection(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: _contextManager(*jsEngine->contextManager)
				{
					_autoFragmentMessages = true;
				}
			#elif defined USE_JAVASCRIPTCORE
				WebSocketConnection::WebSocketConnection(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
				: _contextManager(*jsEngine->contextManager)
				{
					_autoFragmentMessages = true;
				}
			#endif

			WebSocketConnection::WebSocketConnection(ContextManager& contextManager)
			: _contextManager(contextManager)
			{
				_autoFragmentMessages = true;
			}
			
			WebSocketConnection::WebSocketConnection(ContextManager& contextManager,
													 boost::asio::ip::tcp::socket&& socket,
													 const http::request<http::string_body>& request)
			: _contextManager(contextManager)
			{
				_autoFragmentMessages = true;
				
				auto impl = std::make_shared<WebSocketConnectionImpl>(_contextManager, std::move(socket));
				_impl = impl;
				
				impl->onConnected(std::bind(&WebSocketConnection::onConnectedCallback, this));
				impl->onRead(std::bind(&WebSocketConnection::onReadCallback, this, std::placeholders::_1),
							 std::bind(&WebSocketConnection::onReadBinaryCallback, this, std::placeholders::_1,
									   std::placeholders::_2));
				impl->onError(std::bind(&WebSocketConnection::onErrorCallback, this, std::placeholders::_1, std::placeholders::_2));
				impl->onClosed(std::bind(&WebSocketConnection::onClosedCallback, this));
				
				impl->setAutoFragmentMessages(_autoFragmentMessages);

				impl->accept(request);
			}

			WebSocketConnection::~WebSocketConnection()
			{
				auto impl = _impl.lock();
				if (impl)
				{
					impl->clearCallbacks();
					impl->close();
				}
				
				// Events designed mostly for GUI and they will be freed by GUI library
				// but in our case where we are not relying on GUI here we need to remove events manually
				for(auto it = events->begin(); it != events->end(); it++)
				{
					DELETEOBJ(it->second);
				}
			}
			
			WebSocketConnection* WebSocketConnection::handleUpgrade(HttpConnection* connection,
																	const http::request<http::string_body>& request)
			{
				if (connection)
				{
					boost::asio::ip::tcp::socket* socket = connection->getSocket();
					if (socket)
					{
						auto webSocketConnection = RJNEW WebSocketConnection(connection->getContextManager(),
																			 std::move(*socket),
																			 request);
						connection->upgradeHandled();
						return webSocketConnection;
					}
				}
				
				return nullptr;
			}

			void WebSocketConnection::onOpen(std::function<void(void)> func)
			{
				_onOpen = func;
			}
			
			void WebSocketConnection::onMessage(std::function<void(const std::string&)> func,
											std::function<void(const unsigned char*, std::size_t)> funcBinary)
			{
				_onMessage = func;
				_onMessageBinary = funcBinary;
			}
			
			void WebSocketConnection::onClose(std::function<void(void)> func)
			{
				_onClose = func;
			}
			
			void WebSocketConnection::onError(std::function<void(int, const std::string&)> func)
			{
				_onError = func;
			}

			void WebSocketConnection::connect(const std::string& uri)
			{
				_uri = URI::parse(uri);
				connect(_uri.host, String::fromInt(_uri.port), _uri.resource);
			}
			
			void WebSocketConnection::connect(const std::string& host,
										  const std::string& port,
										  const std::string& resource)
			{
				if (_impl.lock())
					return;
				
				auto impl = std::make_shared<WebSocketConnectionImpl>(_contextManager);
				_impl = impl;
				
				impl->onConnected(std::bind(&WebSocketConnection::onConnectedCallback, this));
				impl->onRead(std::bind(&WebSocketConnection::onReadCallback, this, std::placeholders::_1),
							 std::bind(&WebSocketConnection::onReadBinaryCallback, this, std::placeholders::_1, std::placeholders::_2));
				impl->onError(std::bind(&WebSocketConnection::onErrorCallback, this, std::placeholders::_1, std::placeholders::_2));
				impl->onClosed(std::bind(&WebSocketConnection::onClosedCallback, this));
				impl->setAutoFragmentMessages(_autoFragmentMessages);

				_uri.host = host;
				_uri.port = parseInt(port);
				_uri.resource = resource;
				
				impl->connect(host, port, resource);
			}

			void WebSocketConnection::send(const std::string& data)
			{
				auto impl = _impl.lock();
				
				if (impl)
					impl->send(data);
			}
			
			void WebSocketConnection::send(const unsigned char* data, unsigned int size)
			{
				auto impl = _impl.lock();
				
				if (impl)
					impl->send(data, size);
			}
			
			void WebSocketConnection::close()
			{
				auto impl = _impl.lock();
				
				if (impl)
					impl->close();
			}
			
			void WebSocketConnection::setAutoFragmentMessages(bool autoFragment)
			{
				_autoFragmentMessages = autoFragment;
				
				auto impl = _impl.lock();
				
				if (impl)
					impl->setAutoFragmentMessages(_autoFragmentMessages);
			}
			
			bool WebSocketConnection::getAutoFragmentMessages() const
			{
				return _autoFragmentMessages;
			}

			void WebSocketConnection::onConnectedCallback()
			{
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					executeEvent("open");
				#endif

				if (_onOpen)
					_onOpen();
			}
			
			void WebSocketConnection::onReadCallback(const std::string& data)
			{
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					executeEvent("message", data);
				#endif

				if (_onMessage)
					_onMessage(data);
			}
			
			void WebSocketConnection::onReadBinaryCallback(const unsigned char* data, std::size_t size)
			{
				#ifdef USE_V8
					auto dataArray = v8::ArrayBuffer::New(V8_JAVASCRIPT_ENGINE->isolate, size);
					std::copy(data, data+size, (unsigned char*)dataArray->GetContents().Data());
				
					v8::Local<v8::Value> args[1];
					args[0] = dataArray;
				
					executeEvent("message", 1, args);
				#elif defined USE_JAVASCRIPTCORE
					#pragma message("Missing implementation of onReadBinaryCallback for JSC")
					//TODO: add JSC implementation
					//executeEvent("message", 1, args);
				#endif

				if (_onMessageBinary)
					_onMessageBinary(data, size);
			}

			void WebSocketConnection::onClosedCallback()
			{
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					executeEvent("close");
				#endif

				if (_onClose)
					_onClose();
			}

			void WebSocketConnection::onErrorCallback(int errorCode, const std::string& description)
			{
				#ifdef USE_V8
					v8::Local<v8::Value> args[2];
					args[0] = v8::Number::New(V8_JAVASCRIPT_ENGINE->isolate, errorCode);
					args[1] = String(description).toV8String(V8_JAVASCRIPT_ENGINE->isolate);
				
					executeEvent("error", 2, args);
				#elif defined USE_JAVASCRIPTCORE
					#pragma message("Missing implementation of onErrorCallback for JSC")
					//TODO: add JSC implementation
					//executeEvent("error", description);
				#endif

				if (_onError)
					_onError(errorCode, description);
			}
			
			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void WebSocketConnection::on(String event, RJ_FUNC_TYPE func)
				{
					createEvent(event, func);
				}
			#endif
		}
	}
}
