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
#include "v8/RadJavV8JavascriptEngine.h"

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
				RadJAV::RadJav::throwException(tmp.append(ec.message()));
			}

			// Append an HTTP rel-path to a local filesystem path.
			// The returned path is normalized for the platform.
			std::string
				path_cat(
					boost::beast::string_view base,
					boost::beast::string_view path)
			{
				if (base.empty())
					return path.to_string();
				std::string result = base.to_string();
#if BOOST_MSVC
				char constexpr path_separator = '\\';
				if (result.back() == path_separator)
					result.resize(result.size() - 1);
				result.append(path.data(), path.size());
				for (auto& c : result)
					if (c == '/')
						c = path_separator;
#else
				char constexpr path_separator = '/';
				if (result.back() == path_separator)
					result.resize(result.size() - 1);
				result.append(path.data(), path.size());
#endif
				return result;
			}

			// Return a reasonable mime type based on the extension of a file.
			boost::beast::string_view
				mime_type(boost::beast::string_view path)
			{
				using boost::beast::iequals;
				auto const ext = [&path]
				{
					auto const pos = path.rfind(".");
					if (pos == boost::beast::string_view::npos)
						return boost::beast::string_view{};
					return path.substr(pos);
				}();
				if (iequals(ext, ".htm"))  return "text/html";
				if (iequals(ext, ".html")) return "text/html";
				if (iequals(ext, ".php"))  return "text/html";
				if (iequals(ext, ".css"))  return "text/css";
				if (iequals(ext, ".txt"))  return "text/plain";
				if (iequals(ext, ".js"))   return "application/javascript";
				if (iequals(ext, ".json")) return "application/json";
				if (iequals(ext, ".xml"))  return "application/xml";
				if (iequals(ext, ".swf"))  return "application/x-shockwave-flash";
				if (iequals(ext, ".flv"))  return "video/x-flv";
				if (iequals(ext, ".png"))  return "image/png";
				if (iequals(ext, ".jpe"))  return "image/jpeg";
				if (iequals(ext, ".jpeg")) return "image/jpeg";
				if (iequals(ext, ".jpg"))  return "image/jpeg";
				if (iequals(ext, ".gif"))  return "image/gif";
				if (iequals(ext, ".bmp"))  return "image/bmp";
				if (iequals(ext, ".ico"))  return "image/vnd.microsoft.icon";
				if (iequals(ext, ".tiff")) return "image/tiff";
				if (iequals(ext, ".tif"))  return "image/tiff";
				if (iequals(ext, ".svg"))  return "image/svg+xml";
				if (iequals(ext, ".svgz")) return "image/svg+xml";
				return "application/text";
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
				// Returns a bad request response
				auto const bad_request =
					[&req](boost::beast::string_view why)
				{
					http::response<http::string_body> res{ http::status::bad_request, req.version() };
					res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
					res.set(http::field::content_type, "text/html");
					res.keep_alive(req.keep_alive());
					res.body() = why.to_string();
					res.prepare_payload();
					return res;
				};

				// Returns a not found response
				auto const not_found =
					[&req](boost::beast::string_view target)
				{
					http::response<http::string_body> res{ http::status::not_found, req.version() };
					res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
					res.set(http::field::content_type, "text/html");
					res.keep_alive(req.keep_alive());
					res.body() = "The resource '" + target.to_string() + "' was not found.";
					res.prepare_payload();
					return res;
				};

				// Returns a server error response
				auto const server_error =
					[&req](boost::beast::string_view what)
				{
					http::response<http::string_body> res{ http::status::internal_server_error, req.version() };
					res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
					res.set(http::field::content_type, "text/html");
					res.keep_alive(req.keep_alive());
					res.body() = "An error occurred: '" + what.to_string() + "'";
					res.prepare_payload();
					return res;
				};

				// Make sure we can handle the method
				if (req.method() != http::verb::get &&
					req.method() != http::verb::head)
					return send(bad_request("Unknown HTTP-method"));

				// Request path must be absolute and not contain "..".
				if (req.target().empty() ||
					req.target()[0] != '/' ||
					req.target().find("..") != boost::beast::string_view::npos)
					return send(bad_request("Illegal request-target"));

				v8::Local<v8::Function> func = v8::Local<v8::Function>::Cast(servePersistent->Get(V8_JAVASCRIPT_ENGINE->isolate));
				http::response<http::string_body> res{ http::status::ok, req.version() };

				if (V8_JAVASCRIPT_ENGINE->v8IsNull(func) == false)
				{
					AsyncFunctionCall *call = RJNEW AsyncFunctionCall(servePersistent);
					call->deleteOnComplete = false;
					V8_JAVASCRIPT_ENGINE->callFunctionOnNextTick(call);

					while (call->checkForResult() == false)
					{
					}

					v8::Local<v8::Value> result = call->getResult(V8_JAVASCRIPT_ENGINE);

					if (result->IsString() == true)
					{
						String sendToClient = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, result);

						res.set(http::field::content_type, "text/html");
						res.body() = sendToClient;
						res.content_length(sendToClient.length());
						res.keep_alive(req.keep_alive());
					}
					
					if (result->IsObject() == true)
					{
						v8::Local<v8::Object> obj = v8::Local<v8::Object>::Cast (result);

						v8::Local<v8::String> result = v8::Local<v8::String>::Cast (obj->Get(
								String ("serveDir").toV8String (V8_JAVASCRIPT_ENGINE->isolate)));

						String serveDir = parseV8ValueIsolate(V8_JAVASCRIPT_ENGINE->isolate, result);

						// Build the path to the requested file
						std::string path = path_cat(serveDir, req.target());

						if (req.target().back() == '/')
							path.append("index.html");

						// Attempt to open the file
						boost::beast::error_code ec;
						http::file_body::value_type body;
						body.open(path.c_str(), boost::beast::file_mode::scan, ec);

						// Handle the case where the file doesn't exist
						if (ec == boost::system::errc::no_such_file_or_directory)
							return send(not_found(req.target()));

						// Handle an unknown error
						if (ec)
							return send(server_error(ec.message()));

						// Cache the size since we need it after the move
						auto const size = body.size();

						// Respond to HEAD request
						if (req.method() == http::verb::head)
						{
							http::response<http::empty_body> res{ http::status::ok, req.version() };
							res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
							res.set(http::field::content_type, mime_type(path));
							res.content_length(size);
							res.keep_alive(req.keep_alive());
							return send(std::move(res));
						}

						// Respond to GET request
						http::response<http::file_body> res{
							std::piecewise_construct,
							std::make_tuple(std::move(body)),
							std::make_tuple(http::status::ok, req.version()) };
						res.set(http::field::server, BOOST_BEAST_VERSION_STRING);
						res.set(http::field::content_type, mime_type(path));
						res.content_length(size);
						res.keep_alive(req.keep_alive());
						return send(std::move(res));
					}
				}

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
                    #if defined (__WINDOWS__) || defined(WIN32)
                        Sleep(1 * 50); //50 ms
                    #else
                        usleep(1 * 50 * 1000);
                    #endif
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

