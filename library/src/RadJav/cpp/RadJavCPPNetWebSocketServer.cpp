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
#include "cpp/RadJavCPPNetWebServer.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#endif


namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			WebSocketServer::WebSocketServer()
			{
				m_port = 0;
				m_isAlive = false;
			}

			WebSocketServer::~WebSocketServer()
			{
				DELETEOBJ(m_io_context);
			}
			
			#ifdef USE_V8
			void WebSocketServer::on(String event_, v8::Local<v8::Function> func_)
			{
			}
			#endif

			void WebSocketServer::listen(unsigned short port_)
			{
				auto const address = boost::asio::ip::make_address("127.0.0.1");
				m_port = port_;
																				
				// The io_context is required for all I/O
				auto const threads = std::max<int>(1, std::atoi("1"));
				m_io_context = RJNEW boost::asio::io_context{ threads };
				//std::make_shared<WebSocketServerListener>(*m_io_context, boost::asio::ip::tcp::endpoint{ address, m_port })->run();
				m_listener = std::make_shared<WebSocketServerListener>(*m_io_context, boost::asio::ip::tcp::endpoint(address, m_port), &m_sessions);

				m_listener -> set_on_accept_callback(m_serverAcceptEvent);
				m_listener -> set_on_receive_callback(m_serverReceiveEvent);
				
				m_listener -> run();

				WebServerThread* thread = RJNEW WebServerThread(m_io_context);
				thread->Run();
#ifdef GUI_USE_WXWIDGETS
				m_isAlive = thread->IsAlive();
#else
				m_isAlive = true;
#endif
			}

			void WebSocketServer::send(String id_, String message_)
			{
				for (const auto& s : m_sessions)
					if (s.m_session_id == id_)
					{
						s.m_session->do_write(message_);
						break;
					}
			}


		        void WebSocketServer::send(String id_, const void* message_, int msg_len)
			{
				for (const auto& s : m_sessions)
					if (s.m_session_id == id_)
					{
					        s.m_session->do_write(message_, msg_len);
						break;
					}
			}
		  
			void WebSocketServer::sendToAll(String message_)
			{
				for(const auto& s: m_sessions)
					s.m_session->do_write(message_);
			}

			String WebSocketServer::receive()
			{
				if (m_sessions.size() > 0)
				{
				  std::string message = m_sessions[0].m_last_message;
				  m_sessions[0].m_last_message = "";

					return message;
				}
					

				return String("");
			}

			void WebSocketServer::close()
			{
				m_io_context->stop();
			}

			void WebSocketServer::set_on_accept_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverAcceptEvent = callback;
			}

			void WebSocketServer::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			}
		  

		  
		        WebSocketServer::WebSocketServerSession::WebSocketServerSession(boost::asio::ip::tcp::socket socket_, std::string sessionID_,
											std::vector <RadJAV::CPP::Net::WebSocketServer::session_data> *sessions_)
			  : m_ws(std::move(socket_)), m_strand(m_ws.get_executor()), m_sessionID(sessionID_), m_sessions(sessions_)
			{
			}

			void WebSocketServer::WebSocketServerSession::run ()
			{

				// Accept the websocket handshake
				m_ws.async_accept(
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSession::on_accept,
							shared_from_this(),
							std::placeholders::_1)));

			}

		        void WebSocketServer::WebSocketServerSession::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			}
		  
			void WebSocketServer::WebSocketServerSession::on_accept(boost::system::error_code ec_)
			{
				if (ec_)
				{
					RadJav::throwException("on_accept error");

					return;
				}

				// Read a message
				do_read();
			}

			void WebSocketServer::WebSocketServerSession::do_read()
			{
			  
				// Read a message into our buffer
				m_ws.async_read(
					m_readBuffer,
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSession::on_read,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}

			void WebSocketServer::WebSocketServerSession::do_write(String message_)
			{
			  m_ws.text(true);
				m_activeMessage = std::make_shared<std::string>(std::move(message_));
				m_ws.async_write(
					boost::asio::buffer(*m_activeMessage),
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSession::on_write,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}


		        void WebSocketServer::WebSocketServerSession::do_write(const void *message_, int msg_len)
			{
			  //TODO: will have to copy the message
			  m_ws.binary(true);
				m_ws.async_write(
					boost::asio::buffer(message_, msg_len),
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSession::on_write,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}

		  v8::Local<v8::Function> WebSocketServer::WebSocketServerSession::get_on_receive_callback()
			{
			  return m_serverReceiveEvent->Get(V8_JAVASCRIPT_ENGINE->isolate);
			}
		  
			void WebSocketServer::WebSocketServerSession::on_read(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
			{
			  
				boost::ignore_unused(bytes_transferred);

				// This indicates that the session was closed
				if (ec == boost::beast::websocket::error::closed)
				{
					//Remove the session from the list
					for (auto it = m_sessions -> begin(); it != m_sessions -> end(); ++it)
						if (it->m_session_id == m_sessionID)
						{
							m_sessions -> erase(it);
							break;
						}

					return;
				}


				if (ec)
				{
					RadJav::throwException("Read error");

					return;
				}

				#ifdef USE_V8
				
				if (m_serverReceiveEvent != nullptr)				
				{
				v8::Locker myLocker(V8_JAVASCRIPT_ENGINE->isolate);
				V8_JAVASCRIPT_ENGINE->isolate -> Enter();					  
				  
				v8::Local<v8::Function> evt = get_on_receive_callback();

					String id = m_sessionID.c_str();

					v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[2];

					args[0] = id.toV8String(V8_JAVASCRIPT_ENGINE->isolate);

					if (m_ws.got_text())
					  {
					    String message = boost::beast::buffers_to_string(m_readBuffer.data()).c_str();
					    args[1] = message.toV8String(V8_JAVASCRIPT_ENGINE->isolate);
					  }
					else
					  {
					    auto msgLen = boost::beast::buffers_front(m_readBuffer.data()).size();
					    auto message = v8::ArrayBuffer::New(V8_JAVASCRIPT_ENGINE->isolate, msgLen);
					    std::memcpy(message -> GetContents().Data(), boost::beast::buffers_front(m_readBuffer.data()).data(), msgLen);
					    args[1] = message;
					  }


					if (V8_JAVASCRIPT_ENGINE->v8IsNull(evt) == false)
						evt->Call(V8_JAVASCRIPT_ENGINE->globalContext->Global(), 2, args);

					DELETE_ARRAY(args);
				V8_JAVASCRIPT_ENGINE->isolate -> Exit();					  					
				}
				#endif
				m_readBuffer.consume(m_readBuffer.size());
				do_read();				  
			}

			void WebSocketServer::WebSocketServerSession::on_write(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);

				if (ec)
				{
					RadJav::throwException("Write error");

					return;
				}

			}

			WebSocketServer::WebSocketServerListener::WebSocketServerListener(
				boost::asio::io_context& ioc_,
				boost::asio::ip::tcp::endpoint endpoint_,
				std::vector <RadJAV::CPP::Net::WebSocketServer::session_data> *sessions_
								  )
			  : m_acceptor(ioc_), m_socket(ioc_), m_sessions(sessions_)
			{
				boost::system::error_code ec;

				// Open the acceptor
				m_acceptor.open(endpoint_.protocol(), ec);
				if (ec)
				{
					RadJav::throwException("Open error");

					return;
				}

				// Allow address reuse
				m_acceptor.set_option(boost::asio::socket_base::reuse_address(true));
				if (ec)
				{
					RadJav::throwException("set_option error");

					return;
				}

				// Bind to the server address
				m_acceptor.bind(endpoint_, ec);
				if (ec)
				{
					RadJav::throwException("Bind error");

					return;
				}

				// Start listening for connections
				m_acceptor.listen(
					boost::asio::socket_base::max_listen_connections, ec);
				if (ec)
				{
					RadJav::throwException("Listen error");

					return;
				}
			}
		  
		        void WebSocketServer::WebSocketServerListener::set_on_accept_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverAcceptEvent = callback;
			}

		        void WebSocketServer::WebSocketServerListener::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			}
		  
			// Start accepting incoming connections
			void WebSocketServer::WebSocketServerListener::run()
			{
				if (!m_acceptor.is_open())
					return;

				do_accept();
			}

			void WebSocketServer::WebSocketServerListener::do_accept()
			{
			  
				m_acceptor.async_accept(
					m_socket,
					std::bind(
						&WebSocketServerListener::on_accept,
						shared_from_this(),
						std::placeholders::_1));
			}

		        v8::Local<v8::Function> WebSocketServer::WebSocketServerListener::get_on_accept_callback()
			{

			  return v8::Local<v8::Function>::Cast(RadJAV::CPP::Net::WebSocketServer::WebSocketServerListener::m_serverAcceptEvent->Get(V8_JAVASCRIPT_ENGINE->isolate));
			}
		        v8::Persistent<v8::Function> *WebSocketServer::WebSocketServerListener::get_on_receive_persistent_evt()
			{

			  return m_serverReceiveEvent;
			}
		  

			void WebSocketServer::WebSocketServerListener::on_accept(boost::system::error_code ec)
			{
				if (ec)
				{
					RadJav::throwException("Accept error");
				}
				else
				{
					// Create the session and run it
					session_data this_session;

					std::string sessionId = boost::uuids::to_string(boost::uuids::random_generator()());
					auto session = std::make_shared<WebSocketServerSession>(std::move(m_socket), sessionId, m_sessions);
					session -> set_on_receive_callback(get_on_receive_persistent_evt());
					

					this_session.m_session = session;
					this_session.m_session_id = sessionId;
					m_sessions -> push_back(this_session);
					session->run();


					#ifdef USE_V8
					if (m_serverAcceptEvent != nullptr)					
					{
					  v8::Locker myLocker(V8_JAVASCRIPT_ENGINE->isolate);
					  V8_JAVASCRIPT_ENGINE->isolate -> Enter();					  

					  v8::Local<v8::Function> evt = get_on_accept_callback();
						

						String id = sessionId.c_str();
						v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[1];
						args[0] = id.toV8String(V8_JAVASCRIPT_ENGINE->isolate);
						
						if (V8_JAVASCRIPT_ENGINE->v8IsNull(evt) == false)
						  evt->Call(V8_JAVASCRIPT_ENGINE->globalContext->Global(), 1, args);
						
						DELETE_ARRAY(args);
					  V8_JAVASCRIPT_ENGINE->isolate -> Exit();					  
					  
						
					}
					#endif

				}	

				// Accept another connection
				do_accept();
			}
		}
	}
}

