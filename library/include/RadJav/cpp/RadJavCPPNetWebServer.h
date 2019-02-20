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
#ifndef _RADJAV_CPP_NET_WEBSERVER_H_
	#define _RADJAV_CPP_NET_WEBSERVER_H_

	#include "RadJavPreprocessor.h"

	#include <boost/beast/core.hpp>
	#include <boost/beast/http.hpp>
	#include <boost/beast/version.hpp>
	#include <boost/asio.hpp>
	#include <boost/asio/bind_executor.hpp>
	#include <boost/asio/ip/tcp.hpp>
	#include <boost/asio/strand.hpp>
	#include <boost/config.hpp>
	//#include <boost/filesystem.hpp>

	#include <string>

	#include "RadJav.h"
	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "RadJavThread.h"
	#include "cpp/RadJavCPPChainedPtr.h"


	namespace ip = boost::asio::ip;
	using tcp = boost::asio::ip::tcp;
	namespace http = boost::beast::http;

	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				/**
				 * @ingroup group_net_cpp
				 * @brief WebServer class.
				 * @details Accepts incoming connections and launches the sessions.
				 */
				class RADJAV_EXPORT WebServer : public std::enable_shared_from_this<WebServer>, public ChainedPtr
				{
					public:
						WebServer();
						~WebServer();

						/// Listen for any incoming connections. Default 0 port will use port member value instead of wildcard
						void listen(RJINT port = 0);

						#ifdef USE_V8
							/// Serve any GET or POST requests.
							void serve(v8::Local<v8::Function> function);
							/// The V8 serve function.
							v8::Persistent<v8::Function> *servePersistent;
						#elif defined USE_JAVASCRIPTCORE
							/// Serve any GET or POST requests.
							void serve(JSObjectRef function);
							/// The V8 serve function.
							JSObjectRef servePersistent;
						#endif

						/// Stop the web server.
						void stop();

						/// The port.
						RJINT port;
						/// The server type.
						RJINT serverType;
						/// Flag that indicates if listening context available 
						RJBOOL isAlive;

					private:
						void run();
						void do_accept();
						void on_accept(boost::system::error_code ec);

						boost::asio::ip::address address;
						boost::asio::io_context ioc;
						tcp::acceptor acceptor;
						tcp::socket socket;

						SimpleThread *thread;
				};

				/**
				 * @ingroup group_net_cpp
				 * @brief WebServerTypes class.
				 * @details Web server types.
				 */
				enum RADJAV_EXPORT WebServerTypes
				{
					HTTP = 1, 
					HTTPS = 2
				};
			}
		}
	}
#endif

