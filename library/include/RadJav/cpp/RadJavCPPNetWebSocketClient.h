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
#ifndef _RADJAV_CPP_NET_WEBSOCKETCLIENT_H_
#define _RADJAV_CPP_NET_WEBSOCKETCLIENT_H_

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

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			class RADJAV_EXPORT WebSocketClient
			{
				public:
					WebSocketClient(String host, String port);

					void connect();

					void send(String message);

					String receive();

					void close();

				private:
					//the io_context is required for all I/O
					boost::asio::io_context ioc_;

					//these objects perform our I/O
					boost::asio::ip::tcp::resolver resolver{ ioc_ };
					boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws_{ ioc_ };

					String host_;
					String port_ = "80";
			};
		}
	}
}
#endif

