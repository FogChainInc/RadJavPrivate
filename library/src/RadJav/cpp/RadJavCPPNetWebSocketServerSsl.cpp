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
#include "cpp/RadJavCPPNetWebSocketServerSsl.h"

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
		        WebSocketServerSsl::WebSocketServerSsl(std::map<std::string, std::string> &parms) :
			m_ctx(boost::asio::ssl::context::sslv23)
			{

			  //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
				m_port = 0;
				m_isAlive = false;

				//for (auto elem : parms)
				//std::cout << elem.first << " -> " << elem.second << std::endl;
				
			        m_ctx.use_certificate_chain_file(parms["certificateStore"]);
				m_ctx.use_private_key_file(parms["keyStore"], boost::asio::ssl::context::pem);

				m_ctx.set_password_callback(
							    [&parms](std::size_t,
							       boost::asio::ssl::context_base::password_purpose)
							    {
							      std::cout << "PWD CALLBACK" << std::endl;
							      return parms["keystorePwd"];
							    });		    
				
				if (parms.find("verifyMode") != parms.end() && parms["verifyMode"] != "")
				  {
				    std::string::size_type posBegin, posEnd  = 0;
				    std::string input = parms["verifyMode"];
				    std::string delims = ",";

				    input.erase(std::remove_if(input.begin(), 
							       input.end(),
							       [](char x){return std::isspace(x);}),input.end());
				    
				    //boost::asio::ssl::verify_mode verifyMode;
				    while ((posBegin = input.find_first_not_of(delims,posEnd)) != std::string::npos)
				      {
					posEnd = input.find_first_of(delims,posBegin);
					if (posEnd == std::string::npos) posEnd = input.length();

					std::cout << '\'' << input.substr(posBegin,posEnd-posBegin) << '\'' << std::endl;

					if (input.substr(posBegin,posEnd-posBegin) == "VerifyNone")
					  m_verifyMode |= boost::asio::ssl::verify_none;
					else if (input.substr(posBegin,posEnd-posBegin) == "VerifyPeer")
					  m_verifyMode |= boost::asio::ssl::verify_peer;
					else if (input.substr(posBegin,posEnd-posBegin) == "VerifyFailIfNoPeerCert")
					  m_verifyMode |= boost::asio::ssl::verify_fail_if_no_peer_cert;
					else if (input.substr(posBegin,posEnd-posBegin) == "VerifyClientOnce")
					  m_verifyMode |= boost::asio::ssl::verify_client_once;
					else
					  {
					    RadJav::throwException(std::string("WebSocket Server: invalid sll verifier flag: ") + input.substr(posBegin,posEnd-posBegin));
					    break;
					  }
				      }

				    m_ctx.set_verify_mode(m_verifyMode);
			      
				  }

				// If trustStore provided, load it.
				if (parms.find("trustStore") != parms.end() && parms["trustStore"] != "")
				  m_ctx.load_verify_file(parms["trustStore"]);
				else
				  {
				    if (m_verifyMode & boost::asio::ssl::verify_peer ||
					m_verifyMode & boost::asio::ssl::verify_fail_if_no_peer_cert ||
					m_verifyMode & boost::asio::ssl::verify_client_once)
					    RadJav::throwException("WebSocket Server: no trustStore defined, but one of VerifyPeer, VerifyFailIfNoPeerCert or VerifyClientOnce was specified");				      
				  }
				
			}

			WebSocketServerSsl::~WebSocketServerSsl()
			{
				DELETEOBJ(m_io_context);
			}
			
			void WebSocketServerSsl::listen(unsigned short port_)
			{
			  //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
				auto const address = boost::asio::ip::make_address("127.0.0.1");
				m_port = port_;
																				
				// The io_context is required for all I/O
				auto const threads = std::max<int>(1, std::atoi("1"));
				m_io_context = RJNEW boost::asio::io_context{ threads };
				m_listener = std::make_shared<WebSocketServerSslListener>(*m_io_context, std::ref(m_ctx), boost::asio::ip::tcp::endpoint(address, m_port), &m_sessions);

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

			void WebSocketServerSsl::send(String id_, String message_)
			{
				for (const auto& s : m_sessions)
					if (s.m_session_id == id_)
					{
						s.m_session->do_write(message_);
						break;
					}
			}


		        void WebSocketServerSsl::send(String id_, const void* message_, int msg_len)
			{
				for (const auto& s : m_sessions)
					if (s.m_session_id == id_)
					{
					        s.m_session->do_write(message_, msg_len);
						break;
					}
			}
		  
			void WebSocketServerSsl::sendToAll(String message_)
			{
				for(const auto& s: m_sessions)
					s.m_session->do_write(message_);
			}

			String WebSocketServerSsl::receive()
			{
				if (m_sessions.size() > 0)
				{
				  std::string message = m_sessions[0].m_last_message;
				  m_sessions[0].m_last_message = "";

					return message;
				}
					

				return String("");
			}

			void WebSocketServerSsl::close()
			{
				m_io_context->stop();
			}

			void WebSocketServerSsl::set_on_accept_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverAcceptEvent = callback;
			}

			void WebSocketServerSsl::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			}
		  

		  
		        WebSocketServerSsl::WebSocketServerSslSession::WebSocketServerSslSession(boost::asio::ip::tcp::socket socket_, 
												 boost::asio::ssl::context &context_,
												 std::string sessionID_,
											std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *sessions_)
			  : m_ws(std::move(socket_), context_), m_strand(m_ws.get_executor()), m_sessionID(sessionID_), m_sessions(sessions_)
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;

			  m_ws.next_layer().set_verify_callback([this](bool preVerified, boost::asio::ssl::verify_context& vctx) -> bool {
			      return verify_cert(preVerified, vctx);
			    });


			}

		  
		        bool WebSocketServerSsl::WebSocketServerSslSession::verify_cert(bool preVerified,
											boost::asio::ssl::verify_context &vctx)
			{

			  //			  std::cout << __PRETTY_FUNCTION__ << ": PreVerified status: " << preVerified << std::endl;


			  char subject_name[256];
			  X509* cert = X509_STORE_CTX_get_current_cert(vctx.native_handle());
			  X509_NAME_oneline(X509_get_subject_name(cert), subject_name, 256);
			  std::cout << "Verifying " << subject_name << ": " << preVerified << "\n";
			  
			  char issuer_name[256];
			  X509_NAME_oneline(X509_get_issuer_name(cert), issuer_name, 256);
			  std::cout << "ISSUER " << subject_name << ": " << preVerified << "\n";
			  
			  BIO *mem = BIO_new(BIO_s_mem());
			  X509_NAME_print_ex(mem, X509_get_subject_name(cert), 0, XN_FLAG_ONELINE & ~XN_FLAG_SPC_EQ);
			  
			  BUF_MEM *ptr;
			  BIO_get_mem_ptr(mem, &ptr);

			  std::cout << "Length: " << ptr -> length << std::endl << std::flush;
			  std::cout << ptr -> data << std::endl << std::flush;


			  int status = X509_check_ca(cert);
			  std::cout << "CA status: " << status << std::endl << std::flush;
    

			  BIO_set_close(mem, BIO_NOCLOSE); /* So BIO_free() leaves BUF_MEM alone */
			  BIO_free(mem);
			  
			  std::cout << "Length: " << ptr -> length << std::endl << std::flush;
			  std::cout << ptr -> data << std::endl << std::flush;

			  delete ptr;

			  
			  return preVerified;
			}
			  
			void WebSocketServerSsl::WebSocketServerSslSession::run()
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  m_ws.next_layer().async_handshake(
							    boost::asio::ssl::stream_base::server,
							    boost::asio::bind_executor(
										       m_strand,
										       std::bind(
												 &WebSocketServerSslSession::on_handshake,
												 shared_from_this(),
												 std::placeholders::_1)));
			}

		        void WebSocketServerSsl::WebSocketServerSslSession::on_handshake (boost::system::error_code ec_)
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;

				if (ec_)
				{
				  //				  std::cout << __PRETTY_FUNCTION__ << ": HANDSHAKE ERROR: " << ec_.message() << std::endl << std::flush;
					RadJav::throwException("on_handshake error");

					return;
				}

				m_ws.async_accept(
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSslSession::on_accept,
							shared_from_this(),
							std::placeholders::_1)));

			}
		  
		        void WebSocketServerSsl::WebSocketServerSslSession::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			}
		  
			void WebSocketServerSsl::WebSocketServerSslSession::on_accept(boost::system::error_code ec_)
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
				if (ec_)
				{
					RadJav::throwException("on_accept error");

					return;
				}

				// Read a message
				do_read();
			}

			void WebSocketServerSsl::WebSocketServerSslSession::do_read()
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  
				// Read a message into our buffer
				m_ws.async_read(
					m_readBuffer,
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSslSession::on_read,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}

			void WebSocketServerSsl::WebSocketServerSslSession::do_write(String message_)
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  m_ws.text(true);
				m_activeMessage = std::make_shared<std::string>(std::move(message_));
				m_ws.async_write(
					boost::asio::buffer(*m_activeMessage),
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSslSession::on_write,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}


		        void WebSocketServerSsl::WebSocketServerSslSession::do_write(const void *message_, int msg_len)
			{
			  //TODO: will have to copy the message
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  m_ws.binary(true);
				m_ws.async_write(
					boost::asio::buffer(message_, msg_len),
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSslSession::on_write,
							shared_from_this(),
							std::placeholders::_1,
							std::placeholders::_2)));
			}

		  v8::Local<v8::Function> WebSocketServerSsl::WebSocketServerSslSession::get_on_receive_callback()
			{
			  return m_serverReceiveEvent->Get(V8_JAVASCRIPT_ENGINE->isolate);
			}
		  
			void WebSocketServerSsl::WebSocketServerSslSession::on_read(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;			  
			  
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

			void WebSocketServerSsl::WebSocketServerSslSession::on_write(
				boost::system::error_code ec,
				std::size_t bytes_transferred)
			{
				boost::ignore_unused(bytes_transferred);
				//			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;			  

				if (ec)
				{
					RadJav::throwException("Write error");

					return;
				}

			}

			WebSocketServerSsl::WebSocketServerSslListener::WebSocketServerSslListener(
				boost::asio::io_context& ioc_,
				std::reference_wrapper<boost::asio::ssl::context> context_,
				boost::asio::ip::tcp::endpoint endpoint_,
				std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *sessions_
								  )
			    : m_acceptor(ioc_), m_ctx(context_), m_socket(ioc_), m_sessions(sessions_)
			    
			{
			  
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;			  
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
		  
		        void WebSocketServerSsl::WebSocketServerSslListener::set_on_accept_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverAcceptEvent = callback;
			}

		        void WebSocketServerSsl::WebSocketServerSslListener::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			}
		  
			// Start accepting incoming connections
			void WebSocketServerSsl::WebSocketServerSslListener::run()
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;			  
				if (!m_acceptor.is_open())
					return;

				do_accept();
			}

			void WebSocketServerSsl::WebSocketServerSslListener::do_accept()
			{
			  
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;			  
				m_acceptor.async_accept(
					m_socket,
					std::bind(
						&WebSocketServerSslListener::on_accept,
						shared_from_this(),
						std::placeholders::_1));
			}

		        v8::Local<v8::Function> WebSocketServerSsl::WebSocketServerSslListener::get_on_accept_callback()
			{

			  return v8::Local<v8::Function>::Cast(RadJAV::CPP::Net::WebSocketServerSsl::WebSocketServerSslListener::m_serverAcceptEvent->Get(V8_JAVASCRIPT_ENGINE->isolate));
			}
		        v8::Persistent<v8::Function> *WebSocketServerSsl::WebSocketServerSslListener::get_on_receive_persistent_evt()
			{

			  return m_serverReceiveEvent;
			}
		  

			void WebSocketServerSsl::WebSocketServerSslListener::on_accept(boost::system::error_code ec)
			{
			  //			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;			  
				if (ec)
				{
					RadJav::throwException("Accept error");
				}
				else
				{
					// Create the session and run it
					session_data this_session;

					std::string sessionId = boost::uuids::to_string(boost::uuids::random_generator()());
					auto session = std::make_shared<WebSocketServerSslSession>(std::move(m_socket), m_ctx, sessionId, m_sessions);
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

