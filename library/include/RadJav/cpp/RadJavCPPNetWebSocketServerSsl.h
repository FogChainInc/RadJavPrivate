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
#ifndef _RADJAV_CPP_NET_WEBSOCKETSERVER_SSL_H_
	#define _RADJAV_CPP_NET_WEBSOCKETSERVER_SSL_H_

	#include "RadJavPreprocessor.h"

	#include <boost/asio.hpp>
	#include <boost/beast/core.hpp>
	#include <boost/beast/websocket.hpp>
        #include <boost/beast/websocket/ssl.hpp>

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
				class RADJAV_EXPORT WebSocketServerSsl : public ChainedPtr
				{
				public:
				  class WebSocketServerSslSession;
				private:

						struct session_data
						{
							std::string m_session_id;
							std::shared_ptr <WebSocketServerSslSession> m_session;
							std::string m_last_message;

						};
				  
					public:
						WebSocketServerSsl(std::map<std::string, std::string> &parms);
						~WebSocketServerSsl();


						void listen(unsigned short port_ = 9229);

						void send(String id, String message);
						void send(String id, const void *message, int msg_len);

						void sendToAll(String message);

						String receive();

						void close();

						void set_on_accept_callback(v8::Persistent<v8::Function>*);
						void set_on_receive_callback(v8::Persistent<v8::Function>*);

						class RADJAV_EXPORT WebSocketServerSslSession : public std::enable_shared_from_this<WebSocketServerSslSession>
						{
						  boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> m_ws;
							boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
							boost::beast::flat_buffer m_readBuffer;

							public:
								// Take ownership of the socket
							WebSocketServerSslSession(boost::asio::ip::tcp::socket socket_,
										  boost::asio::ssl::context &context_,
										  std::string sessionID_,
									       std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *sessions_);

							        bool verify_cert(bool preVerified, boost::asio::ssl::verify_context &vctx);

							

								// Start the asynchronous operation
								void run();

								void on_handshake (boost::system::error_code ec_);
								void on_accept(boost::system::error_code ec_);

								void do_read();

								void do_write(String message_);
								void do_write(const void *message_, int msg_len);

								void on_read(
									boost::system::error_code ec_,
									std::size_t bytes_transferred_);

								void on_write(
									boost::system::error_code ec_,
									std::size_t bytes_transferred_);

								void set_on_receive_callback(v8::Persistent<v8::Function>*);

								

							private:
								v8::Local<v8::Function> get_on_receive_callback();

						                #ifdef USE_V8
								v8::Persistent<v8::Function> *m_serverReceiveEvent;
						                #endif
								
								
								std::string m_sessionID;
								std::shared_ptr<std::string> m_activeMessage = nullptr;
								std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *m_sessions;
						};

						class RADJAV_EXPORT WebSocketServerSslListener : public std::enable_shared_from_this<WebSocketServerSslListener>
						{
							boost::asio::ip::tcp::acceptor m_acceptor;
							boost::asio::ip::tcp::socket m_socket;
							std::reference_wrapper<boost::asio::ssl::context> m_ctx;
							//boost::asio::ssl::context m_ctx;
							

							std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *m_sessions;
							

							public:
								WebSocketServerSslListener(
									boost::asio::io_context& ioc_,
									std::reference_wrapper<boost::asio::ssl::context> context_,
									boost::asio::ip::tcp::endpoint endpoint_,
									std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *sessions_);

								// Start accepting incoming connections
								void run();

								void do_accept();

								void on_accept(boost::system::error_code ec_);

								void set_on_accept_callback(v8::Persistent<v8::Function>*);	  
								void set_on_receive_callback(v8::Persistent<v8::Function>*);	  


						#ifdef USE_V8
								v8::Persistent<v8::Function> *m_serverAcceptEvent;
								v8::Persistent<v8::Function> *m_serverReceiveEvent;
						#endif
						         private:
								v8::Local<v8::Function> get_on_accept_callback();
								v8::Persistent<v8::Function> *get_on_receive_persistent_evt();
								

								
						};

						#ifdef USE_V8
						v8::Persistent<v8::Function> *m_serverAcceptEvent;
						v8::Persistent<v8::Function> *m_serverReceiveEvent;
						#endif

					private:


						
						unsigned short m_port;

						boost::asio::io_context *m_io_context;
						boost::asio::ssl::context m_ctx;

						// Flag that indicates if listening context available 
						RJBOOL m_isAlive;

						std::shared_ptr<WebSocketServerSslListener> m_listener;
						
						//maintains the list of sessions
						std::vector <session_data> m_sessions;


						boost::asio::ssl::verify_mode m_verifyMode;
						
						  
				};
			}
		}
	}
#endif

