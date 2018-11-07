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
#include "cpp/RadJavCPPNetUriParser.h"

#include "RadJav.h"
#include "RadJavString.h"
#include <boost/algorithm/string.hpp>

#ifdef USE_CRYPTOGRAPHY
	#include "cpp/RadJavCPPCryptoBase.h"
#else
	#pragma message ("NOT using cryptography library. HTTPS Support is disabled.")
#endif

#include <regex>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			URI URI::parse(const String &url)
			{
				URI result;

				auto validate_match = [](const std::string& value, std::string&& default_value) -> String
				{
					return (value.empty() ? default_value : value);
				};

				// Note: only "http", "https", "ws", and "wss" protocols are supported
				static const std::regex PARSE_URL{ R"((([httpsw]{2,5})://)?([^/ :]+)(:(\d+))?(/([^ ?]+)?)?/?\??([^/ ]+\=[^/ ]+)?)",
					std::regex_constants::ECMAScript | std::regex_constants::icase };
				std::smatch match;

				if (std::regex_match(url, match, PARSE_URL) && match.size() == 9)
				{
					result.protocol = validate_match(boost::algorithm::to_lower_copy(std::string(match[2])), "http");
					result.host = (String)match[3];
					const bool secure_protocol = (result.protocol == "https" || result.protocol == "wss");
					result.port = parseInt (validate_match(match[5], (secure_protocol) ? "443" : "80"));
					result.resource = validate_match(match[6], "/");
					result.query = (String)match[8];
					result.target = result.query + (result.resource.size() > 0, std::string("?") + result.resource, result.resource);
				}

				return result;
			}

			HttpRequest::HttpRequest(String url)
			{
				location = URI::parse (url);
				version_ = 11;
			}

			void HttpRequest::connect()
			{
				#ifdef USE_CRYPTOGRAPHY
					if (SSL_set_tlsext_host_name(sslsocket_.native_handle(), location.host.c_str ()) == false)
					{
						/// @fixme Throw an error message.

						return;
					}
				#endif

				//look up the domain name
				auto const results = resolver_.resolve(location.host, String::fromInt(location.port));

				//make the connection on the IP address we get from a lookup
				if (location.protocol == "https")
				{
					#ifdef USE_CRYPTOGRAPHY
						boost::asio::connect(sslsocket_.next_layer (), results.begin(), results.end());
					#else
					/// @fixme Throw an error message saying cryptography library is not enabled.
					return;
					#endif
				}
				else
					boost::asio::connect(socket_, results.begin(), results.end());

				#ifdef USE_CRYPTOGRAPHY
					sslsocket_.handshake(boost::asio::ssl::stream_base::client);
				#endif
			}

			void HttpRequest::send()
			{
				auto action = http::verb::get;

				//if (post)
					//action = http::verb::post;

				//set up an HTTP GET/POST request message
				http::request<http::string_body> req;
				req.version(version_);
				req.method(action);
				req.target(location.target);
				req.set(http::field::host, location.host);
				req.set(http::field::user_agent, HTTP_USER_AGENT);

				//send the HTTP request to the remote host
				if (location.protocol == "https")
				{
					#ifdef USE_CRYPTOGRAPHY
						http::write(sslsocket_, req);
					#else
						/// @fixme Throw an error message saying cryptography library is not enabled.
						return;
					#endif
				}
				else
					http::write(socket_, req);
			}

			String HttpRequest::receivedData()
			{
				//this buffer is used for reading and must be persisted
				boost::beast::flat_buffer buffer;

				//declare a container to hold the response
				http::response<http::dynamic_body> res;

				//receive the HTTP response
				if (location.protocol == "https")
				{
					#ifdef USE_CRYPTOGRAPHY
						http::read(sslsocket_, buffer, res);
					#else
						/// @fixme Throw an error message saying cryptography library is not enabled.
						return;
					#endif
				}
				else
					http::read(socket_, buffer, res);

				//return the response
				return boost::beast::buffers_to_string(res.body().data());
			}

			void HttpRequest::close()
			{
				//gracefully close the socket
				boost::system::error_code ec;

				if (location.protocol == "https")
				{
					#ifdef USE_CRYPTOGRAPHY
						sslsocket_.shutdown(ec);
					#else
						/// @fixme Throw an error message saying cryptography library is not enabled.
						return;
					#endif
				}
				else
					socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);
			}
		}
	}
}

