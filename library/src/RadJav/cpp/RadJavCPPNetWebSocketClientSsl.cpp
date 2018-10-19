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
#include "cpp/RadJavCPPNetWebSocketClientSsl.h"

#include "RadJav.h"
#include "RadJavString.h"

#include <regex>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
		  WebSocketClientSsl::WebSocketClientSsl(std::map<std::string, std::string> &parms) :
		    m_ctx(boost::asio::ssl::context::sslv23_client)
			{

			  //for (auto elem : parms)
			  //std::cout << elem.first << " -> " << elem.second << std::endl;
			  
			  m_ctx.load_verify_file(parms["trustStore"]);

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

				  std::cout << '\'' << input.substr(posBegin,posEnd-posBegin) << '\'' << std::endl;

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

				}

			      m_ctx.set_verify_mode(verifyMode);
			      
			    }


			  m_ctx.set_password_callback(
						      [&parms](std::size_t,
							       boost::asio::ssl::context_base::password_purpose)
						      {
							std::cout << "PWD CALLBACK" << std::endl;
							return parms["keystorePwd"];
						      });		    
			  
			  bool hasCertStore = false;
			  bool hasKeyStore = false;
			  if (parms.find("certificateStore") != parms.end() && parms["certificateStore"] != "")
			    {
			      hasCertStore = true;
			      m_ctx.use_certificate_chain_file(parms["certificateStore"]);
			    }


			  if (parms.find("keyStore") != parms.end() && parms["keyStore"] != "")
			    {
			      hasKeyStore = true;
			      m_ctx.use_private_key_file(parms["keyStore"], boost::asio::ssl::context::pem);
			    }
			  
			  // We want keyStore and certificateStore to be both set or both empty
			  if (hasCertStore != hasKeyStore)
			    RadJav::throwException("WebSocket Client: keyStore and certificateStore must be both defined or both undefined");

			  m_ws = std::make_shared<boost::beast::websocket::stream<boost::asio::ssl::stream<boost::asio::ip::tcp::socket>>>(m_ioc, m_ctx);

			  m_ws -> next_layer().set_verify_callback([this](bool preVerified, boost::asio::ssl::verify_context& vctx) -> bool {
			      return verify_cert(preVerified, vctx);
			    });
		
			}


		  bool WebSocketClientSsl::verify_cert(bool preVerified,
						       boost::asio::ssl::verify_context &vctx)
			{

			  if (preVerified)
			    {
			      X509* cert = X509_STORE_CTX_get_current_cert(vctx.native_handle());

			      int isCaCert = X509_check_ca(cert);
			      std::cout << "CA status: " << isCaCert << std::endl << std::flush;

			      if (!isCaCert)
				{
			      
			      
				  BIO *nameBio = BIO_new(BIO_s_mem());

				  X509_NAME_print_ex(nameBio, X509_get_subject_name(cert), 0, XN_FLAG_ONELINE & ~XN_FLAG_SPC_EQ);
			  
				  BUF_MEM *namePtr;
				  BIO_get_mem_ptr(nameBio, &namePtr);

				  std::cout << "Length: " << namePtr -> length << std::endl << std::flush;
				  std::cout << namePtr -> data << std::endl << std::flush;
				  
				  BIO_set_close(nameBio, BIO_NOCLOSE); /* So BIO_free() leaves BUF_MEM alone */
				  BIO_free(nameBio);
			      
				  std::cout << "Length: " << namePtr -> length << std::endl << std::flush;
				  std::cout << namePtr -> data << std::endl << std::flush;
			      
			      
				  delete namePtr;
			      
				}
			    }

			  
			  return preVerified;
			}
		  

			void WebSocketClientSsl::connect(String host_, String port_)
			{
			  
				//look up the domain name
				auto const results = resolver.resolve(host_, port_);


				//make the connection on the IP address we get from a lookup
				boost::asio::connect(m_ws -> next_layer().next_layer(), results.begin(), results.end());

				
				m_ws -> next_layer().handshake(boost::asio::ssl::stream_base::client);


				//perform the websocket handshake
				m_ws -> handshake(host_, "/");

			       std::cout << "CC" << std::endl;
				

			}

			void WebSocketClientSsl::send(String message_)
			{
				//send the message
			        m_ws -> text(true);
				m_ws -> write(boost::asio::buffer(std::string(message_)));
			}
		  
  		        void WebSocketClientSsl::send(const void *message_, int msgLen)
			{
				//send the message
			        m_ws -> binary(true);
				m_ws -> write(boost::asio::buffer(message_, msgLen)); 
			}

			void WebSocketClientSsl::receive(std::function <void (const std::string &str)> stringSetter,
						      std::function <void (const void *buf, int bufLen)> binSetter)
			{
				//this buffer will hold the incoming message
				boost::beast::flat_buffer buffer;

				//read a message into our buffer
				m_ws -> read(buffer);
				if (m_ws -> got_text())
				  {
				    stringSetter(boost::beast::buffers_to_string(buffer.data()));
				  }
				    
				else
				  binSetter(boost::beast::buffers_front(buffer.data()).data(),
					    boost::beast::buffers_front(buffer.data()).size());
			}

			void WebSocketClientSsl::close()
			{
				//close the WebSocket connection
				m_ws -> close(boost::beast::websocket::close_code::normal);
			}
		}
	}
}

