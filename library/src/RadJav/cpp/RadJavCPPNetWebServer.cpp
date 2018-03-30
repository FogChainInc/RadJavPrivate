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


#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

#include "RadJavString.h"
#include "RadJav\v8\RadJavV8JavascriptEngine.h"

using namespace RadJAV::CPP::Net;

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{

#ifdef GUI_USE_WXWIDGETS

			WebServerThread::WebServerThread(boost::asio::io_context* ioc)
			: ioc(ioc)
			{
			}

			WebServerThread::ExitCode WebServerThread::Entry() {
				ioc->run();

				V8_JAVASCRIPT_ENGINE->removeThread(this);
				return (0);
			};

#endif

			// Report a failure
			void fail(boost::system::error_code ec, char const* what)
			{
				String tmp("error [");
				tmp.append(what);
				tmp.append("]:\n");
				RadJAV::RadJav::throwJSException(tmp.append(ec.message()));
			}

			//String(*persistentCallback)(void* persistentArgs);

			// This function produces an HTTP response for the given
			// request. The type of the response object depends on the
			// contents of the request, so the interface requires the
			// caller to pass a generic lambda for receiving the response.
			template< class Body, class Allocator, class Send>
			void handle_request(
				http::request<Body, http::basic_fields<Allocator>>&& req,
				Send&& send,
				v8::Persistent<v8::Function> * servePersistent
			)
			{
				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(servePersistent->Get(V8_JAVASCRIPT_ENGINE->isolate));
				String sendToClient = "";

				if (V8_JAVASCRIPT_ENGINE->v8IsNull(func) == false)
				{
					AsyncFunctionCall *call = RJNEW AsyncFunctionCall(servePersistent);
					call->deleteOnComplete = false;
					V8_JAVASCRIPT_ENGINE->callFunctionOnNextTick(call);

					while (call->checkForResult() == false)
					{
					}

					v8::Local<v8::Value> result = call->getResult(V8_JAVASCRIPT_ENGINE);

					sendToClient = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, result);
				}

				http::response<http::string_body> res{ http::status::ok, req.version() };
				res.set(http::field::content_type, "text/html");
				res.body() = sendToClient;
				res.content_length(sendToClient.length());
				res.keep_alive(req.keep_alive());

				return send(std::move(res));
			}

			//define inner worker classes
			class session : public std::enable_shared_from_this<session>
			{
				// This is the C++11 equivalent of a generic lambda.
				// The function object is used to send an HTTP message.
				struct send_lambda
				{
					session& self_;

					explicit
						send_lambda(session& self)
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
						auto sp = std::make_shared<http::message<isRequest, Body, Fields>>(std::move(msg));

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
									&session::on_write,
									self_.shared_from_this(),
									std::placeholders::_1,
									std::placeholders::_2,
									sp->need_eof())));
					}
				};

				tcp::socket socket_;
				boost::asio::strand<boost::asio::io_context::executor_type> strand_;
				boost::beast::flat_buffer buffer_;
				http::request<http::string_body> req_;
				std::shared_ptr<void> res_;
				send_lambda lambda_;
				v8::Persistent<v8::Function> * servePersistent;

			public:
				// Take ownership of the socket
				explicit session(tcp::socket socket, v8::Persistent<v8::Function> * servePersistent)
					: socket_(std::move(socket))
					, strand_(socket_.get_executor())
					, lambda_(*this)
					, servePersistent(servePersistent)
				{
				}

				// Start the asynchronous operation
				void run()
				{
					do_read();
				}

				void do_read()
				{
					// Make the request empty before reading,
					// otherwise the operation behavior is undefined.
					req_ = {};

					// Read a request
					http::async_read(socket_, buffer_, req_,
						boost::asio::bind_executor(
							strand_,
							std::bind(
								&session::on_read,
								shared_from_this(),
								std::placeholders::_1,
								std::placeholders::_2)));
				}

				void
					on_read(
						boost::system::error_code ec,
						std::size_t bytes_transferred)
				{
					boost::ignore_unused(bytes_transferred);

					// This means they closed the connection
					if (ec == http::error::end_of_stream)
						return do_close();

					if (ec)
						return fail(ec, "read");

					// Send the response
					handle_request(std::move(req_), lambda_, servePersistent);
				}

				void on_write(boost::system::error_code ec, std::size_t bytes_transferred, bool close)
				{
					boost::ignore_unused(bytes_transferred);

					if (ec)
						return fail(ec, "write");

					if (close)
					{
						// This means we should close the connection, usually because
						// the response indicated the "Connection: close" semantic.
						return do_close();
					}

					// We're done with the response so delete it
					res_ = nullptr;

					// Read another request
					do_read();
				}

				void do_close()
				{
					// Send a TCP shutdown
					boost::system::error_code ec;
					socket_.shutdown(tcp::socket::shutdown_send, ec);

					// At this point the connection is closed gracefully
				}
			};

			// Accepts incoming connections and launches the sessions
			WebServer::WebServer()
				:ioc(1), //single child thread
				acceptor(ioc),
				socket(ioc),
				isAlive(false)
			{
				serverType = WebServerTypes::HTTP;
				address = boost::asio::ip::make_address("127.0.0.1");
				port = 80;
			}

			WebServer::~WebServer()
			{
			}

			void WebServer::listen(RJINT portNumber)
			{
				//override "any port" behavior for 0 value; default http server port number is set to 80
				//listen() call without parameters must be preceded with WebServer->port(<value>) call
				if (portNumber != 0) {
					port = portNumber;
				}

				boost::system::error_code ec;

				tcp::endpoint endpoint{ address, static_cast<unsigned short>(port) };

				// Open the acceptor
				acceptor.open(endpoint.protocol(), ec);
				if (ec)
				{
					fail(ec, "open");
					return;
				}

				// Allow address reuse
				acceptor.set_option(boost::asio::socket_base::reuse_address(true));
				if (ec)
				{
					fail(ec, "set_option");
					return;
				}

				// Bind to the server address
				acceptor.bind(endpoint, ec);
				if (ec)
				{
					fail(ec, "bind");
					return;
				}

				// Start listening for connections
				acceptor.listen(
					boost::asio::socket_base::max_listen_connections, ec);
				if (ec)
				{
					fail(ec, "listen");
					return;
				}
				
				this->run();

#ifdef GUI_USE_WXWIDGETS	
				WebServerThread* thread = new WebServerThread(&ioc);
				thread->Run();
				isAlive = thread->IsAlive();
#else
				//blocking execution
				isAlive = true;
				ioc.run();
#endif
			}

#ifdef USE_V8
			void WebServer::serve(v8::Local<v8::Function> function)
			{
				if (!function.IsEmpty())
				{
					servePersistent = RJNEW v8::Persistent<v8::Function>();
					servePersistent->Reset(V8_JAVASCRIPT_ENGINE->isolate, function);
				}
			}
#endif

			void WebServer::stop()
			{
				ioc.stop();
				while (false == ioc.stopped()) {
					Sleep(1 * 50); //50 ms
				}
				this->close();
			}

			// Start accepting incoming connections
			void WebServer::run()
			{
				if (!acceptor.is_open())
					return;
				do_accept();
			}

			void WebServer::do_accept()
			{
				acceptor.async_accept(
					socket,
					std::bind(
						&WebServer::on_accept,
						this,
						std::placeholders::_1));
			}

			void WebServer::on_accept(boost::system::error_code ec)
			{
				if (ec)
				{
					fail(ec, "accept");
				}
				else
				{
					// Create the session and run it
					std::make_shared<session>(std::move(socket), servePersistent)->run();
				}

				// Accept another connection
				do_accept();
			}

			void WebServer::close()
			{
				//TODO: check graceful exit behavior on ioc.stop()
				acceptor.close();
				isAlive = false;
			}
		}
	}
}

