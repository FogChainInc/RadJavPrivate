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
#include "cpp/RadJavCPPNetWebSocketClient.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			WebSocketClient::WebSocketClient() {}

			#ifdef USE_V8
			void WebSocketClient::on(String event_, v8::Local<v8::Function> func_)
			{
			}
			#endif

			void WebSocketClient::connect(String host_, String port_)
			{
				//look up the domain name
				auto const results = resolver.resolve(host_, port_);

				//make the connection on the IP address we get from a lookup
				boost::asio::connect(m_ws.next_layer(), results.begin(), results.end());

				//perform the websocket handshake
				m_ws.handshake(host_, "/");

			}

			void WebSocketClient::send(String message_)
			{
				//send the message
			        m_ws.text(true);
				m_ws.write(boost::asio::buffer(std::string(message_)));
			}
		  
  		        void WebSocketClient::send(const void *message_, int msgLen)
			{
				//send the message
			        m_ws.binary(true);
				m_ws.write(boost::asio::buffer(message_, msgLen)); 
			}

			void WebSocketClient::receive(std::function <void (const std::string &str)> stringSetter,
						      std::function <void (const void *buf, int bufLen)> binSetter)
			{
				//this buffer will hold the incoming message
				boost::beast::flat_buffer buffer;

				//read a message into our buffer
				m_ws.read(buffer);
				if (m_ws.got_text())
				  {
				    stringSetter(boost::beast::buffers_to_string(buffer.data()));
				  }
				    
				else
				  binSetter(boost::beast::buffers_front(buffer.data()).data(),
					    boost::beast::buffers_front(buffer.data()).size());
			}

			void WebSocketClient::close()
			{
				//close the WebSocket connection
				m_ws.close(boost::beast::websocket::close_code::normal);
			}
		}
	}
}

