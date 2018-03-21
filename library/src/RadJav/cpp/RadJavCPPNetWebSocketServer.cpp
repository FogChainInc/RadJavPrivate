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
#include "cpp/RadJavCPPNetWebSocketServer.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			WebSocketServer::WebSocketServer()
			{
				port = 0;
			}

			WebSocketServer::~WebSocketServer()
			{
				DELETEOBJ(io_context_);
			}

			void WebSocketServer::listen()
			{
				auto const address = boost::asio::ip::make_address("127.0.0.1");
				auto const port = static_cast<unsigned short>(std::atoi("80")); //9229
																				// The io_context is required for all I/O
				auto const threads = std::max<int>(1, std::atoi("1"));
				io_context_ = new boost::asio::io_context{ threads };
				std::make_shared<WebSocketServerListener>(*io_context_, boost::asio::ip::tcp::endpoint{ address, port })->run();
			}

			void WebSocketServer::send(String message)
			{
			}

			void WebSocketServer::sendToAll(String message)
			{
			}

			String WebSocketServer::receivedData()
			{
				return ("");
			}

			void WebSocketServer::close()
			{
				io_context_->stop();
			}

			WebSocketServer::WebSocketServerSession::WebSocketServerSession(boost::asio::ip::tcp::socket socket)
				: ws_(std::move(socket)), strand_(ws_.get_executor())
			{
			}

			void WebSocketServer::WebSocketServerSession::run ()
			{
				// Accept the websocket handshake
				ws_.async_accept(
					boost::asio::bind_executor(
						strand_,
						std::bind(
							&WebSocketServerSession::on_accept,
							shared_from_this(),
							std::placeholders::_1)));
			}

			void WebSocketServer::WebSocketServerSession::on_accept(boost::system::error_code ec)
			{
				if (ec)
				{
					RadJav::throwJSException("on_accept error");

					return;
				}

				// Read a message
				do_read();
			}

			void WebSocketServer::WebSocketServerSession::do_read()
			{
				// Read a message into our buffer
				ws_.async_read(
					buffer_,
					boost::asio::bind_executor(
						strand_,
						std::bind(
							&WebSocketServerSession::on_read,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}

			void WebSocketServer::WebSocketServerSession::on_read(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				// This indicates that the session was closed
				if (ec == boost::beast::websocket::error::closed)
					return;

				if (ec)
				{
					RadJav::throwJSException("Read error");

					return;
				}

				// Echo the message
				ws_.text(ws_.got_text());
				ws_.async_write(
					buffer_.data(),
					boost::asio::bind_executor(
						strand_,
						std::bind(
							&WebSocketServerSession::on_write,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}

			void WebSocketServer::WebSocketServerSession::on_write(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				if (ec)
				{
					RadJav::throwJSException("Write error");

					return;
				}

				// Clear the buffer
				buffer_.consume(buffer_.size());

				// Do another read
				do_read();
			}

			WebSocketServer::WebSocketServerListener::WebSocketServerListener(
				boost::asio::io_context& ioc,
				boost::asio::ip::tcp::endpoint endpoint)
					: acceptor_(ioc), socket_(ioc)
			{
				boost::system::error_code ec;

				// Open the acceptor
				acceptor_.open(endpoint.protocol(), ec);
				if (ec)
				{
					RadJav::throwJSException("Open error");

					return;
				}

				// Allow address reuse
				acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
				if (ec)
				{
					RadJav::throwJSException("set_option error");

					return;
				}

				// Bind to the server address
				acceptor_.bind(endpoint, ec);
				if (ec)
				{
					RadJav::throwJSException("Bind error");

					return;
				}

				// Start listening for connections
				acceptor_.listen(
					boost::asio::socket_base::max_listen_connections, ec);
				if (ec)
				{
					RadJav::throwJSException("Listen error");

					return;
				}
			}

			// Start accepting incoming connections
			void WebSocketServer::WebSocketServerListener::run()
			{
				if (!acceptor_.is_open())
					return;

				do_accept();
			}

			void WebSocketServer::WebSocketServerListener::do_accept()
			{
				acceptor_.async_accept(
					socket_,
					std::bind(
						&WebSocketServerListener::on_accept,
						shared_from_this(),
						std::placeholders::_1));
			}

			void WebSocketServer::WebSocketServerListener::on_accept(boost::system::error_code ec)
			{
				if (ec)
				{
					RadJav::throwJSException("Accept error");
				}
				else
				{
					// Create the session and run it
					std::make_shared<WebSocketServerSession>(std::move(socket_))->run();
				}

				// Accept another connection
				do_accept();
			}
		}
	}
}

