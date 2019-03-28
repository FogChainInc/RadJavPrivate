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
#include "cpp/RadJavCPPNetWebSocketClient.h"

#include "RadJav.h"
#include "RadJavString.h"
#include "cpp/RadJavCPPNet.h"
#include "cpp/RadJavCPPNetNetworkManager.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <cstdlib>
#include <functional>
#include <memory>
#include <string>
#include <queue>


namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			class WebSocketClientImpl : public std::enable_shared_from_this<WebSocketClientImpl>
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
				WebSocketClientImpl(boost::asio::io_context& context)
				: _context(context)
				, _resolver(context)
				, _webSocket(context)
				, _state(State::Idle)
				{
				}
				
				boost::asio::io_context& getContext()
				{
					return _context;
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
											std::bind(&WebSocketClientImpl::internalOnResolve,
													  shared_from_this(),
													  std::placeholders::_1,
													  std::placeholders::_2));
					
					_state = State::ResolvingHost;
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
											   std::bind(&WebSocketClientImpl::internalOnWrite,
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
											   std::bind(&WebSocketClientImpl::internalOnWrite,
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
										   std::bind(&WebSocketClientImpl::internalOnClose,
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
				
			protected:
				void close(boost::system::error_code ec)
				{
					_errorCode = ec;
					
					switch(_state)
					{
						case State::ResolvingHost:
						{
							if (_onError)
								_onError(_errorCode.value(), _errorCode.message());
							_state = State::Idle;
							return;
						}
						default:;
					}
					
					// Close the WebSocket connection
					_webSocket.async_close(boost::beast::websocket::close_code::normal,
										   std::bind(&WebSocketClientImpl::internalOnClose,
													 shared_from_this(),
													 std::placeholders::_1));

					_state = State::ClosingDueToError;
				}
				
				void internalOnResolve(boost::system::error_code ec,
									   boost::asio::ip::tcp::resolver::results_type results)
				{
					if(ec)
					{
						if (_state == State::Closing)
						{
							_state = State::Idle;
							if (_onClosed)
								_onClosed();
						}
						else if (_state != State::ClosingDueToError)
						{
							close(ec);
							return;
						}
					}
					
					// Make the connection on the IP address we get from a lookup
					boost::asio::async_connect(_webSocket.next_layer(),
											   results.begin(),
											   results.end(),
											   std::bind(
														 &WebSocketClientImpl::internalOnConnect,
														 shared_from_this(),
														 std::placeholders::_1));
					
					_state = State::Connecting;
				}
				
				void internalOnConnect(boost::system::error_code ec)
				{
					if(ec)
					{
						if (_state != State::Closing &&
							_state != State::ClosingDueToError)
						{
							close(ec);
							return;
						}
					}
					
					// Perform the websocket handshake
					_webSocket.async_handshake(_host, _resource,
											   std::bind(
														 &WebSocketClientImpl::internalOnHandshake,
														 shared_from_this(),
														 std::placeholders::_1));

					_state = State::Handshaking;
				}
				
				void internalOnHandshake(boost::system::error_code ec)
				{
					if(ec)
					{
						if (_state != State::Closing &&
							_state != State::ClosingDueToError)
						{
							close(ec);
							return;
						}
					}
					
					// Read a message into our buffer
					_webSocket.async_read(_buffer,
										  std::bind(
													&WebSocketClientImpl::internalOnRead,
													shared_from_this(),
													std::placeholders::_1,
													std::placeholders::_2));

					_state = State::Active;
					
					if (_onConnected)
						_onConnected();
				}
				
				void internalOnWrite(boost::system::error_code ec,
									 std::size_t bytes_transferred)
				{
					if(ec)
					{
						namespace error = boost::asio::error;
						if(ec != error::make_error_code(error::operation_aborted))
						{
							if (_state != State::Closing &&
								_state != State::ClosingDueToError)
							{
								close(ec);
							}
						}
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
											   std::bind(&WebSocketClientImpl::internalOnWrite,
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
						namespace error = boost::asio::error;
						if(ec == error::make_error_code(error::connection_reset))
						{
							close(ec);
						}
						else if(ec != error::make_error_code(error::operation_aborted))
						{
							if (_state != State::Closing &&
								_state != State::ClosingDueToError)
							{
								close(ec);
							}
						}
						return;
					}
					
					if (_webSocket.got_text())
					{
						if (_onRead)
							_onRead(boost::beast::buffers_to_string(_buffer.data()));
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
							
							_onReadBinary(data, offset);
						}
					}

					_buffer.consume(bytes_transferred);
					
					// Read a message into our buffer
					_webSocket.async_read(_buffer,
										  std::bind(
													&WebSocketClientImpl::internalOnRead,
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
					
					// Not sure how to handle errors in that case
					if(ec)
					{
						//if (_onError)
						//	_onError(ec.value());
					}
					
					// If we get here then the connection is closed gracefully
					if (_state == State::Closing)
					{
						_state = State::Idle;
						if (_onClosed)
							_onClosed();
					}
					else if (_state == State::ClosingDueToError)
					{
						_state = State::Idle;
						if (_onError)
							_onError(_errorCode.value(), _errorCode.message());
					}
				}
				
			protected:
				std::function<void(void)> _onConnected;
				std::function<void(void)> _onClosed;
				std::function<void(const std::string&)> _onRead;
				std::function<void(const unsigned char*, std::size_t)> _onReadBinary;
				std::function<void(int, const std::string&)> _onError;
				
				boost::asio::io_context& _context;
				boost::asio::ip::tcp::resolver _resolver;
				boost::beast::websocket::stream<boost::asio::ip::tcp::socket> _webSocket;
				boost::beast::multi_buffer _buffer;
				boost::system::error_code _errorCode;
				std::queue<std::tuple<std::vector<unsigned char>, bool>> _outMessages;
				std::string _host;
				std::string _port;
				std::string _resource;
				State _state;
			};

			#ifdef USE_V8
				WebSocketClient::WebSocketClient(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: _impl(nullptr)
				, _networkManager(*jsEngine->networkManager)
				{
				}
			#elif defined USE_JAVASCRIPTCORE
				WebSocketClient::WebSocketClient(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
				: _impl(nullptr)
				, _networkManager(*jsEngine->networkManager)
				, _suppressCallbacks(false)
				{
				}
			#endif

			WebSocketClient::WebSocketClient(NetworkManager& networkManager)
			: _impl(nullptr)
			, _networkManager(networkManager)
			{
			}
			
			WebSocketClient::~WebSocketClient()
			{
				releaseImpl();
				
				// Events designed mostly for GUI and they will be freed by GUI library
				// but in our case where we are not relying on GUI here we need to remove events manually
				for(auto it = events->begin(); it != events->end(); it++)
				{
					DELETEOBJ(it->second);
				}
			}
			
			void WebSocketClient::onOpen(std::function<void(void)> func)
			{
				_onOpen = func;
			}
			
			void WebSocketClient::onMessage(std::function<void(const std::string&)> func,
											std::function<void(const unsigned char*, std::size_t)> funcBinary)
			{
				_onMessage = func;
				_onMessageBinary = funcBinary;
			}
			
			void WebSocketClient::onClose(std::function<void(void)> func)
			{
				_onClose = func;
			}
			
			void WebSocketClient::onError(std::function<void(int, const std::string&)> func)
			{
				_onError = func;
			}

			void WebSocketClient::createImpl()
			{
				if (_impl)
					return;
				
				_impl = std::make_shared<WebSocketClientImpl>(_networkManager.getContext());
				
				_impl->onConnected(std::bind(&WebSocketClient::onConnectedCallback, this));
				_impl->onRead(std::bind(&WebSocketClient::onReadCallback, this, std::placeholders::_1),
							  std::bind(&WebSocketClient::onReadBinaryCallback, this, std::placeholders::_1, std::placeholders::_2));
				_impl->onError(std::bind(&WebSocketClient::onErrorCallback, this, std::placeholders::_1, std::placeholders::_2));
				_impl->onClosed(std::bind(&WebSocketClient::onClosedCallback, this));
			}

			void WebSocketClient::releaseImpl()
			{
				if (_impl)
				{
					_impl->onConnected(nullptr);
					_impl->onRead(nullptr, nullptr);
					_impl->onError(nullptr);
					_impl->onClosed(nullptr);
					_impl = nullptr;
				}
			}

			void WebSocketClient::connect(const std::string& uri)
			{
				if (_impl)
					return;
				
				createImpl();

				if (_impl)
				{
					_uri = URI::parse(uri);
					_impl->connect(_uri.host, String::fromInt(_uri.port), _uri.resource);
				}
			}
			
			void WebSocketClient::connect(const std::string& host,
										  const std::string& port,
										  const std::string& resource)
			{
				if (_impl)
					return;
				
				createImpl();
				
				if (_impl)
				{
					_uri.host = host;
					_uri.port = parseInt(port);
					_uri.resource = resource;

					_impl->connect(host, port, resource);
				}
			}

			void WebSocketClient::send(const std::string& data)
			{
				if (_impl)
					_impl->send(data);
			}
			
			void WebSocketClient::send(const unsigned char* data, unsigned int size)
			{
				if (_impl)
					_impl->send(data, size);
			}
			
			void WebSocketClient::close()
			{
				if (_impl)
					_impl->close();
			}
			
			void WebSocketClient::onConnectedCallback()
			{
				if (_onOpen)
					_onOpen();
				
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					executeEvent("open");
				#endif
			}
			
			void WebSocketClient::onReadCallback(const std::string& data)
			{
				if (_onMessage)
					_onMessage(data);
				
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					executeEvent("message", data);
				#endif
			}
			
			void WebSocketClient::onReadBinaryCallback(const unsigned char* data, std::size_t size)
			{
				if (_onMessageBinary)
					_onMessageBinary(data, size);
				
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
			}

			void WebSocketClient::onClosedCallback()
			{
				_networkManager.contextReleased(_impl->getContext());
				
				releaseImpl();

				if (_onClose)
					_onClose();
				
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					executeEvent("close");
				#endif
			}

			void WebSocketClient::onErrorCallback(int errorCode, const std::string& description)
			{
				_networkManager.contextReleased(_impl->getContext());
				
				releaseImpl();

				if (_onError)
					_onError(errorCode, description);
				
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
			}
			
			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void WebSocketClient::on(String event, RJ_FUNC_TYPE func)
				{
					createEvent(event, func);
				}
			#endif
		}
	}
}

