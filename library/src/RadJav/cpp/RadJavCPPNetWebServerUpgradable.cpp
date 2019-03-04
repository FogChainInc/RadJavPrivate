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
#include "cpp/RadJavCPPNetWebServerUpgradable.h"
#include "cpp/RadJavCPPEvent.h"

#ifdef USE_INSPECTOR
#include "RadJav.h"
#include "RadJavString.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <utility>

#include <algorithm>
#include <string>
#include <cctype>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			/// Try to find in the Haystack the Needle - ignore case
			bool findStringIC(const std::string & strHaystack, const std::string & strNeedle)
			{
				auto it = std::search(
					strHaystack.begin(), strHaystack.end(),
					strNeedle.begin(), strNeedle.end(),
					[](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
				);
				return (it != strHaystack.end());
			}

			// This function produces an HTTP response for the given
			// request. The type of the response object depends on the
			// contents of the request, so the interface requires the
			// caller to pass a generic lambda for receiving the response.
			template<
				class Body, class Allocator,
				class Send>
			void handle_request(
					http::request<Body, http::basic_fields<Allocator>>&& req,
					Send&& send)
			{
				// Returns a bad request response
				auto const bad_request =
					[&req](boost::beast::string_view why)
				{
					http::response<http::string_body> res{ http::status::bad_request, req.version() };
					res.set(http::field::content_type, "text/html");
					res.keep_alive(false);
					res.body() = why.to_string();
					res.prepare_payload();
					return res;
				};

				// Make sure we can handle the method
				if (	(req.method() != http::verb::get)
					 || ( ! findStringIC(req.base().target().to_string(), "/json") )   )
				{
					return send(bad_request("not a debug session"));
				}
				
				//prepare OK response template
				http::response<http::string_body> res{ http::status::ok, 10 };
				//TODO: check charset settings
				//res.set(http::field::content_type, "application/json; charset=UTF-8");
				//res.set(http::field::accept_charset, "UTF-8");
				res.set(http::field::content_type, "application/json; charset=UTF-8");
				res.set(http::field::cache_control, "no-cache");
				res.keep_alive(false);// req.keep_alive());

				String payload;
				
				do //switch for request type
				{
					std::string tempstr = req.base().target().to_string();
					auto str = req.base().target().to_string().substr(strlen("/json"), std::string::npos);

					if ((str.length() == 0) // here /json equals to /json/list in http request
						|| findStringIC(str, "/list"))
					{
						//send.prepareListPayload(&payload);
						payload.append("[ {"); payload.append("\n");
						payload.append("  \"description\": \"v8inspector instance\","); payload.append("\n");
						payload.append("  \"devtoolsFrontendUrl\": \"chrome-devtools://devtools/bundled/inspector.html?"
												"experiments=true&v8only=true&ws=127.0.0.1:9229/003000e9-00ee-406d-80ac-008100d3000e\","); payload.append("\r\n");
						payload.append("  \"id\": \"003000e9-00ee-406d-80ac-008100d3000e\","); payload.append("\n");
						payload.append("  \"title\": \"RadJav\","); payload.append("\n");
						payload.append("  \"type\": \"node\","); payload.append("\n");
						payload.append("  \"url\": \"file://\","); payload.append("\n");
						payload.append("  \"webSocketDebuggerUrl\": \"ws://127.0.0.1:9229/003000e9-00ee-406d-80ac-008100d3000e\""); payload.append("\n");
						payload.append("} ]"); payload.append("\n\n");
						break;
					}
					if (findStringIC(str, "/protocol"))
					{
						//prepareProtocolPayload(&payload);
						RadJav::throwException("get request : PROTOCOL");
						break;
					}
					if (findStringIC(str, "/version"))
					{
						//return;//send nothing
						payload.append("[ {"); payload.append("\r\n");
						payload.append("  \"Browser\": \"v8inspector\","); payload.append("\r\n");
						payload.append("  \"Protocol-Version\": \"1.3\","); payload.append("\r\n");
						payload.append("] }"); payload.append("\r\n");
						break;
					}
					if (findStringIC(str, "/activate"))
					{
						//prepareActivatePayload(&payload);
						RadJav::throwException("get request : ACTIVATE");
					}
				} while (false);

				if (payload.empty())
				{
					return send(bad_request("target [" + req.base().target().to_string() + "] is not valid"));
				}

				res.body() = payload;
				res.prepare_payload();
				return send(std::move(res));
			}

			WebServerUpgradable::WebServerUpgradable(String listenAddress, RJUSHORT port, RJBOOL useOwnThread)
			{
				io_context_ = nullptr;
				this->listenAddress = listenAddress;
				this->useOwnThread = useOwnThread;
				this->port = port;
				isAlive = false;
				thread = NULL;

				myThread = NULL;
			}

			WebServerUpgradable::~WebServerUpgradable()
			{
				close();

				DELETEOBJ(thread);
			}

			void WebServerUpgradable::myListenThread()
			{
				while (isAlive == true)
					io_context_->run_one();

				io_context_->stop();
			}

			void WebServerUpgradable::listen()
			{
				auto const address = boost::asio::ip::make_address(listenAddress);
				const int threads = 1;

				io_context_ = RJNEW boost::asio::io_context{ threads };
				upgradableListener = std::make_shared<UpgradableListener>(*io_context_, boost::asio::ip::tcp::endpoint{ address, port }, this);
				upgradableListener->run();

				isAlive = true;

				if (useOwnThread == true)
				{
					myThread = RJNEW std::thread(&WebServerUpgradable::myListenThread, this);
				}
				else
				{
					DELETEOBJ(thread);
					thread = RJNEW SimpleThread();
					thread->onStart = [this]()
						{
							while (isAlive == true)
								io_context_->run_one();

							io_context_->stop();
						};

					RadJav::addThread(thread);
				}
			}

			void WebServerUpgradable::send(String id, String message)
			{
				if (websocketSessions.size() > 0)
				{
					auto wssBegin = websocketSessions.find(id);
					auto wssEnd = websocketSessions.end ();

					if (wssBegin == wssEnd)
						return;

					//s.second->do_write(message);
					std::shared_ptr<WebServerUpgradable::WebSocketSession> session = websocketSessions.at(id);

					if (session->sessionID_.compare(id) == 0)
						session->do_write(message);
				}
			}

			void WebServerUpgradable::close()
			{
				{
					auto begin = httpSessions.begin();
					auto end = httpSessions.end();

					while (begin != end)
					{
						(*begin).second->do_close();
						begin++;
					}
				}

				{
					auto begin = websocketSessions.begin();
					auto end = websocketSessions.end();

					while (begin != end)
					{
						(*begin).second->do_close();
						begin++;
					}
				}

				if (upgradableListener)
					upgradableListener->close();

				upgradableListener.reset();

				if (myThread != NULL)
				{
					isAlive = false;
					myThread->join();

					DELETEOBJ(io_context_);
					DELETEOBJ(myThread);
				}
			}

			WebServerUpgradable::HttpUpgradableSession::HttpUpgradableSession(
				boost::asio::ip::tcp::socket socket, String sessionID, WebServerUpgradable* parent)
				: socket_(std::move(socket))
				, strand_(socket_.get_executor())
				, sessionID_(std::move(sessionID))
				, parent_(parent)
				, lambda_(*this)
			{
			}

			void WebServerUpgradable::HttpUpgradableSession::run ()
			{
				// Make sure we run on the strand
				if (!strand_.running_in_this_thread())
					return boost::asio::post(
						boost::asio::bind_executor(
							strand_,
							std::bind(
								&HttpUpgradableSession::run,
								shared_from_this())));

				do_read();
			}

			void WebServerUpgradable::HttpUpgradableSession::do_read()
			{
				// Make the request empty before reading,
				// otherwise the operation behavior is undefined.
				req_ = {};

				// Read a request
				http::async_read(socket_, buffer_, req_,
					boost::asio::bind_executor(
						strand_,
						std::bind(
							&HttpUpgradableSession::on_read,
							shared_from_this(),
							std::placeholders::_1)));
			}

			void WebServerUpgradable::HttpUpgradableSession::on_read(boost::system::error_code ec)
			{
				// If client closed the connection
				if (ec == http::error::end_of_stream)
					return do_close();

				// Throw exception on any other read error
				if (ec)
				{
					RadJav::throwException("Read error");
					return;
				}

				// See if it is a WebSocket Upgrade
				if (websocket::is_upgrade(req_))
				{
					// Create a WebSocket session by transferring the socket
					auto session = std::make_shared<WebSocketSession>(std::move(socket_), this->sessionID_, parent_);

					session->do_accept(std::move(req_));

					return;
				}

				String *id = RJNEW String(sessionID_);
				String *message = RJNEW String(boost::beast::buffers_to_string(buffer_.data()));
				//String *message = RJNEW String(req_.target ().to_string ());

				String *result = (String *)parent_->executeCppEvent("httpReceive", Array<void *>({ id, message }));
				String data = "";
				
				if (result != NULL)
				{
					data = *result;
					DELETEOBJ(result);
				}

				DELETEOBJ(id);
				DELETEOBJ(message);

				// Send the Chrome response
				handle_request(std::move(req_), lambda_);

				/*http::response<http::string_body> res{ http::status::ok, 10 };

				res.set(http::field::content_type, "application/json; charset=UTF-8");
				res.set(http::field::cache_control, "no-cache");
				res.keep_alive(false);

				res.body() = data;
				res.prepare_payload();
				lambda_(std::move(res));*/

				buffer_.consume(buffer_.size ());

				do_read();
			}

			void WebServerUpgradable::HttpUpgradableSession::on_write(boost::system::error_code ec, std::size_t bytes_transferred, bool close)
			{
				boost::ignore_unused(bytes_transferred);

				if (ec)
				{
					return RadJav::throwException("on_write error");
				}

				if (close)
				{
					// This means we should close the connection, usually because
					// the response indicated the "Connection: close" semantic.
					return do_close();
				}

				String *id = RJNEW String(sessionID_.c_str());
				String *message = RJNEW String(boost::beast::buffers_to_string(buffer_.data()).c_str());

				parent_->executeCppEvent("httpSend", Array<void *>({ id, message }));

				DELETEOBJ(id);
				DELETEOBJ(message);

				// Clear the buffer
				buffer_.consume(buffer_.size());
				
				// We're done with the response so delete it
				res_.reset();

				// Read another request
				do_read();
			}

			void WebServerUpgradable::HttpUpgradableSession::do_close()
			{
				// Send a TCP shutdown
				boost::system::error_code ec;
				socket_.shutdown(tcp::socket::shutdown_send, ec);

				// At this point the connection is closed gracefully
			}

			WebServerUpgradable::WebSocketSession::WebSocketSession(tcp::socket socket, String sessionID, WebServerUpgradable* parent)
				: ws_(std::move(socket))
				, strand_(ws_.get_executor())
				, sessionID_(sessionID)
				, parent_(parent)
			{
			}

			void WebServerUpgradable::WebSocketSession::on_accept(boost::system::error_code ec)
			{
				if (ec)
				{
					RadJav::throwException("websocket: on_accept error");
					return;
				}

				String *id = RJNEW String(sessionID_);

				RJBOOL *result = (RJBOOL *)parent_->executeCppEvent("webSocketUpgrade", Array<void *>({ id }));
				parent_->websocketSessions.insert(HashMapPair<std::string, std::shared_ptr<WebServerUpgradable::WebSocketSession> >(sessionID_, shared_from_this()));

				DELETEOBJ(result);

				DELETEOBJ(id);

				do_read();
			}

			void WebServerUpgradable::WebSocketSession::do_read()
			{
				// Read a message into our buffer
				ws_.async_read(
					buffer_,
					boost::asio::bind_executor(
						strand_,
						std::bind(
							&WebSocketSession::on_read,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}

			void WebServerUpgradable::WebSocketSession::do_write(String message)
			{
				ws_.text(true);
				//static boost::beast::multi_buffer b;
				//b.consume(b.size());
				//boost::beast::ostream(b) << message;
				//buffer_.consume(buffer_.size());
				//boost::beast::ostream(buffer_) << message;
				boost::beast::multi_buffer b;
				boost::beast::ostream(b) << message;
				boost::beast::error_code ec;
				ws_.auto_fragment(false);
				ws_.write(b.data(), ec);
				if (ec)
				{
					printf("\n << ws_.write(b.data(), ec); error: \n %s \n", ec.message().c_str());
				}
				//ws_.async_write(
				//	//b.data(),
				//	b,
				//	boost::asio::bind_executor(
				//		strand_,
				//		std::bind(
				//			&WebSocketSession::on_write,
				//			shared_from_this(),
				//			std::placeholders::_1,
				//			std::placeholders::_2)));

				String *id = RJNEW String(sessionID_);
				String *recvMsg = RJNEW String(message);

				parent_->executeCppEvent("webSocketSend", Array<void *>({ id, recvMsg }));

				DELETEOBJ(id);
				DELETEOBJ(recvMsg);

				b.consume(b.size ());
			}
			void WebServerUpgradable::WebSocketSession::on_read(boost::system::error_code ec, std::size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				// This indicates that the websocket_session was closed
				if (ec == websocket::error::closed)
					return;

				if (ec)
				{
					RadJav::throwException("websocket: on_read error - " + ec.message ());
					return;
				}

				String *id = RJNEW String(sessionID_);
				String *message = RJNEW String(boost::beast::buffers_to_string(buffer_.data()).c_str());

				parent_->executeCppEvent("webSocketReceive", Array<void *>({ id, message }));

				DELETEOBJ(id);
				DELETEOBJ(message);

				// Clear the buffer
				buffer_.consume(buffer_.size());

				// Do another read
				do_read();
			}

			void WebServerUpgradable::WebSocketSession::on_write(boost::system::error_code ec, std::size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				// Happens when the timer closes the socket
				if (ec == boost::asio::error::operation_aborted)
					return;

				if (ec)
				{
					RadJav::throwException("websocket: on_write error");
					return;
				}

				String *id = RJNEW String(sessionID_.c_str());
				String *message = RJNEW String(boost::beast::buffers_to_string(buffer_.data()).c_str());

				parent_->executeCppEvent("webSocketSend", Array<void *>({ id, message }));

				DELETEOBJ(id);
				DELETEOBJ(message);

				// Clear the buffer
				buffer_.consume(buffer_.size());

				// Do another read
				do_read();
			}

			void WebServerUpgradable::WebSocketSession::do_close()
			{
				// Send a TCP shutdown
				//boost::system::error_code ec;
				//ws_.close(boost::beast::websocket::close_reason::close_reason ());

				// At this point the connection is closed gracefully
			}

			WebServerUpgradable::UpgradableListener::UpgradableListener(
				boost::asio::io_context& ioc,
				boost::asio::ip::tcp::endpoint endpoint,
				WebServerUpgradable* parent)
					: acceptor_(ioc), socket_(ioc), parent_(parent)
			{
				boost::system::error_code ec;

				// Open the acceptor
				acceptor_.open(endpoint.protocol(), ec);
				if (ec)
				{
					RadJav::throwException("Open error");
					return;
				}

				// Allow address reuse
				acceptor_.set_option(boost::asio::socket_base::reuse_address(true));
				if (ec)
				{
					RadJav::throwException("set_option error");
					return;
				}

				// Bind to the server address
				acceptor_.bind(endpoint, ec);
				if (ec)
				{
					RadJav::throwException("Bind error");
					return;
				}

				// Start listening for connections
				acceptor_.listen(boost::asio::socket_base::max_listen_connections, ec);
				if (ec)
				{
					RadJav::throwException("Listen error");
					return;
				}
			}

			// Start accepting incoming connections
			void WebServerUpgradable::UpgradableListener::run()
			{
				if (acceptor_.is_open())
				{
					do_accept();
				}
			}

			void WebServerUpgradable::UpgradableListener::close()
			{
				boost::system::error_code ec;
				socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
			}

			void WebServerUpgradable::UpgradableListener::do_accept()
			{
				acceptor_.async_accept(
					socket_,
					std::bind(
						&UpgradableListener::on_accept,
						shared_from_this(),
						std::placeholders::_1));
			}

			void WebServerUpgradable::UpgradableListener::on_accept(boost::system::error_code ec)
			{
				if (ec)
				{
					RadJav::throwException("Accept error");
				}
				else
				{
					// Create the http session and run it
					String session_id = boost::uuids::to_string(boost::uuids::random_generator()());
					String *id = RJNEW String(session_id);

					parent_->executeCppEvent("httpAccept", Array<void *>({ id }));

					DELETEOBJ(id);

					auto session = std::make_shared<HttpUpgradableSession>(std::move(socket_), session_id, parent_);
					parent_->httpSessions.push_back(std::make_pair(session_id, session));
					session->run();
				}

				// Accept another connection
				do_accept();
			}
		}
	}
}
#endif

