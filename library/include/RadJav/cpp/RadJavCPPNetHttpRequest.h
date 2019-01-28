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
#ifndef _RADJAV_CPP_NET_HTTPREQUEST_H_
#define _RADJAV_CPP_NET_HTTPREQUEST_H_

#include "RadJavPreprocessor.h"

#include <boost/asio.hpp>

#include <boost/beast/core.hpp>
#include <boost/beast/version.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/http.hpp>

#ifdef USE_CRYPTOGRAPHY
#include <boost/asio/ssl/error.hpp>
#include <boost/asio/ssl/stream.hpp>
#endif

#include "RadJav.h"
#include "RadJavString.h"
#include "RadJavHashMap.h"
#include "cpp/RadJavCPPChainedPtr.h"

#include <memory>
#include <map>

//#ifdef NET_ON

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			/**
			 * @ingroup group_net_cpp
			 * @brief HttpRequest2 class.
			 */
			class RADJAV_EXPORT HttpRequest2 : public ChainedPtr
			{
				public:
					HttpRequest2(std::map<std::string, std::string> &parms);

					String fetch(const String &target);
					String connect(const String &uri);

				protected:
					//the io_context is required for all I/O
					boost::asio::io_context myIoc;

					//these objects perform our I/O
					boost::beast::http::verb myAction;

					std::unique_ptr<boost::asio::ip::tcp::socket> mySocket;

					std::unique_ptr<boost::beast::http::request<boost::beast::http::string_body>> myRequest;
				#ifdef USE_CRYPTOGRAPHY
					boost::asio::ssl::context myCtx{boost::asio::ssl::context::sslv23_client};
					std::unique_ptr<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>> mySocketSsl;
				#endif

					RJINT myVersion;
					String myUserAgent;
					bool myLoadDefaultCerts{false};

					bool myIsSslRequest{false};

					void resetHttpRequest();
					void closeSocket();
					void resetSocket();

				#ifdef USE_CRYPTOGRAPHY
					void closeSocketSsl();
					void resetSocketSsl();
				#endif
			};
		}
	}
}
#endif

