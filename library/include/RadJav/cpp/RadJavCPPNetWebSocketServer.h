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
#ifndef _RADJAV_CPP_NET_WEBSOCKETSERVER_H_
	#define _RADJAV_CPP_NET_WEBSOCKETSERVER_H_

	#include "RadJavPreprocessor.h"

	#include <boost/asio.hpp>
	#include <boost/beast/core.hpp>
	#include <boost/beast/websocket.hpp>
	#include <boost/asio/bind_executor.hpp>
	#include <boost/asio/strand.hpp>
	#include <boost/asio/ip/tcp.hpp>
	#include <boost/asio/io_service.hpp>

	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "cpp/RadJavCPPChainedPtr.h"

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				class RADJAV_EXPORT WebSocketServer : public ChainedPtr
				{
					public:
						WebSocketServer();
						~WebSocketServer();

						static void on(String event_, v8::Local<v8::Function> func_);

						void listen(unsigned short port_ = 9229);

						void send(String id, String message);

						void sendToAll(String message);

						String receive();

						void close();

						class RADJAV_EXPORT WebSocketServerSession : public std::enable_shared_from_this<WebSocketServerSession>
						{
							boost::beast::websocket::stream<boost::asio::ip::tcp::socket> m_ws;
							boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
							boost::beast::multi_buffer m_readBuffer;

							public:
								// Take ownership of the socket
								WebSocketServerSession(boost::asio::ip::tcp::socket socket_, std::string sessionID_);

								// Start the asynchronous operation
								void run();

								void on_accept(boost::system::error_code ec_);

								void do_read();

								void do_write(String message_);

								void on_read(
									boost::system::error_code ec_,
									std::size_t bytes_transferred_);

								void on_write(
									boost::system::error_code ec_,
									std::size_t bytes_transferred_);

							private:
								std::string m_sessionID;
								std::shared_ptr<std::string> m_activeMessage = nullptr;
						};

						class RADJAV_EXPORT WebSocketServerListener : public std::enable_shared_from_this<WebSocketServerListener>
						{
							boost::asio::ip::tcp::acceptor m_acceptor;
							boost::asio::ip::tcp::socket m_socket;

							public:
								WebSocketServerListener(
									boost::asio::io_context& ioc_,
									boost::asio::ip::tcp::endpoint endpoint_);

								// Start accepting incoming connections
								void run();

								void do_accept();

								void on_accept(boost::system::error_code ec_);
						};				

						#ifdef USE_V8
						static v8::Persistent<v8::Function> *m_serverAcceptEvent;
						static v8::Persistent<v8::Function> *m_serverReceiveEvent;
						#endif
				
					private:
						unsigned short m_port;

						boost::asio::io_context *m_io_context;

						// Flag that indicates if listening context available 
						RJBOOL m_isAlive;

						struct session_data
						{
							std::string m_session_id;
							std::shared_ptr <WebSocketServerSession> m_session;
							std::string m_last_message;
						};

						//maintains the list of sessions
						static std::vector <session_data> m_sessions;
				};
			}
		}
	}
#endif

