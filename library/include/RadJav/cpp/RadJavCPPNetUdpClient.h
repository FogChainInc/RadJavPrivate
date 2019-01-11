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
#ifndef _RADJAV_CPP_NET_UDPCLIENT_H_
#define _RADJAV_CPP_NET_UDPCLIENT_H_

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
#include "cpp/RadJavCPPChainedPtr.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
		        /**
			 * @class UdpClient
			 * 
			 * @brief The class implement UdpClient functionality. Instantiated objects can send and receive datagrams between other Udp objects.
			 *
			 */
			class RADJAV_EXPORT UdpClient : public ChainedPtr
			{
				public:
				        /** 
					 * @brief Constructs a UdpClient object.
					 *
					 * @param parms A list of key<->value pairs.
					 *
					 * The following parameters are supported:
					 *   - maxDatagramSize [2048] - size of internal buffer, limiting the size of datagrams.
					 */				  
			                UdpClient(std::map<std::string, std::string> parms);

				        /** 
					 * @brief Destroys the object.
					 */
					~UdpClient();

				        /** 
					 * @brief Connects to a remote endpoint.
					 *
					 * @param host A string representing host name or IP4 address. 
					 * @param service A string representing service name or port number.
					 */
					void connect(std::string host, std::string service);

					
				        /** 
					 * @brief Sends a string datagram to a connected endpoint.
					 *
					 * @param message A string message.
					 */
					void send(std::string message);

				        /** 
					 * @brief Sends a binary datagram to a connected endpoint.
					 *
					 * @param message Pointer to the buffer containing message.
					 * @param msgLen Length of the message.
					 */
					void send(const void *message, int msgLen);

					
				        /** 
					 * @brief receives a binary datagram from a connected remote endpoint.
					 *
					 * @param message A setter function accepting pointer to the buffer and buffer length. 
					 */
					void receive(std::function <void (const void *buf, int bufLen)> binSetter);

					

				        /** 
					 * @brief Sends a string datagram to a specified endpoint.
					 *
					 * @param message A string message.
					 * @param host A string representing host name or IP4 address. 
					 * @param service A string representing service name or port number.
					 */
					void send(std::string message, std::string host, std::string service);
					
				        /** 
					 * @brief Sends a binary datagram to a specified remote endpoint.
					 *
					 * @param message Pointer to the buffer containing message.
					 * @param msgLen Length of the message.
					 * @param host A string representing host name or IP4 address. 
					 * @param service A string representing service name or port number.
					 */
					void send(const void *message, int msgLen, std::string host, std::string service);

					
				        /** 
					 * @brief Receives a binary datagram from a specified remote endpoint.
					 *
					 * @param message A setter function accepting pointer to the buffer and buffer length. 
					 * @param host A string representing host name or IP4 address. 
					 * @param service A string representing service name or port number.
					 */
					void receive(std::function <void (const void *buf, int bufLen)> binSetter,
						     std::string host, std::string service);

				        /** 
					 * @brief Closes the connection.
					 */
					void close();

				private:

					/** @name Parametrization */
					//@{
					int myMaxDatagramSize{2048};
					//@}
					
					/** @name ASIO objects */
					//@{
					boost::asio::io_context myIoc;
					std::shared_ptr<boost::asio::ip::udp::socket> mySocket;
					//@}

					/** @name Buffers */
					//@{
					std::shared_ptr<unsigned char> myRecvBuffer;
					//@}
					
			};
		}
	}
}
#endif

