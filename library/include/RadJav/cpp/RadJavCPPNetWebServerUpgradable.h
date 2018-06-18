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
#ifndef _RADJAV_CPP_NET_WEBSERVERUPGRADABLE_H_
	#define _RADJAV_CPP_NET_WEBSERVERUPGRADABLE_H_

	#include "RadJavPreprocessor.h"

	#include <boost/beast/core.hpp>
	#include <boost/beast/http.hpp>
	#include <boost/beast/websocket.hpp>
	#include <boost/asio/bind_executor.hpp>
	#include <boost/asio/ip/tcp.hpp>
	#include <boost/asio/signal_set.hpp>
	#include <boost/asio/strand.hpp>
	#include <algorithm>
	#include <functional>
	#include <memory>
	#include <string>
	#include <vector>


namespace inspector {
	class Semaphore;
	class InspectorIoDelegate;
}

	using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
	namespace http = boost::beast::http;            // from <boost/beast/http.hpp>
	namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

	#include "RadJavString.h"

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				class RADJAV_EXPORT WebServerUpgradable
				{
				public:
					WebServerUpgradable(inspector::InspectorIoDelegate* delegate, String path_name, String script_name, RJBOOL wait_for_connect_);
					~WebServerUpgradable();

					void setIOCallback(inspector::InspectorIoDelegate* delegate);

					void listen(v8::Isolate* isolate, inspector::Semaphore* lock);

					void send(String id, String message);

					void close();

					RJINT port;

					boost::asio::io_context *io_context_;

					/// Flag that indicates if listening context available 
					RJBOOL isAlive;
					RJBOOL is_waiting_;
					String path_name_;
					String script_name_;
					inspector::InspectorIoDelegate* io_delegate_;
					RJBOOL wait_for_connect_;
					inspector::Semaphore* lock_;

					RJBOOL isWaiting() { return is_waiting_; }
						
					class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
					{

					public:
						websocket::stream<tcp::socket> ws_;
						boost::asio::strand<boost::asio::io_context::executor_type> strand_;
						boost::beast::multi_buffer buffer_;
						std::string sessionID_;

						// Take ownership of the socket
						explicit WebSocketSession(tcp::socket socket, std::string sessionID, WebServerUpgradable* parent);

						// Start the asynchronous operation
						template<class Body, class Allocator>
						void do_accept(http::request<Body, http::basic_fields<Allocator>> req)
						{
							// Accept the websocket handshake
							ws_.async_accept(
								req,
								boost::asio::bind_executor(
									strand_,
									std::bind(
										&WebSocketSession::on_accept,
										shared_from_this(),
										std::placeholders::_1)));
						}

						void on_accept(boost::system::error_code ec);

						void do_read();

						void do_write(String message);

						void on_read(boost::system::error_code ec, std::size_t bytes_transferred);

						void on_write(boost::system::error_code ec, std::size_t bytes_transferred);

					private:

						WebServerUpgradable* parent_;
					};

					void setWebSocketSession(const std::shared_ptr<WebSocketSession>& session);

						class HttpUpgradableSession : public std::enable_shared_from_this<HttpUpgradableSession>
						{
							// This is the C++11 equivalent of a generic lambda.
							// The function object is used to send an HTTP message.
							struct send_lambda
							{
								HttpUpgradableSession& self_;

								explicit
									send_lambda(HttpUpgradableSession& self)
									: self_(self)
								{
								}

								template<bool isRequest, class Body, class Fields>
								void
									operator()(http::message<isRequest, Body, Fields>&& msg) const
								{
									// The lifetime of the message has to extend
									// for the duration of the async operation so
									// we use a shared_ptr to manage it.
									auto sp = std::make_shared<
										http::message<isRequest, Body, Fields>>(std::move(msg));

									// Store a type-erased version of the shared
									// pointer in the class to keep it alive.
									self_.res_ = sp;

									// Write the response
									http::async_write(
										self_.socket_,
										*sp,
										boost::asio::bind_executor(
											self_.strand_,
											std::bind(
												&HttpUpgradableSession::on_write,
												self_.shared_from_this(),
												std::placeholders::_1,
												std::placeholders::_2,
												sp->need_eof())));
								}
							};

							tcp::socket socket_;
							boost::asio::strand<boost::asio::io_context::executor_type> strand_;
							boost::beast::flat_buffer buffer_;
							http::request<http::string_body> req_{};
							std::shared_ptr<void> res_;
							send_lambda lambda_;
							WebServerUpgradable* parent_;

							public:
								// Take ownership of the socket
								explicit HttpUpgradableSession(boost::asio::ip::tcp::socket socket, std::string sessionID, WebServerUpgradable* parent);

								// Start the asynchronous operation
								void run();

								void do_read();

								void on_read(boost::system::error_code ec);

								void on_write(boost::system::error_code ec, std::size_t bytes_transferred, bool close);
								
								void do_close();

							private:
								std::string sessionID_;
						};

						class UpgradableListener : public std::enable_shared_from_this<UpgradableListener>
						{
							boost::asio::ip::tcp::acceptor acceptor_;
							boost::asio::ip::tcp::socket socket_;
							WebServerUpgradable* parent_;

							public:
								UpgradableListener(
									boost::asio::io_context& ioc,
									boost::asio::ip::tcp::endpoint endpoint,
									WebServerUpgradable* parent);

								// Start accepting incoming connections
								void run();

								void do_accept();

								void on_accept(boost::system::error_code ec);
						};

						//maintains the list of sessions
						static std::vector<std::pair<std::string, std::shared_ptr<HttpUpgradableSession>>> sessions_;
						std::shared_ptr<WebServerUpgradable::WebSocketSession> websocket_session_;

				};
			}
		}
	}
#endif // _RADJAV_CPP_NET_WEBSERVERUPGRADABLE_H_

