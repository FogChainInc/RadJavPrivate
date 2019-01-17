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
#ifndef _RADJAV_CPP_NET_WEBSOCKETSERVER_H_
	#define _RADJAV_CPP_NET_WEBSOCKETSERVER_H_

	#include "RadJavPreprocessor.h"

	#include <boost/asio.hpp>
	#include <boost/beast/core.hpp>
	#include <boost/beast/websocket.hpp>
	#include <boost/asio/bind_executor.hpp>
	#include <boost/asio/strand.hpp>
	#include <boost/asio/ip/tcp.hpp>
	#include <boost/asio/io_service.hpp>

	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "cpp/RadJavCPPChainedPtr.h"
	#include "RadJavThread.h"

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				/**
				 * @class WebSocketServer
				 *
				 * @ingroup group_debug
				 *
				 * @brief Upgradable websocket server used by the v8 Inspector Agent
				 *
				 * This class is meant to be used only by the v8 Inspector Agent, its implementation
				 * is not complete and only serves as connection point for Chrome Dev Tools with
				 * minimal functionality to allow JS debugging. WebSocketServer waits for connection
				 * to default 9229 port and dispatches messages between V8 engine and connected
				 * instance of Chrome Dev Tools.
				 *
				 */
				class RADJAV_EXPORT WebSocketServer : public ChainedPtr
				{
				public:
				  class WebSocketServerSession;
				private:

						/**
						 * @brief session data, including ID, session and last message to dispatch
						 */
						struct session_data
						{
							std::string m_session_id;
							std::shared_ptr <WebSocketServerSession> m_session;
							std::string m_last_message;

						};
				  
					public:
						/**
						 * @brief Create websocket server instance
						 */
						WebSocketServer();
						~WebSocketServer();

						#ifdef USE_V8
						/**
						 * @brief V8 function to call when message handling event occurs
						 */
						static void on(String event_, v8::Local<v8::Function> func_);
						#elif defined USE_JAVASCRIPTCORE
						/**
						 * @brief JS Core function to call when message handling event occurs
						 */
						 static void on(String event_, JSObjectRef func_);
						#endif

						/**
						 * @brief start port listening
						 *
						 * @param port_ port number to listen, default = 9229 (Chrome Dev Tools)
						 *
						 */
						void listen(unsigned short port_ = 9229);

						/**
						 * @brief send message to specific client
						 *
						 * @param id string that contains UID of connected client
						 * @param message contents of a message
						 */
						void send(String id, String message);
						void send(String id, const void *message, int msg_len);

						/**
						 * @brief send message to all connected clients
						 *
						 * @param message contents of a message
						 */
						void sendToAll(String message);

						/**
						 * @brief dispatch message from client
						 */
						String receive();

						/**
						 * @brief close the WebSocketServer
						 */
						void close();

						/**
						 * @brief Set onAccept callback.
						 *
						 * @param callback A Persistent function to be called on event.
						 */
						void set_on_accept_callback(v8::Persistent<v8::Function> *callback);

						/**
						 * @brief Set onReceive callback.
						 *
						 * @param callback A Persistent function to be called on event.
						 */
						void set_on_receive_callback(v8::Persistent<v8::Function> *callback);						

						

						/**
						 * @brief message dispatching thread
						 */
						SimpleThread *thread;

						/**
						 * @class WebSocketServerSession
						 *
						 * @ingroup group_debug
						 *
						 * @brief Inner Websocket session class used by WebSocketServer to keep and dispatch
						 * one connection between external client and debug agent of JS engine
						 *
						 * This class is meant to be used only by the v8 Inspector Agent, its implementation
						 * is not complete and only serves as connection point for Chrome Dev Tools with
						 * minimal functionality to allow JS debugging.
						 *
						 */

						class RADJAV_EXPORT WebSocketServerSession : public std::enable_shared_from_this<WebSocketServerSession>
						{
							/**
							 * @brief boost tcp socket stream for incoming connections
							 *
							 * see the boost::beast::websocket::stream for details
							 */
							boost::beast::websocket::stream<boost::asio::ip::tcp::socket> m_ws;

							/**
							 * @brief boost strand for socket
							 *
							 * see the boost::asio::strand for details
							 */
							boost::asio::strand<boost::asio::io_context::executor_type> m_strand;

							/**
							 * @brief message buffer for socket connection
							 */
							boost::beast::flat_buffer m_readBuffer;

							public:
								// Take ownership of the socket
								/**
								 * @brief Take ownership of the socket
								 */							
							        WebSocketServerSession(boost::asio::ip::tcp::socket socket_, std::string sessionID_,
									       std::vector <RadJAV::CPP::Net::WebSocketServer::session_data> *sessions_);

								/**
								 * @brief Start asynchronous operation
								 */
								void run();

								/**
								 * @brief action on accept of incoming connection
								 *
								 * on_accept throws an error if error code is set, otherwise reads the message
								 *
								 * @param ec_ boost system error code
								 */
								void on_accept(boost::system::error_code ec_);

								/**
								 * @brief read the message to internal buffer
								 */
								void do_read();

								/**
								 * @brief write message to client
								 *
								 * @param message_ message string
								 */
								void do_write(String message_);
								void do_write(const void *message_, int msg_len);

								/**
								 * @brief on_read checks errors and writes incoming message from the
								 * client to internal buffer. If V8 engine is used, on_read calls stored
								 * V8 procedure for inspector agent with message as parameter
								 *
								 * @param ec_ error code if occurred
								 *
								 * @param bytes_transferred_ number of bytes transferred (parameter ignored)
								 */
								void on_read(
									boost::system::error_code ec_,
									std::size_t bytes_transferred_);

								/**
								 * @brief on_write checks errors, clears buffer and initiates new read attempt
								 *
								 * @param ec_ error code if occurred
								 *
								 * @param bytes_transferred_ number of bytes transferred (parameter ignored)
								 */
								void on_write(
									boost::system::error_code ec_,
									std::size_t bytes_transferred_);

								void set_on_receive_callback(v8::Persistent<v8::Function>*);

								

							private:
								/**
								 * @brief Obtains the onReceive callback.
								 *
								 * @returns Localized onReceive callback.
								 */
								v8::Local<v8::Function> get_on_receive_callback();

								/**
								 * @brief Persistent onReceiveEvent.
								 */
						                #ifdef USE_V8
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
								std::vector <RadJAV::CPP::Net::WebSocketServer::session_data> *m_sessions;
						};

						/**
						* @class WebSocketServerListener
						*
						* @ingroup group_debug
						*
						* @brief Inner Websocket session listener class used by WebSocketServer to dispatch
						* all connections between external clients and debug agent of JS engine
						*
						* This class is meant to be used only by the v8 Inspector Agent, its implementation
						* wraps around boost::asio::io_context to accept incoming connections and create websocket
						* sessions
						*
						*/
						class RADJAV_EXPORT WebSocketServerListener : public std::enable_shared_from_this<WebSocketServerListener>
						{
							/**
							 * @brief TCP acceptor class, see boost::asio::ip::tcp::acceptor for details
							 */
							boost::asio::ip::tcp::acceptor m_acceptor;

							/**
							 * @brief TCP socket class, see boost::asio::ip::tcp::socket for details
							 */
							boost::asio::ip::tcp::socket m_socket;

							std::vector <RadJAV::CPP::Net::WebSocketServer::session_data> *m_sessions;
							

							public:
								/**
								 * @brief Create websocket server listener
								 *
								 * @param ioc_ input-output context
								 *
								 * @param endpoint_ tcp endpoint (host+port) definition
								 */
								WebSocketServerListener(
									boost::asio::io_context& ioc_,
									boost::asio::ip::tcp::endpoint endpoint_,
									std::vector <RadJAV::CPP::Net::WebSocketServer::session_data> *sessions_);

								/**
								 * @brief Start accepting incoming connections
								 */
								void run();

								/**
								 * @brief call on_accept
								 */
								void do_accept();

								/**
								 * @brief create WebSocketServerSession and run it
								 *
								 * check error code and throw error if required, otherwise generate UID for session,
								 * create WebSocketServerSession and run it. If V8 JS engine is used and V8
								 * m_serverAcceptEvent callback pointer is not null, call it with arguments
								 */
								void on_accept(boost::system::error_code ec_);

								void set_on_accept_callback(v8::Persistent<v8::Function>*);	  
								void set_on_receive_callback(v8::Persistent<v8::Function>*);	  


						#ifdef USE_V8
						/**
						 * @brief pointer to AcceptEvent persistent function of V8 engine
						 */
	                                         v8::Persistent<v8::Function> *m_serverAcceptEvent;

						/**
						 * @brief pointer to ReceiveEvent persistent function of V8 engine.
						 */
						v8::Persistent<v8::Function> *m_serverReceiveEvent;

						#elif defined USE_JAVASCRIPTCORE

						/**
						 * @brief reference to AcceptEvent persistent function of JS Core engine
						 */
						JSObjectRef *m_serverAcceptEvent;

						/**
						 * @brief reference to ReceiveEvent persistent function of JS Core engine
						 */
						JSObjectRef *m_serverReceiveEvent;
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
						/**
						 * @brief Port to listen (9229)
						 */
						unsigned short m_port;

						/**
						 * @brief Input/Output context
						 */
						boost::asio::io_context *m_io_context;

						/**
						 * @brief Flag that indicates if listening context available
						 */
						RJBOOL m_isAlive;

						/**
						 * @brief Litener object.
						 */						
						std::shared_ptr<WebSocketServerListener> m_listener;

						/**
						 * List of sessions
						 */
						std::vector <session_data> m_sessions;
						  
				};
			}
		}
	}
#endif

