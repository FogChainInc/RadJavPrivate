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
		        const int NID_COMMON_NAME = 13;
		        const int NID_COUNTRY = 14;
		        const int NID_LOCALITY = 15;
		        const int NID_STATE = 16;
		        const int NID_ORGANIZATION = 17;
		        const int NID_ORGANIZATIONAL_UNIT = 18;
		  
   		        const int NID_EMAIL_ADDRESS = 48;

		        const int NID_GIVEN_NAME = 99;
		        const int NID_SURNAME = 100;
		        const int NID_INITIALS = 101;
		        const int NID_SERIAL_NUMBER = 105;
		        const int NID_TITLE = 106;
		        const int NID_DESCRIPTION = 107;

		        const int NID_STREET_ADDRESS = 660;
		  

		        bool partialMatch(const X509_NAME* name, const std::vector<std::shared_ptr<CertMatchType>> &certFilter);
		        bool exactMatch(const X509_NAME* name, const std::vector<std::shared_ptr<CertMatchType>> &certFilter);
		  
		        WebSocketServerSsl::WebSocketServerSsl(std::map<std::string, std::string> &parms) :
			m_ctx(boost::asio::ssl::context::sslv23)
			{

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
					    throw std::invalid_argument(std::string("WebSocket Server: invalid sll verifier flag: ") + input.substr(posBegin,posEnd-posBegin));
					    break;
					  }
				      }

				    m_ctx.set_verify_mode(m_verifyMode);

				  }

				if (parms.find("certFilterMode") == parms.end())
				  m_certFilterMode = CertFilterMode::noFiltering;
				else if (parms.find("certFilterMode") != parms.end() && parms["certFilterMode"] == "")
				  m_certFilterMode = CertFilterMode::noFiltering;
				else if (parms["certFilterMode"] == "exactMatch")
				  m_certFilterMode = CertFilterMode::exactMatch;
				else if (parms["certFilterMode"] == "partialMatch")
				  m_certFilterMode = CertFilterMode::partialMatch;
				else
				  throw std::invalid_argument("WebSocketServer: invalid certFilterMode: " + parms["certFilterMode"] +
							      ". Supported values: empty/undefined, 'exactMatch', 'partialMatch'.");
				  
				// If trustStore provided, load it.
				if (parms.find("trustStore") != parms.end() && parms["trustStore"] != "")
				  m_ctx.load_verify_file(parms["trustStore"]);
				else
				  {
				    if (m_verifyMode & boost::asio::ssl::verify_peer ||
					m_verifyMode & boost::asio::ssl::verify_fail_if_no_peer_cert ||
					m_verifyMode & boost::asio::ssl::verify_client_once)
					throw std::invalid_argument("WebSocket Server: no trustStore defined, but one of VerifyPeer, VerifyFailIfNoPeerCert or VerifyClientOnce was specified");
				  }
				  
			} // End of constructor

			WebSocketServerSsl::~WebSocketServerSsl()
			{
				DELETEOBJ(m_io_context);
			} // End of destructor

		         void WebSocketServerSsl::open_new_cert_match()
			 {
			   m_certFilter.push_back(std::make_shared<CertMatchType>());
			 } // End of open_new_cert_match
		  
		         void WebSocketServerSsl::add_cert_match(std::string nidStr, std::string str)
			 {

			   std::cout << "Adding cert match: " << nidStr << ": " << str << std::endl;
			   int nid;

			   if (nidStr == "commonName")
			     nid = NID_COMMON_NAME;
			   else if (nidStr == "country")
			     nid = NID_COUNTRY;
			   else if (nidStr == "locality")
			     nid = NID_LOCALITY;
			   else if (nidStr == "state")
			     nid = NID_STATE;
			   else if (nidStr == "organization")
			     nid = NID_ORGANIZATION;
			   else if (nidStr == "organizationalUnit")
			     nid = NID_ORGANIZATIONAL_UNIT;
			   else if (nidStr == "emailAddress")
			     nid = NID_EMAIL_ADDRESS;
			   else if (nidStr == "givenName")
			     nid = NID_GIVEN_NAME;
			   else if (nidStr == "surname")
			     nid = NID_SURNAME;
			   else if (nidStr == "initials")
			     nid = NID_INITIALS;
			   else if (nidStr == "serialNumber")
			     nid = NID_SERIAL_NUMBER;
			   else if (nidStr == "title")
			     nid = NID_TITLE;
			   else if (nidStr == "description")
			     nid = NID_DESCRIPTION;
			   else if (nidStr == "streetAddress")
			     nid = NID_STREET_ADDRESS;
			   else
			     {
			       throw std::invalid_argument("Unsupported certificate attribute: " + nidStr);
			     }
			     
			   m_certFilter.back() -> push_back(std::make_tuple(nid, str));
			 } // End of add_cert_match
		  
			
			void WebSocketServerSsl::listen(unsigned short port_)
			{
				auto const address = boost::asio::ip::make_address("127.0.0.1");
				m_port = port_;
																				
				// The io_context is required for all I/O
				auto const threads = std::max<int>(1, std::atoi("1"));
				m_io_context = RJNEW boost::asio::io_context{ threads };
				m_listener = std::make_shared<WebSocketServerSslListener>(*m_io_context, std::ref(m_ctx), boost::asio::ip::tcp::endpoint(address, m_port), &m_sessions, this);

				m_listener -> set_on_accept_callback(m_serverAcceptEvent);
				m_listener -> set_on_receive_callback(m_serverReceiveEvent);
				
				m_listener -> run();

				/*
				WebServerThread* thread = RJNEW WebServerThread(m_io_context);
				thread->Run();
				*/

				// Just one thread now.
				for  (int i=0; i<threads; i++)
				  {
				    std::cout << "----> Thread: " << i << std::endl;
				    std::thread th{[this]
					{
					  /// @fixme If we throw and exception in any of the handlers, it will come out of m_io_context -> run()
					  try
					    {
					      m_io_context -> run();
					    }
					  catch (std::exception &e)
					    {
					      /// @fixme Exception handler here can propagate the error to JavaScript
					      std::cout << "======================= CAUGHT: " << e.what() << std::endl;
					    }
					}
				    };
				    m_IocThreads.push_back(std::move(th));
				  }
				
				//#ifdef GUI_USE_WXWIDGETS
				//				m_isAlive = thread->IsAlive();
				//#else
				m_isAlive = true;
				//#endif
			} // End of listen

			void WebSocketServerSsl::send(String id_, String message_)
			{
				for (const auto& s : m_sessions)
					if (s.m_session_id == id_)
					{
						s.m_session->do_write(message_);
						break;
					}
			} // End of send


		        void WebSocketServerSsl::send(String id_, const void* message_, int msg_len)
			{
				for (const auto& s : m_sessions)
					if (s.m_session_id == id_)
					{
					        s.m_session->do_write(message_, msg_len);
						break;
					}
			} // End of send
		  
			void WebSocketServerSsl::sendToAll(String message_)
			{
				for(const auto& s: m_sessions)
					s.m_session->do_write(message_);
			} // End of sendToAll

			String WebSocketServerSsl::receive()
			{
				if (m_sessions.size() > 0)
				{
				  std::string message = m_sessions[0].m_last_message;
				  m_sessions[0].m_last_message = "";

					return message;
				}
					

				return String("");
			} // End of receive

			void WebSocketServerSsl::close()
			{
				m_io_context->stop();
			} // End of close

			void WebSocketServerSsl::set_on_accept_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverAcceptEvent = callback;
			} // End of set_on_accept_callback

			void WebSocketServerSsl::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			} // End of set_on_receive_callback
		  
		  
		        WebSocketServerSsl::WebSocketServerSslSession::WebSocketServerSslSession(boost::asio::ip::tcp::socket socket_, 
												 boost::asio::ssl::context &context_,
												 std::string sessionID_,
												 std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *sessions_,
												 WebSocketServerSsl *parent
												 )
			  : m_ws(std::move(socket_), context_), m_strand(m_ws.get_executor()), m_sessionID(sessionID_), m_sessions(sessions_), m_parent(parent)
			{

			  m_ws.next_layer().set_verify_callback([this](bool preVerified, boost::asio::ssl::verify_context& vctx) -> bool {
			      return verify_cert(preVerified, vctx);
			    });


			} // End of WebSocketServerSslSession constructor

		  
		        bool WebSocketServerSsl::WebSocketServerSslSession::verify_cert(bool preVerified,
											boost::asio::ssl::verify_context &vctx)
			{

			  X509* cert = X509_STORE_CTX_get_current_cert(vctx.native_handle());
			  

			  int isCa = X509_check_ca(cert);
			  std::cout << "CA status: " << isCa << std::endl << std::flush;

			  X509_NAME *name = X509_get_subject_name(cert);
			  
			  bool certVerificationResult = true;



			  if (!isCa)
			    {
			      
			      std::cout << "OKAY, might be doing more verification" << std::endl;
			      
			      //certVerificationResult = false;
				  
			      switch (m_parent -> m_certFilterMode)
				{
				case CertFilterMode::noFiltering:
				  std::cout << "########## noFiltering" << std::endl;
				  certVerificationResult = true;
				  break;

				case CertFilterMode::partialMatch:
				  std::cout << "########## partialMatch" << std::endl;
				  certVerificationResult = partialMatch(name, m_parent -> m_certFilter);
				  break;

				case CertFilterMode::exactMatch:
				  std::cout << "########## exactMatch" << std::endl;
				  certVerificationResult = exactMatch(name, m_parent -> m_certFilter);
				  break;				  
				}

			    }


			  
			  return certVerificationResult;
			} // End of verify_cert
			  
			void WebSocketServerSsl::WebSocketServerSslSession::run()
			{
			  m_ws.next_layer().async_handshake(
							    boost::asio::ssl::stream_base::server,
							    boost::asio::bind_executor(
										       m_strand,
										       std::bind(
												 &WebSocketServerSslSession::on_handshake,
												 shared_from_this(),
												 std::placeholders::_1)));
			} // End of run

		        void WebSocketServerSsl::WebSocketServerSslSession::on_handshake (boost::system::error_code ec)
			{

				if (ec)
				{

				  /// @fixme This doesn't work, we need to throw a regular exception, catch it in io_service thread, then propagate to JavaScript.
				  RadJav::throwException(ec.message());

					return;
				  
				}

				m_ws.async_accept(
					boost::asio::bind_executor(
						m_strand,
						std::bind(
							&WebSocketServerSslSession::on_accept,
							shared_from_this(),
							std::placeholders::_1)));

			} // End of on_handshake
		  
		        void WebSocketServerSsl::WebSocketServerSslSession::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			} // End of set_on_receive_callback
		  
			void WebSocketServerSsl::WebSocketServerSslSession::on_accept(boost::system::error_code ec)
			{
				if (ec)
				{
				        /// @fixme This doesn't work, we need to throw a regular exception, catch it in io_service thread, then propagate to JavaScript.
					RadJav::throwException(ec.message());

					return;
				}

				// Read a message
				do_read();
			} // End of on_accept

			void WebSocketServerSsl::WebSocketServerSslSession::do_read()
			{
			  
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
			} // End of do_read

			void WebSocketServerSsl::WebSocketServerSslSession::do_write(String message_)
			{
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
			} // End of do_write


		        void WebSocketServerSsl::WebSocketServerSslSession::do_write(const void *message_, int msg_len)
			{
			  //TODO: will have to copy the message
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
			} // End of do_write

		        v8::Local<v8::Function> WebSocketServerSsl::WebSocketServerSslSession::get_on_receive_callback()
			{
			  return m_serverReceiveEvent->Get(V8_JAVASCRIPT_ENGINE->isolate);
			} // End of get_on_receive_callback
		  
			void WebSocketServerSsl::WebSocketServerSslSession::on_read(
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
				        /// @fixme This doesn't work, we need to throw a regular exception, catch it in io_service thread, then propagate to JavaScript.
					RadJav::throwException(ec.message());

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

				if (ec)
				{
				        /// @fixme This doesn't work, we need to throw a regular exception, catch it in io_service thread, then propagate to JavaScript.
					RadJav::throwException(ec.message());

					return;
				}

			} // End of on_read

			WebSocketServerSsl::WebSocketServerSslListener::WebSocketServerSslListener(
				boost::asio::io_context& ioc_,
				std::reference_wrapper<boost::asio::ssl::context> context_,
				boost::asio::ip::tcp::endpoint endpoint_,
				std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *sessions_,
				WebSocketServerSsl *parent
				
								  )
			  : m_acceptor(ioc_), m_ctx(context_), m_socket(ioc_), m_sessions(sessions_), m_parent(parent)
			    
			{
			  
				boost::system::error_code ec;

				// Open the acceptor
				m_acceptor.open(endpoint_.protocol(), ec);
				if (ec)
				{
					RadJav::throwException(ec.message());

					return;
				}

				// Allow address reuse
				m_acceptor.set_option(boost::asio::socket_base::reuse_address(true));
				if (ec)
				{
					RadJav::throwException(ec.message());

					return;
				}

				// Bind to the server address
				m_acceptor.bind(endpoint_, ec);
				if (ec)
				{
					RadJav::throwException(ec.message());

					return;
				}

				// Start listening for connections
				m_acceptor.listen(
					boost::asio::socket_base::max_listen_connections, ec);
				if (ec)
				{
					RadJav::throwException(ec.message());

					return;
				}
			} // End of WebSocketServerSslListener
		  
		        void WebSocketServerSsl::WebSocketServerSslListener::set_on_accept_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverAcceptEvent = callback;
			} // End of set_on_accept_callback

		        void WebSocketServerSsl::WebSocketServerSslListener::set_on_receive_callback(v8::Persistent<v8::Function>* callback)
			{
			  m_serverReceiveEvent = callback;
			} // End of set_on_receive_callback
		  
			// Start accepting incoming connections
			void WebSocketServerSsl::WebSocketServerSslListener::run()
			{
				if (!m_acceptor.is_open())
					return;

				do_accept();
			} // End of run

			void WebSocketServerSsl::WebSocketServerSslListener::do_accept()
			{
			  
				m_acceptor.async_accept(
					m_socket,
					std::bind(
						&WebSocketServerSslListener::on_accept,
						shared_from_this(),
						std::placeholders::_1));
			} // End of do_accept

		        v8::Local<v8::Function> WebSocketServerSsl::WebSocketServerSslListener::get_on_accept_callback()
			{

			  return v8::Local<v8::Function>::Cast(RadJAV::CPP::Net::WebSocketServerSsl::WebSocketServerSslListener::m_serverAcceptEvent->Get(V8_JAVASCRIPT_ENGINE->isolate));
			} // End of get_on_accept_callback
		        v8::Persistent<v8::Function> *WebSocketServerSsl::WebSocketServerSslListener::get_on_receive_persistent_evt()
			{

			  return m_serverReceiveEvent;
			} // get_on_receive_callback
		  

			void WebSocketServerSsl::WebSocketServerSslListener::on_accept(boost::system::error_code ec)
			{
				if (ec)
				{
					RadJav::throwException(ec.message());

					return;
				}
				else
				{
					// Create the session and run it
					session_data this_session;

					std::string sessionId = boost::uuids::to_string(boost::uuids::random_generator()());
					auto session = std::make_shared<WebSocketServerSslSession>(std::move(m_socket), m_ctx, sessionId, m_sessions, m_parent);
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
			} // End of on_accept
		  


		        bool partialMatch(const X509_NAME* name, const CertMatchType &certMatch)
			{
			  bool match = false;
			  for (int i = 0; i < X509_NAME_entry_count(name); i++)
			    {
			      X509_NAME_ENTRY *nameEntry = X509_NAME_get_entry(name, i);
			      ASN1_STRING *nameStr = X509_NAME_ENTRY_get_data(nameEntry);
			      //const unsigned char *str = ASN1_STRING_get0_data(nameStr);
			      const char *str = reinterpret_cast<const char*>(ASN1_STRING_get0_data(nameStr));

			      ASN1_OBJECT *asn1Obj = X509_NAME_ENTRY_get_object(nameEntry);

			      int attrNid = OBJ_obj2nid(asn1Obj);
			      int attrLen = ASN1_STRING_length(nameStr);

			      for (auto matchAttr : certMatch)
				{
				  std::cout << "Checking:" << std::get<1>(matchAttr) << std::endl;
	  
				  if (attrNid == std::get<0>(matchAttr))
				    {

				      std::cout << "NID match: " << attrNid << std::endl;
				      if (attrLen == std::get<1>(matchAttr).size() &&
					  !std::strncmp(str, std::get<1>(matchAttr).c_str(), attrLen))
					{
					  match = true;
					  std::cout << "Checking the match: " << std::get<0>(matchAttr) << ':' << std::get<1>(matchAttr)
						    << " -> " << match<< std::endl;
		
					}
				      else
					{
					  match = false;
					  std::cout << "Checking the match: " << std::get<0>(matchAttr) << ':' << std::get<1>(matchAttr)
						    << " -> " << match<< std::endl;
					  return match;
					}
				    }

				}
      

			      std::cout << "N: " << ASN1_STRING_type(nameStr) << ": " << OBJ_obj2nid(asn1Obj) << ": ";
			      for (int i=0; i<ASN1_STRING_length(nameStr); i++)
				std::cout << str[i];
			      std::cout << std::endl;
			    }

			  return match;
			} // End of partialMatch
		  
  		        bool exactMatch(const X509_NAME *name, const CertMatchType &certMatch)
			{

			  // If elements counts are different, obviously, we have no match.

			  int certAttrCount = X509_NAME_entry_count(name);
			  if (certAttrCount != certMatch.size())
			    return false;

			  // Since a certificate can have more than one organizational unit, we will tackle these separately, after parsing through other elements.
			  std::vector <std::tuple<const char*, int>> certOu;
			  std::vector <const std::string*> matchOu;
  
			  bool match = false;
			  for (int i = 0; i < certAttrCount; i++)
			    {
			      X509_NAME_ENTRY *nameEntry = X509_NAME_get_entry(name, i);
			      ASN1_STRING *nameStr = X509_NAME_ENTRY_get_data(nameEntry);
			      //const unsigned char *str = ASN1_STRING_get0_data(nameStr);
			      const char *str = reinterpret_cast<const char*>(ASN1_STRING_get0_data(nameStr));

			      ASN1_OBJECT *asn1Obj = X509_NAME_ENTRY_get_object(nameEntry);

			      int attrNid = OBJ_obj2nid(asn1Obj);
			      int attrLen = ASN1_STRING_length(nameStr);

			      for (auto& matchAttr : certMatch)
				{
	  
				  std::cout << "Checking:" << std::get<1>(matchAttr) << ", Addr: "  << " -> " << &std::get<1>(matchAttr) << std::endl;
	  
				  if (attrNid == std::get<0>(matchAttr))
				    {
				      std::cout << "NID match: " << attrNid << std::endl;
	      
				      if (attrNid == NID_ORGANIZATIONAL_UNIT)
					{
					  certOu.push_back(std::make_tuple(str, attrLen));
					  matchOu.push_back(&std::get<1>(matchAttr));
					  std::cout << "PUSHING cert, match attributes: ";
					  std::cout.write(str, attrLen);
					  std::cout << std::get<1>(matchAttr)
						    << " -> " << &std::get<1>(matchAttr) << std::endl;
					}
				      else
					if (attrLen == std::get<1>(matchAttr).size() &&
					    !std::strncmp(str, std::get<1>(matchAttr).c_str(), attrLen))
					  {
					    match = true;
					    std::cout << "Checking the match: " << std::get<0>(matchAttr) << ':' << std::get<1>(matchAttr)
						      << " -> " << match<< std::endl;
					  }
					else
					  {
					    match = false;
					    std::cout << "Checking the match: " << std::get<0>(matchAttr) << ':' << std::get<1>(matchAttr)
						      << " -> " << match<< std::endl;
					    return match;
					  }
				    }

				}


			      std::cout << "N: " << ASN1_STRING_type(nameStr) << ": " << OBJ_obj2nid(asn1Obj) << ": ";
			      for (int i=0; i<ASN1_STRING_length(nameStr); i++)
				std::cout << str[i];
			      std::cout << std::endl;
			    }

			  std::cout << "PHASE 2: " << certOu.size() << ", " << matchOu.size() << std::endl;

			  for (auto certOuAttr = certOu.begin(); certOuAttr != certOu.end(); )
			    {
			      int certOuSize = certOu.size();
			      for (auto matchOuAttr = matchOu.begin(); matchOuAttr != matchOu.end(); )
				{
				  std::cout << "Comparing OU: ";
				  std::cout.write(std::get<0>(*certOuAttr), std::get<1>(*certOuAttr));
				  std::cout << " <-> " << **matchOuAttr << std::endl;

				  // Comparison.
				  if (std::get<1>(*certOuAttr) == (*matchOuAttr) -> size() &&
				      !std::strncmp(std::get<0>(*certOuAttr), (*matchOuAttr) -> c_str(), std::get<1>(*certOuAttr)))
				    {
				      //std::cout << "Erasing: " << *certOuAttr << ", " << *matchOuAttr << std::endl;
				      certOuAttr = certOu.erase(certOuAttr);
				      matchOuAttr = matchOu.erase(matchOuAttr);
				    }
				  else
				    {
				      ++matchOuAttr;
				      if (matchOuAttr != matchOu.end())
					std::cout << "will try matchOuAttr = " << **matchOuAttr << std::endl;
				      else 
					std::cout << "matchOu loop done" << std::endl;
	      
				    }
				}
			      if (matchOu.size() && certOu.size() == certOuSize && certOuAttr != certOu.end() )
				++certOuAttr;

			      std::cout << "Still here" << std::endl;
			    }

			  if (certOu.size() || matchOu.size())
			    match = false;
			  else
			    match = true;

			  return match;
			} // End of exactmatch

		        bool partialMatch(const X509_NAME* name, const std::vector<std::shared_ptr<CertMatchType>> &certFilter)
			{

			  // If any of the patterns matches, return success, otherwise return failure (false)
			  for (auto certMatch : certFilter)
			    {
			      if (partialMatch(name, *certMatch))
				return true;
			    }
			  return false;
			} // End of partialMatch
		  
		        bool exactMatch(const X509_NAME* name, const std::vector<std::shared_ptr<CertMatchType>> &certFilter)
			{
			  // If any of the patterns matches, return success, otherwise return failure (false)
			  for (auto certMatch : certFilter)
			    {
			      if (exactMatch(name, *certMatch.get()))
				return true;
			    }
			  return false;
			} // End of exactMatch
		  
		} // namespace Net
	} // namespace CPP
} // namespace RadJav

