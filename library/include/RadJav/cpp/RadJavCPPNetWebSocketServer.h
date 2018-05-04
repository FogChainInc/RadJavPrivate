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

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				class RADJAV_EXPORT WebSocketServer
				{
					public:
						WebSocketServer();
						~WebSocketServer();

						void listen();

						void send(String id, String message);

						void sendToAll(String message);

						void close();

						RJINT port;
						// Put connected clients here.

						boost::asio::io_context *io_context_;

						/// Flag that indicates if listening context available 
						RJBOOL isAlive;


						class RADJAV_EXPORT WebSocketServerSession : public std::enable_shared_from_this<WebSocketServerSession>
						{
							boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_;
							boost::asio::strand<boost::asio::io_context::executor_type> strand_;
							boost::beast::multi_buffer buffer_;

							public:
								// Take ownership of the socket
								WebSocketServerSession(boost::asio::ip::tcp::socket socket, std::string sessionID);

								// Start the asynchronous operation
								void run();

								void on_accept(boost::system::error_code ec);

								void do_read();

								void do_write(String message);

								void on_read(
									boost::system::error_code ec,
									std::size_t bytes_transferred);

								void on_write(
									boost::system::error_code ec,
									std::size_t bytes_transferred);

							private:
								std::string sessionID_;
						};

						class RADJAV_EXPORT WebSocketServerListener : public std::enable_shared_from_this<WebSocketServerListener>
						{
							boost::asio::ip::tcp::acceptor acceptor_;
							boost::asio::ip::tcp::socket socket_;

							public:
								WebSocketServerListener(
									boost::asio::io_context& ioc,
									boost::asio::ip::tcp::endpoint endpoint);

								// Start accepting incoming connections
								void run();

								void do_accept();

								void on_accept(boost::system::error_code ec);
						};

						//maintains the list of sessions
						static std::vector<std::pair<std::string, std::shared_ptr<WebSocketServerSession>>> sessions_;
				};
			}
		}
	}
#endif

