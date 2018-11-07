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

#ifdef USE_CRYPTOGRAPHY
	#include <boost/asio/ssl.hpp>
	#include <boost/asio/ssl/error.hpp>
	#include <boost/asio/ssl/stream.hpp>

	#include "cpp/RadJavCPPCryptoBase.h"
#endif

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			/// A parsed URI.
			class RADJAV_EXPORT URI
			{
				public:
					URI()
					{
						protocol = "";
						host = "";
						port = 80;
						resource = "";
						query = "";
						target = "";
					}

					/// Parse a URL.
					static URI parse(const String &url);

					String protocol;
					String host;
					RJINT port;
					String resource;
					String query;
					String target;
			};

			/// Make HTTP Requests.
			class RADJAV_EXPORT HttpRequest
			{
				public:
					explicit HttpRequest(String url);

					void connect();
					void send();

					inline void setUserAgent(String userAgent)
					{
						this->userAgent = userAgent;
					}

					inline String getUserAgent()
					{
						return (userAgent);
					}

					String receivedData();

					void close();

				private:
					//the io_context is required for all I/O
					boost::asio::io_context ioc_;

					//these objects perform the I/O
					boost::asio::ip::tcp::resolver resolver_{ ioc_ };
					boost::asio::ip::tcp::socket socket_{ ioc_ };
					#ifdef USE_CRYPTOGRAPHY
						boost::asio::ssl::stream<boost::asio::ip::tcp::socket> sslsocket_{ ioc_, RadJAV::CPP::Crypto::SSLContext };
					#endif

					/// The URL connection info.
					URI location;
					RJINT version_;
					String userAgent;
			};
		}
	}
}
#endif

