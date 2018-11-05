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

#ifdef USE_INSPECTOR
#include "RadJav.h"
#include "RadJavString.h"
#include "cpp/RadJavCPPNetWebServer.h"
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <utility>

#include <algorithm>
#include <string>
#include <cctype>
#include "cpp/RadJavCPPAgentIO.h"

std::vector<std::pair<std::string, std::shared_ptr<RadJAV::CPP::Net::WebServerUpgradable::HttpUpgradableSession>>> RadJAV::CPP::Net::WebServerUpgradable::sessions_;

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
				void
				handle_request(
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

				std::string payload;
				
				do //switch for request type
				{
					auto str = req.base().target().to_string().substr(strlen("/json"), std::string::npos);

					if ((str.length() == 0) // here /json equals to /json/list in http request
						|| findStringIC(str, "/list"))
					{
						//send.prepareListPayload(&payload);
						payload.append("[ {"); payload.append("\n");
						payload.append("  \"description\": \"v8inspector instance\","); payload.append("\n");
						payload.append("  \"devtoolsFrontendUrl\": \"chrome-devtools://devtools/bundled/inspector.html?"
												"experiments=true&v8only=true&ws=10.130.41.58:9229/003000e9-00ee-406d-80ac-008100d3000e\","); payload.append("\r\n");
						payload.append("  \"id\": \"003000e9-00ee-406d-80ac-008100d3000e\","); payload.append("\n");
						payload.append("  \"title\": \"../../../examples/window/window.xrj\","); payload.append("\n");
						payload.append("  \"type\": \"node\","); payload.append("\n");
						payload.append("  \"url\": \"file://\","); payload.append("\n");
						payload.append("  \"webSocketDebuggerUrl\": \"ws://10.130.41.58:9229/003000e9-00ee-406d-80ac-008100d3000e\""); payload.append("\n");
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
						payload.append("  \"Protocol-Version\": \"1.1\","); payload.append("\r\n");
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

			WebServerUpgradable::WebServerUpgradable(inspector::InspectorIoDelegate* delegate, String path_name, String script_name, RJBOOL wait_for_connect)
			{
				port = 0;
				isAlive = false;
				io_context_ = nullptr;
				io_delegate_ = delegate;
				path_name_ = path_name;
				script_name_ = script_name;
				is_waiting_ = true;
				wait_for_connect_ = wait_for_connect;
				lock_ = nullptr;
			}

			WebServerUpgradable::~WebServerUpgradable()
			{
				DELETEOBJ(io_context_);
			}

			void WebServerUpgradable::setIOCallback(inspector::InspectorIoDelegate* delegate)
			{
				io_delegate_ = delegate;
			}

			void WebServerUpgradable::listen(v8::Isolate* isolate, inspector::Semaphore* lock)
			{
				auto const address = boost::asio::ip::make_address("0.0.0.0");
				const unsigned short port = 9229;
				const int threads = 1;

				lock_ = lock;
				io_context_ = RJNEW boost::asio::io_context{ threads };
				std::make_shared<UpgradableListener>(*io_context_, boost::asio::ip::tcp::endpoint{ address, port }, this)->run();

/*#ifdef GUI_USE_WXWIDGETS
				
				WebServerThread* thread = RJNEW WebServerThread(io_context_);
				thread->Run();
				isAlive = thread->IsAlive();
#else
				//blocking execution
				isAlive = true;
				io_context_->run();
#endif*/

				SimpleThread *thread = RJNEW SimpleThread();
				thread->onStart = [this]()
				{
					this->isAlive = true;
					this->io_context_->run();
				};

				#ifdef USE_V8
					V8_JAVASCRIPT_ENGINE->addThread(thread);
				#endif
			}

			void WebServerUpgradable::send(String id, String message)
			{
				for (const auto& s : sessions_)
					if (s.first == id)
					{
						//s.second->do_write(message);
						if ((this->websocket_session_) && (this->websocket_session_->sessionID_.compare(id) == 0))
						{
							websocket_session_->do_write(message);
						}
						break;
					}
			}

			void WebServerUpgradable::close()
			{
				io_context_->stop();
			}

			WebServerUpgradable::HttpUpgradableSession::HttpUpgradableSession(
				boost::asio::ip::tcp::socket socket, std::string sessionID, WebServerUpgradable* parent)
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
					assert(parent_->io_delegate_);
					parent_->io_delegate_->setSessionID(this->sessionID_);
					auto session = std::make_shared<WebSocketSession>(std::move(socket_), this->sessionID_, parent_);
					parent_->setWebSocketSession(session);
					session->do_accept(std::move(req_));
					return;
				}

				// Send the Chrome response
				handle_request(std::move(req_), lambda_);

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

				// Clear the buffer
				buffer_.consume(buffer_.size());

				// We're done with the response so delete it
				res_ = nullptr;

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

			WebServerUpgradable::WebSocketSession::WebSocketSession(tcp::socket socket, std::string sessionID, WebServerUpgradable* parent)
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
			}
			void WebServerUpgradable::WebSocketSession::on_read(boost::system::error_code ec, std::size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				// This indicates that the websocket_session was closed
				if (ec == websocket::error::closed)
					return;

				if (ec)
				{
					RadJav::throwException("websocket: on_read error");
					return;
				}

				if (parent_->io_delegate_)
				{
					if (parent_->isWaiting())
					{
						//check for isWaiting message that means frontend is ready
						
						if (boost::beast::buffers_to_string(buffer_.data()).find("\"Runtime.runIfWaitingForDebugger\"") != std::string::npos) {
							this->parent_->is_waiting_ = false;
							parent_->lock_->Notify();
						}
					}
					parent_->io_delegate_->sendMessageToInspector(boost::beast::buffers_to_string(buffer_.data()).c_str());
				}

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

				// Clear the buffer
				buffer_.consume(buffer_.size());

				// Do another read
				do_read();
			}

			void WebServerUpgradable::setWebSocketSession(const std::shared_ptr<WebSocketSession>& session)
			{
				websocket_session_ = session;
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
					std::string session_id = boost::uuids::to_string(boost::uuids::random_generator()());
					auto session = std::make_shared<HttpUpgradableSession>(std::move(socket_), session_id, parent_);
					sessions_.push_back(std::make_pair(session_id, session));
					session->run();
				}

				// Accept another connection
				do_accept();
			}
		}
	}
}
#endif

