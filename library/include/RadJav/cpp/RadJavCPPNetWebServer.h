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

	#include "RadJavString.h"
	#include "RadJavHashMap.h"


	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
				class RADJAV_EXPORT WebServer
				{
					public:
						WebServer();
						~WebServer();

						/// Listen for any incoming connections.
						void listen();

						/// Serve any GET or POST requests.
						void serve();

						/// Stop the web server.
						void stop();

						/// The port.
						RJINT port;
						/// The server type.
						RJINT _serverType;

				private:
					boost::asio::ip::address address_;
					int threads_;
					std::vector<std::thread> v;
					boost::asio::io_context ioc;
					std::string doc_root_;
				};

				/// Web server types.
				enum RADJAV_EXPORT WebServerTypes
				{
					HTTP = 1, 
					HTTPS = 2
				};
			}
		}
	}
#endif

