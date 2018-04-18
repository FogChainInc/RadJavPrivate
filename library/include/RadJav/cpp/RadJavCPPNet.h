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
#ifndef _RADJAV_CPP_NET_H_
#define _RADJAV_CPP_NET_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"

#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>


namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			class RADJAV_EXPORT HttpRequest
			{
				public:
					explicit HttpRequest(String url);

					void connect();

					void send(bool post);

					String receivedData();

					void close();

				private:
					//the io_context is required for all I/O
					boost::asio::io_context ioc;

					//these objects perform the I/O
					boost::asio::ip::tcp::resolver resolver{ ioc };
					boost::asio::ip::tcp::socket socket{ ioc };

					//target information
					String host;
					String port = "80";
					String target;
					int version;
			};
		}
	}
}
#endif

