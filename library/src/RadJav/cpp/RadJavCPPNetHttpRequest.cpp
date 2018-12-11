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
#include "cpp/RadJavCPPNetHttpRequest.h"
#include "cpp/RadJavCPPNetUriParser.h"


#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#elif defined USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#ifdef USE_CRYPTOGRAPHY
#include "cpp/RadJavCPPCryptoBase.h"
#endif

namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
		  
		        HttpRequest2::HttpRequest2(std::map<std::string, std::string> &parms)
			{

				if (parms.find("action") == parms.end() || parms["action"] == "")
				  parms["action"] = "GET";

				if (parms.find("version") == parms.end() || parms["version"] == "")
				  parms["version"] = "1.1";
				
				if (parms.find("userAgent") == parms.end() || parms["userAgent"] == "")
				  parms["userAgent"] = HTTP_USER_AGENT;

				if (parms.find("useDefaultCertificates") == parms.end() || parms["useDefaultCertificates"] == "")
				  parms["useDefaultCertificates"] = "false";
				
				
				if (parms["action"] == "GET")
				  myAction = boost::beast::http::verb::get;
				else if (parms["action"] == "PUT")
				  myAction = boost::beast::http::verb::put;
				else if (parms["action"] == "POST")
				  myAction = boost::beast::http::verb::post;
				else if (parms["action"] == "PATCH")
				  myAction = boost::beast::http::verb::patch;
				else if (parms["action"] == "DELETE")
				  myAction = boost::beast::http::verb::delete_;
				else
				  {
				    throw std::invalid_argument("Illegal action: " + parms["action"]);
				  }

				myUserAgent = parms["userAgent"];

				if (parms["version"] == "1.0")
				  myVersion = 10;
				else if (parms["version"] == "1.1")
				  myVersion = 11;
				else
				  throw std::invalid_argument("Unsupported version: " + parms["version"]);


                                #ifdef USE_CRYPTOGRAPHY
				

				if (parms["useDefaultCertificates"] == "false")
				  myLoadDefaultCerts = false;
				else if (parms["useDefaultCertificates"] == "true")
				  myLoadDefaultCerts = true;
				else
				  throw std::invalid_argument("Invalid parameter useDefaultCertificates: " + parms["useDefaultCertificates"] + ", only 'true' or 'false' is allowed");

				//
				// ------------------------------------------------------
				//

				if (parms.find("verifyMode") != parms.end() && parms["verifyMode"] != "")
				  {
				    std::string::size_type posBegin, posEnd  = 0;
				    std::string input = parms["verifyMode"];
				    std::string delims = ",";

				    input.erase(std::remove_if(input.begin(), 
							       input.end(),
							       [](char x){return std::isspace(x);}),input.end());

				    boost::asio::ssl::verify_mode verifyMode;
				    while ((posBegin = input.find_first_not_of(delims,posEnd)) != std::string::npos)
				      {
					posEnd = input.find_first_of(delims,posBegin);
					if (posEnd == std::string::npos) posEnd = input.length();

					//std::cout << '\'' << input.substr(posBegin,posEnd-posBegin) << '\'' << std::endl;

					if (input.substr(posBegin,posEnd-posBegin) == "VerifyNone")
					  verifyMode |= boost::asio::ssl::verify_none;
					else if (input.substr(posBegin,posEnd-posBegin) == "VerifyPeer")
					  verifyMode |= boost::asio::ssl::verify_peer;
					else if (input.substr(posBegin,posEnd-posBegin) == "VerifyFailIfNoPeerCert")
					  verifyMode |= boost::asio::ssl::verify_fail_if_no_peer_cert;
					else if (input.substr(posBegin,posEnd-posBegin) == "VerifyClientOnce")
					  verifyMode |= boost::asio::ssl::verify_client_once;
					else
					  {
					    RadJav::throwException(std::string("WebSocket Client: invalid sll verifier flag: ") + input.substr(posBegin,posEnd-posBegin));
					    break;
					  }
					myCtx.set_verify_mode(verifyMode);
					

				      }
				  }

				if (parms.find("trustStore") != parms.end() && parms["trustStore"] != "")
				  myCtx.load_verify_file(parms["trustStore"]);

				if (myLoadDefaultCerts)
				  {
				    Array<String> certificates = RadJAV::CPP::Crypto::getDefaultCertificates();

				    for (RJINT iIdx = 0; iIdx < certificates.size(); iIdx++)
				      {
					String certificate = certificates.at (iIdx);
					
					boost::system::error_code ec;
					myCtx.add_certificate_authority(boost::asio::buffer(certificate.c_str(), certificate.size()), ec);
					
					if (ec)
					  throw boost::system::system_error{ec};
					
				      }

				  }

                                #endif
				
			} // End of HttpRequest2()


		        String HttpRequest2::connect(const String &uri)
			{
			  resetHttpRequest();

			  UriParser uriParser{uri};

			  // If port is provided, then we should use it explicitly in the resolver. Otherwise, we should
			  // pass the scheme/protocol and let the resolver deduct the port himself.
			  std::string &service = uriParser.port;
			  if (service == "")
			    service = uriParser.scheme;
			      
			  //look up the domain name
			  
			  boost::asio::ip::tcp::resolver resolver{myIoc};
			  auto const results = resolver.resolve(uriParser.host, service);


			  std::string target = uriParser.path;
			  if (uriParser.query != "") target += "?" + uriParser.query;
			  if (uriParser.fragment != "") target += "#" + uriParser.fragment;
			  


			  if (uriParser.scheme == "https")
			    {
                              #ifdef USE_CRYPTOGRAPHY
			      myIsSslRequest = true;

			      
			      resetSocketSsl();

			      // Set SNI Hostname (many hosts need this to handshake successfully)
			      if(! SSL_set_tlsext_host_name(mySocketSsl -> native_handle(), uriParser.host.c_str()))
				{
				  boost::system::error_code ec{static_cast<int>(::ERR_get_error()), boost::asio::error::get_ssl_category()};
				  throw boost::system::system_error{ec};
				}

			      // Make the connection on the IP address we get from a lookup
			      boost::asio::connect(mySocketSsl -> next_layer(), results.begin(), results.end());

			      // Perform the SSL handshake
			      mySocketSsl -> handshake(boost::asio::ssl::stream_base::client);


			      myRequest -> set(boost::beast::http::field::host, uriParser.host);
			      myRequest -> set(boost::beast::http::field::user_agent, myUserAgent);
			      
			      #else

			      throw std::runtime_error("RadJav needs to be built with USE_CRYTPOGRAPHY enabled to use https");

			      #endif

			      return target;

			    }
			  else if (uriParser.scheme == "http")
			    {
			      resetSocket();

			      //make the connection on the IP address we get from a lookup
			      boost::asio::connect(*mySocket, results.begin(), results.end());

			      //std::string target = uriParser.path;
			      //if (uriParser.query != "") target += "?" + uriParser.query;
			      //			  if (uriParser.fragment != "") target += "#" + uriParser.fragment;

			      myRequest -> set(boost::beast::http::field::host, uriParser.host);
			      myRequest -> set(boost::beast::http::field::user_agent, myUserAgent);

			      return target;
			      
			    }
			  else
			    {
			      throw std::runtime_error("Unsupported scheme/protocol: " + uriParser.scheme); 
			    }
			  
			  return target;
			}
		  
			String HttpRequest2::fetch(const String &target)
			{

			  if (myRequest == nullptr)
			    throw std::runtime_error("Attempting to fetch the target: '" + target + "' with object that is not connected to any host.");

			  myRequest -> target(target);
			  
			  //send the HTTP request to the remote host
			  if (!myIsSslRequest)
			    boost::beast::http::write(*mySocket, *myRequest);
			  
                          #ifdef USE_CRYPTOGRAPHY
			  
			  else
			      boost::beast::http::write(*mySocketSsl, *myRequest);

			  #endif


			  // This buffer is used for reading and must be persisted
			  boost::beast::flat_buffer buffer;


			  // Declare a container to hold the response
			  //http::response<http::dynamic_body> res;
			  http::response<http::dynamic_body> res;

			  // Receive the HTTP response

			  if (!myIsSslRequest)
			    boost::beast::http::read(*mySocket, buffer, res);
			  
                          #ifdef USE_CRYPTOGRAPHY
			  
			  else
			    boost::beast::http::read(*mySocketSsl, buffer, res);

			  #endif

			  return boost::beast::buffers_to_string(res.body().data());
			  
			}

 		        void HttpRequest2::resetHttpRequest()
			{
			  myRequest.reset(new boost::beast::http::request<boost::beast::http::string_body>());
			  myRequest -> version(myVersion);
			  myRequest -> method(myAction);
			}


 		        void HttpRequest2::resetSocket()
			{
			  if (mySocket != nullptr)
			      closeSocket();

			  mySocket.reset(new boost::asio::ip::tcp::socket{myIoc});

			}

		  
			void HttpRequest2::closeSocket()
			{

			  boost::system::error_code ec;
			  mySocket -> shutdown(boost::asio::ip::tcp::socket::shutdown_both, ec);

			  // not_connected happens sometimes
			  // so don't bother reporting it.
			  //
			  if (ec && ec != boost::system::errc::not_connected)
			    throw boost::system::system_error{ec};
			  
			}


                        #ifdef USE_CRYPTOGRAPHY
		  
		        void HttpRequest2::resetSocketSsl()
			{
			  if (mySocketSsl != nullptr)
			      closeSocketSsl();

			  mySocketSsl.reset(new boost::asio::ssl::stream<boost::asio::ip::tcp::socket>{myIoc, myCtx});

			}

			void HttpRequest2::closeSocketSsl()
			{

			  boost::system::error_code ec;
			  mySocketSsl -> shutdown(ec);

			  if(ec)
			    throw boost::system::system_error{ec};
			  
			}

		        #endif
		  
		}
	}
}

