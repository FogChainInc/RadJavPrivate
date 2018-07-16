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
#include "cpp/RadJavCPPNet.h"

#include "RadJav.h"
#include "RadJavString.h"
#include <boost/algorithm/string.hpp>
#include <regex>


namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			HttpRequest::HttpRequest(String url)
			{
				//TO DO: Need a URI parser here to get:
				//	host
				//	port
				//	target
				//	version
			}

			void HttpRequest::connect()
			{
				//look up the domain name
				auto const results = resolver.resolve(host, port);

				//make the connection on the IP address we get from a lookup
				boost::asio::connect(socket, results.begin(), results.end());
			}

			void HttpRequest::send(bool post)
			{
				auto action = http::verb::get;

				if (post) action = http::verb::post;

				//set up an HTTP GET/POST request message
				http::request<http::string_body> req{ action, target, version };
				req.set(http::field::host, host);
				req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);

				//send the HTTP request to the remote host
				http::write(socket, req);
			}

			String HttpRequest::receivedData()
			{
				//this buffer is used for reading and must be persisted
				boost::beast::flat_buffer buffer;

				//declare a container to hold the response
				http::response<http::dynamic_body> res;

				//receive the HTTP response
				http::read(socket, buffer, res);

				//return the response
				return boost::beast::buffers_to_string(res.body().data());
			}

			void HttpRequest::close()
			{
				//gracefully close the socket
				boost::system::error_code ec;
				socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
			}
		}
	}
}

