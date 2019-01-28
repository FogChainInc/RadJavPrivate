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

        #include <vector>
        #include <list>
        #include <tuple>

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
			        using CertAttrType  = std::tuple<int, std::string>;
				using CertMatchType = std::list<CertAttrType>;

				/**
				 * @class WebSocketServerSsl
				 * 
				 * @brief WebSocketServer with SSL layer.
				 */
				class RADJAV_EXPORT WebSocketServerSsl : public ChainedPtr
				{
				public:
				  class WebSocketServerSslSession;
				private:
				                /**
						 * @brief used to specify the certificate filtering mode.
						 *  - noFiltering means no additional verification (so as long as the certificate was signed with allowed root cert, it will 
						 *    be allowed to connect.
						 *  - partialMatch - The server will allow connection as long as the certificate has the attributes in the match list.
						 *  - exactMatch - In this mode, all attributes of a certificate must be provided to allow the connection.
						 */
				                 enum class CertFilterMode : int { noFiltering, partialMatch, exactMatch };

						/**
						 * @brief session data, including ID, session and last message to dispatch
						 */
						struct session_data
						{
							std::string m_session_id;
							std::shared_ptr <WebSocketServerSslSession> m_session;
							std::string m_last_message;

						};
				  
					public:
						/**
						 * @brief Constructs the object
						 */						 
						WebSocketServerSsl(std::map<std::string, std::string> &parms);
						
						/**
						 * @brief Destroys the object
						 */
						~WebSocketServerSsl();


						/**
						 * @brief Start listening
						 * 
						 * @param port_ The port to listen on.
						 */
						void listen(unsigned short port_ = 9229);

						/**
						 * @brief send a string message to a client.
						 * 
						 * @param id Session ID.
						 * @param message Message to send.
						 */
						void send(String id, String message);
						/**
						 * @brief send a binary message to a client.
						 * 
						 * @param id Session ID.
						 * @param message Message to send.
						 * @param message Message size.
						 */
						void send(String id, const void *message, int msg_len);

						/**
						 * @brief send a string message to all clients.
						 * 
						 * @param message Message to send.
						 */
						void sendToAll(String message);

						/**
						 * @brief Displatch message from client.
						 */
						String receive();

						/**
						 * @brief Close the server.
						 */
						void close();

						/**
						 * @brief set onAccept callback for this object.
						 *
						 * @param callback A Persistent function to be called on event.
						 */
						void set_on_accept_callback(v8::Persistent<v8::Function>*);
						/**
						 * @brief set onReceive callback for this object.
						 *
						 * @param callback A Persistent function to be called on event.
						 */
						void set_on_receive_callback(v8::Persistent<v8::Function>*);

						/**
						 * @brief Create a new certificate match set.
						 *
						 * This method is called from the V8 binder, more specifically, from the V8B::net::WebSocketServerSsl::_init method.
						 * After the instance of this object was created. The V8 _init will parse the certFilter parameter, which contains an array
						 * of hashes. Each hash contains a set of key <-> value pairs, where keys are allowed certificate attributes
						 * and values are strings.
						 * 
						 * For example: [{commonName: "MyThisAndThatServer", organizationalUnit: "Accounting"}, 
						 *               {commonName: "SomeOtherEntity", organizationalUnit: "Enginnering"}]
						 * 
						 * The _init method will execute open_new_cert_match() for each element of above array.
						 *
						 * Note, there are two modes for certificate matcihng:
						 *  - partialMatch
						 *  - exactMatch
						 *
						 * The partial match will work with an example like above. Any certificate that matches the listed commonName/organizationalUnit
						 * will satisfy the certificate verification. For exactMatch, all attributes of an certificate must be correctly listed, to allow
						 * that client to connect. 
						 */
						void open_new_cert_match();
						/**
						 * @brief Add and attribute to certificate match set.
						 *
						 * @param nid A string representing a well known ID of the attribute.
						 * @param str A string value of the attribute.
						 *
						 * This method is called from the V8 binder, after the instance of this object was created. The V8 binder 
						 * parses the array of parameters passed from JavaScript, and calls this method for each attribute found in a match set.
						 * Supported attributes: Refer to implementation of this function in a cpp file. Most frequently used 
						 * attributes:
						 *  - commonName
						 *  - organization
						 *  - organizationalUnit
						 *  - country
						 *  - locality
						 *  - serialNumber
						 *
						 * Note, a x509 certificate supports one of each attribute with one exception of organizationalUnit. More 
						 * than one organizationalUnit attributes are allowed. For example, a person could be certified to be 
						 * a part of Enginnering Group and management.
						 */
						void add_cert_match(std::string nid, std::string str);

						/**
						 * @class WebSocketServerSession
						 *
						 * @brief Session object
						 */
						class RADJAV_EXPORT WebSocketServerSslSession : public std::enable_shared_from_this<WebSocketServerSslSession>
						{
						  boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> m_ws;
							boost::asio::strand<boost::asio::io_context::executor_type> m_strand;
							boost::beast::flat_buffer m_readBuffer;

							public:

							/**
							 * @brief Constructs the object.
							 */
							WebSocketServerSslSession(boost::asio::ip::tcp::socket socket_,
										  boost::asio::ssl::context &context_,
										  std::string sessionID_,
										  std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *sessions_,
										  WebSocketServerSsl *parent
										  );

							        /**
								 * @brief verify_cert Called from Beast to perform additional certificate verification.
								 *
								 * @param preVerified Passed from the previous verification operation.
								 * @param vctx An opaque verify context.
								 */
							        bool verify_cert(bool preVerified, boost::asio::ssl::verify_context &vctx);

							

								/**
								 * @brief starts asynchronous operation.
								 */
								void run();

								/**
								 * @brief Called when handshake is finished.
								 *
								 * @param ec_ Error code.
								 */
								void on_handshake (boost::system::error_code ec_);
								/**
								 * @brief Called when a connection was accepted
								 *
								 * @param ec_ Error code.
								 */
								void on_accept(boost::system::error_code ec_);

								/**
								 * @brief Read the message
								 */
								void do_read();

								/**
								 * @brief Write message out
								 *
								 * @param message_ A string content
								 */
								void do_write(String message_);
								/**
								 * @brief Write message out
								 *
								 * @param message_ Binary data.
								 * @param msg_len Data size.
								 */
								void do_write(const void *message_, int msg_len);

								/**
								 * @brief Called after session received data from client.
								 *
								 * @param ec_ Error code.
								 * @param bytes_transferred_ Number of bytes received.
								 *
								 * This method executes a JavaScript callback if one is set. Received data is passed to the callback.
								 */
								void on_read(
									boost::system::error_code ec_,
									std::size_t bytes_transferred_);

								/**
								 * @brief Called after data was sent out.
								 *
								 * @param ec_ Error code.
								 * @param bytes_transferred_ Number of bytes received.
								 */
								void on_write(
									boost::system::error_code ec_,
									std::size_t bytes_transferred_);

								/**
								 * @brief Sets the onReceive callback for the session.
								 *
								 * @param callback Persistent JavaScript object.
								 */
								void set_on_receive_callback(v8::Persistent<v8::Function> *callback);

							private:
								/**
								 * @brief Gets the localized JavaScript callback object.
								 */
								v8::Local<v8::Function> get_on_receive_callback();

						                #ifdef USE_V8
								/**
								 * @brief Persistent onReceive event.
								 */
								v8::Persistent<v8::Function> *m_serverReceiveEvent;
						                #endif
								
								
								/**
								 * @brief client session ID
								 */
								std::string m_sessionID;
								/**
								 * @brief pointer to currently dispatching message
								 */
								std::shared_ptr<std::string> m_activeMessage = nullptr;
								/**
								 * @brief Session holder.
								 */
								std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *m_sessions;

								WebSocketServerSsl *m_parent;
						};

						/**
						* @class WebSocketServerListener
						*
						*/
						class RADJAV_EXPORT WebSocketServerSslListener : public std::enable_shared_from_this<WebSocketServerSslListener>
						{
						        /**
							 *  @brief TCP acceptor object.
							 */
							boost::asio::ip::tcp::acceptor m_acceptor;
						        /**
							 * @brief TCP socket.
							 */
							boost::asio::ip::tcp::socket m_socket;
						        /**
							 *  @brief TCP context object
							 */
							std::reference_wrapper<boost::asio::ssl::context> m_ctx;

						        /**
							 *  @brief Reference to the parent object.
							 * 
							 * THis reference is used to access various parameters, set earlier during object contruction from JavaScript.
							 */
							WebSocketServerSsl *m_parent;
							
							//boost::asio::ssl::context m_ctx;
							

							/**
							 * @brief Session holder.
							 */
							std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *m_sessions;
							

							public:
							        /**
								 * @brief Constructs the object
								 *
								 * @param ioc_ ASIO context.
								 * @param context_ SSL context (needed for SSL handshake)
								 * @param endpoint_ Tells the listener which interface/port to listen on.
								 * @param sessions_ The structure storing all current session data.
								 * @param parent_ The reference to the WebSocketServerSsl object, used to access some parameters.
								 */
								WebSocketServerSslListener(
									boost::asio::io_context& ioc_,
									std::reference_wrapper<boost::asio::ssl::context> context_,
									boost::asio::ip::tcp::endpoint endpoint_,
									std::vector <RadJAV::CPP::Net::WebSocketServerSsl::session_data> *sessions_,
									WebSocketServerSsl *parent_
											   );

								/**
								 * @brief Start accepting incoming connections
								 */
								void run();

								/**
								 * @brief Initiate asynchronous operation.
								 */
								void do_accept();

								/**
								 * @brief Callback called from Beast.
								 *
								 * The method creates and starts another session.
								 *
								 * @param ec_ Error Code
								 */
								void on_accept(boost::system::error_code ec_);

								/**
								 * @brief Sets the onReceive callback for the session.
								 *
								 * @param callback Persistent JavaScript object.
								 */
								void set_on_accept_callback(v8::Persistent<v8::Function>*);	  
								/**
								 * @brief Sets the onAccept callback for the session.
								 *
								 * @param callback Persistent JavaScript object.
								 */
								void set_on_receive_callback(v8::Persistent<v8::Function>*);	  


						#ifdef USE_V8
								/**
								 * @brief pointer to AcceptEvent persistent function of V8 engine
								 */
								v8::Persistent<v8::Function> *m_serverAcceptEvent;
								/**
								 * @brief pointer to ReceiveEvent persistent function of V8 engine
								 */
								v8::Persistent<v8::Function> *m_serverReceiveEvent;
						#endif
						         private:
						                /**
								 * @brief obtains localized callback object.
								 */
								v8::Local<v8::Function> get_on_accept_callback();
						                /**
								 * @brief obtains persistent onReceive callback object.
								 */
								v8::Persistent<v8::Function> *get_on_receive_persistent_evt();
								

								
						};

						#ifdef USE_V8
						/**
						 * @brief Persistent JavaScript callback object
						 */
						v8::Persistent<v8::Function> *m_serverAcceptEvent;
						/**
						 * @brief Persistent JavaScript callback object
						 */
						v8::Persistent<v8::Function> *m_serverReceiveEvent;
						#endif

					private:

						
						/**
						 * @brief Port the server listens on.
						 */
						unsigned short m_port;

						/**
						 * @brief ASIO context
						 */
						boost::asio::io_context *m_io_context;
						/**
						 * @brief ASIO SSL context
						 */
						boost::asio::ssl::context m_ctx;

						/**
						 * @brief Flag that indicates if listening context available 
						 */
						RJBOOL m_isAlive;

						/**
						 * @brief Handle to the listener object.
						 */
						std::shared_ptr<WebSocketServerSslListener> m_listener;
						
						/**
						 * @brief Maintains the list of sessions.
						 */
						std::vector <session_data> m_sessions;


						/**
						 * @brief ASIO SLL verify mode controlling how client certificate verification is done.
						 */
						boost::asio::ssl::verify_mode m_verifyMode;

						/**
						 * @brief Certificate filtering mode.
						 * 
						 * Dictates how additional certificate verification is performed
						 */
						CertFilterMode m_certFilterMode;

						/**
						 * @brief Contains data from JavaScript, representing certificate match patterns.
						 */
						std::vector<std::shared_ptr<CertMatchType>> m_certFilter;

						/**
						 * @brief Stores ASIO context delivery threads.
						 */
						std::vector<std::thread> m_IocThreads;
						
						  
				};
			}
		}
	}
#endif

