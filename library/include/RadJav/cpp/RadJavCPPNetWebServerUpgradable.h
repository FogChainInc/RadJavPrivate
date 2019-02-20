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

#ifdef USE_INSPECTOR
	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "cpp/RadJavCPPChainedPtr.h"
	#include "cpp/RadJavCPPEvent.h"
	#include "RadJavThread.h"

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
	#include <thread>

	using tcp = boost::asio::ip::tcp;               // from <boost/asio/ip/tcp.hpp>
	namespace http = boost::beast::http;            // from <boost/beast/http.hpp>
	namespace websocket = boost::beast::websocket;  // from <boost/beast/websocket.hpp>

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				/**
				 * @class WebServerUpgradable
				 *
				 * @ingroup group_debug
				 * @ingroup group_net_cpp
				 *
				 * @brief V8 Inspector WebServer class that allows upgrade from http to websocket
				 *
				 * This class is meant to be used only by the v8 Inspector Agent, its implementation
				 * is not complete and only serves as connection point for Chrome Dev Tools with
				 * minimal functionality to allow JS debugging.
				 */
				class RADJAV_EXPORT WebServerUpgradable: public Events, public ChainedPtr
				{
					public:
						/**
						 * @brief Create upgradable webserver
						 *
						 * Upgradable webserver class picks up HTTP connection from Chrome Dev Tools, upgrades it to
						 * websocket connection and keeps communication session alive until disconnect
						 *
						 * @param delegate Inspector IO delegate responsible for the IO operations
						 *
						 * @param path_name path to script
						 *
						 * @param script_name script name
						 *
						 * @param wait_for_connect boolean flag that defines whether the server should wait for
						 * incoming connection or return immediately
						 *
						 */
						WebServerUpgradable(String listenAddress = "0.0.0.0", RJUSHORT port = 9229, RJBOOL useOwnThread = false);
						~WebServerUpgradable();

						/// If this sever needs to use it's own internal thread instead of RadJav's use it.
						RJBOOL useOwnThread;
						/// The internal thread to use.
						std::thread *myThread;

						/// This server's listen thread, separate from RadJav.
						void myListenThread();

						RJBOOL isAlive;

						/**
						 * @brief Start listening to connections
						 *
						 * @param isolate V8 isolate
						 *
						 * @param lock Semaphore to lock messages from different sessions
						 */
						void listen();

						/**
						 * @brief send message to specific session ID
						 *
						 * @param id String ID
						 *
						 * @param message Message to send
						 */
						void send(String id, String message);

						/**
						 * @brief Close webserver and release sessions
						 */
						void close();

						/// Get the number of HTTP connections.
						inline RJINT numHTTPConnections()
						{
							return (httpSessions.size ());
						}

						/// Get the number of websocket connections.
						inline RJINT numWebSocketConnections()
						{
							return (websocketSessions.size ());
						}

						/// The address to listen on.
						String listenAddress;
						/**
						 * @brief port number
						 *
						 * port is public to simplify JS/V8 invocation
						 */
						RJUSHORT port;

						/**
						 * @brief io_context pointer
						 */
						boost::asio::io_context *io_context_;

						/**
						 * brief simple thread to run the IO service
						 */
						SimpleThread *thread;

						/**
						 * @class WebSocketSession
						 *
						 * @ingroup group_debug
						 * @ingroup group_net_cpp
						 *
						 * @brief V8 Inspector websocket session class that takes ownership of a socket and
						 * implements websocket handshake and communication
						 *
						 * This class is meant to be used only by the v8 Inspector Agent, its implementation
						 * is not complete and only serves as connection point for Chrome Dev Tools with
						 * minimal functionality to allow JS debugging.
						 */
						class WebSocketSession : public std::enable_shared_from_this<WebSocketSession>
						{

						public:
							/**
							 * @brief websocket stream
							 */
							websocket::stream<tcp::socket> ws_;

							/**
							 * @brief websocket strand
							 */
							boost::asio::strand<boost::asio::io_context::executor_type> strand_;

							/**
							 * @brief IO buffer
							 */
							boost::beast::multi_buffer buffer_;

							/**
							 * @brief String with session ID
							 */
							String sessionID_;

							/**
							 * @brief Constructor for websocket session that takes ownership of underlying socket
							 *
							 * @param socket tcp socket
							 *
							 * @param sessionID session ID
							 *
							 * @param parent upgradable web server object that will contain current session
							 *
							 */
							explicit WebSocketSession(tcp::socket socket, String sessionID, WebServerUpgradable* parent);

							/**
							 * @brief asynchronously accept websocket handshake
							 */
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

							/**
							 * @brief throw error if any, then call do_read()
							 *
							 * @param ec boost system error code
							 */
							void on_accept(boost::system::error_code ec);

							/**
							 * @brief read a message into inner buffer
							 */
							void do_read();

							/**
							 * @brief write a message to the socket
							 *
							 * @param message message to send
							 */
							void do_write(String message);

							/**
							 * @brief process socket close if occurs, throw error if any, then send message to inspector and perform next read
							 *
							 * bytes_transferred parameter is ignored
							 *
							 * @param ec boost system error code
							 *
							 * @param bytes_transferred ignored
							 *
							 */
							void on_read(boost::system::error_code ec, std::size_t bytes_transferred);

							/**
							 * @brief process socket close if occurs, throw error if any, clear buffer and do another read
							 *
							 * @param ec boost system error code
							 *
							 * @param bytes_transferred ignored
							 *
							 */
							void on_write(boost::system::error_code ec, std::size_t bytes_transferred);

						private:

							/**
							 * @brief pointer to parent upgradable web server object in context of which the current session runs
							 */
							WebServerUpgradable* parent_;
						};

						/**
						 * @class HttpUpgradableSession
						 *
						 * @ingroup group_debug
						 * @ingroup group_net_cpp
						 *
						 * @brief V8 Inspector http session class that is supposed to be upgraded to websocket
						 * session after handshake
						 *
						 * This class is meant to be used only by the v8 Inspector Agent, its implementation
						 * is not complete and only serves as connection point for Chrome Dev Tools with
						 * minimal functionality to allow JS debugging.
						 */
						class HttpUpgradableSession : public std::enable_shared_from_this<HttpUpgradableSession>
						{
							/**
							 * @struct send_lambda
							 *
							 * @brief This is the C++11 equivalent of a generic lambda.
							 * The function object is used to send an HTTP message.
							 *
							 */

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

							/**
							 * @brief plain tcp socket
							 */
							tcp::socket socket_;

							/**
							 * @brief boost strand
							 */
							boost::asio::strand<boost::asio::io_context::executor_type> strand_;

							/**
							 * @brief internal buffer for incoming messages
							 */
							boost::beast::flat_buffer buffer_;

							/**
							 * @brief current http request
							 */
							http::request<http::string_body> req_{};

							/**
							 * @brief current response
							 */
							std::shared_ptr<void> res_;

							/**
							 * @brief message sending lambda
							 */
							send_lambda lambda_;

							/**
							 * @brief pointer to parent upgradable web server object in context of which the current session runs
							 */
							WebServerUpgradable* parent_;

							public:
								/**
								 * @brief Create upgradable http session by taking ownership of incoming socket
								 *
								 * @param socket tcp socket
								 *
								 * @param sessionID string with session ID
								 *
								 * @param parent upgradable web server object that will contain current session
								 */
								explicit HttpUpgradableSession(boost::asio::ip::tcp::socket socket, String sessionID, WebServerUpgradable* parent);

								/**
								 * @brief Start the asynchronous operation
								 */
								void run();

								/**
								 * @brief
								 */
								void do_read();

								/**
								 * @brief throw error if any, then call do_read()
								 *
								 * @param ec boost system error code
								 */
								void on_read(boost::system::error_code ec);

								/**
								 * @brief throw error if any, close connection if close flag is true,
								 * otherwise clear the buffer and perform another read
								 *
								 * bytes_transferred parameter is ignored
								 *
								 * @param ec boost system error code
								 *
								 * @param bytes_transferred ignored
								 *
								 * @param close boolean flag that usually is set because
								 * the response indicated the "Connection: close" semantic.
								 */
								void on_write(boost::system::error_code ec, std::size_t bytes_transferred, bool close);

								/**
								 * @brief close current session
								 */
								void do_close();

							private:
								/**
								 * @brief session ID
								 */
								String sessionID_;
						};

						/**
						 * @class UpgradableListener
						 *
						 * @ingroup group_debug
						 * @ingroup group_net_cpp
						 *
						 * @brief V8 Inspector upgradable tcp socket listener class
						 *
						 * This class is meant to be used only by the v8 Inspector Agent, its implementation
						 * is not complete and only serves as connection point for Chrome Dev Tools with
						 * minimal functionality to allow JS debugging.
						 */
						class UpgradableListener : public std::enable_shared_from_this<UpgradableListener>
						{
							/**
							 * @brief tcp acceptor
							 */
							boost::asio::ip::tcp::acceptor acceptor_;

							/**
							 * @brief tcp socket
							 */
							boost::asio::ip::tcp::socket socket_;

							/**
							 * @brief pointer to parent upgradable web server object
							 */
							WebServerUpgradable* parent_;

							public:
								/**
								 * @brief Create upgradable listener
								 *
								 * @param ioc boost io_context
								 *
								 * @param endpoint ip and port values to listen
								 *
								 * @param parent pointer to parent upgradable web server object
								 */
								UpgradableListener(
									boost::asio::io_context& ioc,
									boost::asio::ip::tcp::endpoint endpoint,
									WebServerUpgradable* parent);

								/**
								 * @brief Start accepting incoming connections
								 */
								void run();

								/**
								 * @brief call on_accept()
								 */
								void do_accept();

								/**
								 * @brief throw error if any, create the http session and run it, then call do_accept
								 *
								 * @param ec boost system error code
								 */
								void on_accept(boost::system::error_code ec);
						};

						/**
						 * @brief list of active http sessions
						 */
						Array< std::pair<std::string, std::shared_ptr<HttpUpgradableSession> > > httpSessions;

						/**
						 * @brief The current websocket sessions
						 */
						HashMap< std::string, std::shared_ptr<WebServerUpgradable::WebSocketSession> > websocketSessions;
				};
			}
		}
	}
#endif
#endif // _RADJAV_CPP_NET_WEBSERVERUPGRADABLE_H_

