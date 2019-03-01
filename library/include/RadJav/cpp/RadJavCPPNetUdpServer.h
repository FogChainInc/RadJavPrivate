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
#ifndef _RADJAV_CPP_NET_UDPSERVER_H_
	#define _RADJAV_CPP_NET_UDPSERVER_H_

	#include "RadJavPreprocessor.h"

        #include <boost/array.hpp>
        #include <boost/bind.hpp>
        #include <boost/shared_ptr.hpp>
        #include <boost/asio.hpp>

	#include "RadJavString.h"
	#include "RadJavHashMap.h"
	#include "cpp/RadJavCPPChainedPtr.h"

        #include <thread>
        #include <deque>
        #include <tuple>


	namespace RadJAV
	{
		namespace CPP
		{
			namespace Net
			{
			        /**
				 * @type UdpCallbackType
				 * @brief Defines callback signature.
				 */
			        typedef std::function <void (std::shared_ptr<const unsigned char> buf, int bufLen, const std::string &senderAddr, unsigned short port)> UdpCallbackType;

				/** @class UdpServer
				 * 
				 * @brief The class implements UDP server functionality.
				 * 
				 * The class constructor takes a set of parameters allowing for configuration of object behavior, such as number of delivery threads,
				 * performance, etc.
				 */				  
			        class RADJAV_EXPORT UdpServer : public ChainedPtr
				{
				public:
				  /**
				   * @brief Constructs a UdpServer object.
				   *
				   * @param parms A list of key<->value pairs.
				   *
				   * The following parameters are supported.
				   *   - threads [1] - the number of delivery threads for ASIO context.
				   *   - maxDatagramSize [2048] - size of internal buffer, limiting the size of datagrams.
				   *   - fastServerMode [false] - when disabled (set to false), the object will not be ready to accept another datagram
				   *     until processing the current datagram, including execution of provided onReceive function has finished. When enabled 
				   *     (set to true), the object will cache incoming buffer on the heap and will resume waiting for datagrams. In this mode
				   *     the object might consume more memory, but is more responsive.
				   */				  
				  UdpServer(std::map<std::string, std::string> &parms);

				  
				  /**
				   * @brief Destroys the object.
				   */
				  ~UdpServer();
				  
				  /**
				   * @brief Closes the socket. No more datagrams will be accepted or sent from the object.
				   *
				   * @return void
				   */
				  void close();

				  /**
				   * @brief Discards the previous listen operation and associated structures, initializes new state and begins to listen for incoming datagrams.
				   * 
				   * @param host String representation of host name or IP4 address. Passing "0.0.0.0" will cause listening on all interfaces.
				   * @param service Service name or port number.
				   * @return void.
				   *
				   * Note, the method does not block.
				   */				  
				  void listen(const std::string& host, const std::string &service);

				  /**
				   * @brief Initiate sending a datagram from a buffer on heap to a remote target.
				   *
				   * @param data Data to be sent.
				   * @param dataSize Data size/length in bytes.
				   * @param host String representation of host name or IP4 address. 
				   * @param service Service name or port number.
				   * @return void.
				   *
				   * Note, the method does not block.
				   */				  
				  void send(std::shared_ptr<const unsigned char> data, int dataSize, const std::string &address, const std::string &service);

				  /**
				   * @brief Initiate sending a datagram from a buffer on heap to a remote target.
				   *
				   * @param data Data to be sent.
				   * @param dataSize Data size/length in bytes.
				   * @param host String representation of host name or IP4 address. 
				   * @param service Service name or port number.
				   * @return void.
				   *
				   * Note, the method does not block. Data will be cached.
				   */				  
				  void send(const void *data, int dataSize, const std::string &address, const std::string &service);

				  /**
				   * @brief Initiate sending a string datagram from a buffer on heap to a remote target.
				   * 
				   * @brief msg String message to be sent.
				   * @param host String representation of host name or IP4 address. 
				   * @param service Service name or port number.
				   * @return void.
				   *
				   * Note, the method does not block.
				   */				  
				  void send(const std::string &msg, const std::string &address, const std::string &service);

				  /**
				   * @brief Sets onReceive handler.
				   *
				   * @param UdpCallbackType callback.
				   * @return void
				   * 
				   * User code can call this function at any time, user can also overwrite the callback with new one. The callback function gets access
				   * to the datagram received by the object, as well as remote endpoint parameters (hostname/IP and service/port).
				   */
				  void setOnReceiveCallback(UdpCallbackType callback);

				  /**
				   * @brief Sets onSend handler.
				   *
				   * @param UdpCallbackType callback.
				   * @return void
				   * 
				   * User code can call this function at any time, user can also overwrite the callback with new one. The callback function gets access
				   * to the datagram sent by the object, as well as remote endpoint parameters (hostname/IP and service/port). Note, all write requests are
				   * serialized, hence this calback can be used to indicate when the particular datagram was actually sent.
				   */
				  void setOnSendCallback(UdpCallbackType callback);


				protected:
				  /** @name Datagram receiving */
				  //@{
				  /**
				   * @brief Initiates waiting for a datagram.
				   */
				  void startReceive();

				  /**
				   * @brief Captures the datagram and executes callback if provided.
				   */
				  void handleReceive(const boost::system::error_code& error,
						     std::size_t bytes_transferred
						     );
				  //@}


				  /** @name Datagram sending */
				  //@{
				  /**
				   * @brief Adds message to be sent to queue.
				   */
				  void queueMessage(std::shared_ptr<const unsigned char> data, int dataSize, std::shared_ptr<boost::asio::ip::udp::endpoint> sendToEndpoint);

				  /**
				   * @brief Called when object is ready to send out another datagram.
				   */
				  void startPacketSend();

				  /**
				   * @brief Called after the datagram was sent out.Executes callback if provided.
				   */
				  void handleSendDone(std::shared_ptr<const unsigned char> data, int dataSize,
						      const boost::system::error_code& /*error*/,
						      std::size_t /*bytes_transferred*/,
						      std::shared_ptr<boost::asio::ip::udp::endpoint> remoteEndpoint
						      );
				  //@}
				  
				protected:
				  /** @name Parametrization */
				  //@{
				  bool myFastServerMode{false};
				  int myNoOfIocThreads{1};
				  int myMaxDatagramSize{2048};
				  //@}

				  /** @name ASIO objects */
				  //@{
				  std::shared_ptr<boost::asio::io_context> myIoc;
				  std::shared_ptr<boost::asio::ip::udp::socket> mySocket;
				  std::shared_ptr<boost::asio::io_context::strand> myWriteStrand;
				  std::shared_ptr<boost::asio::ip::udp::endpoint> myReceiveFromEndpoint;
				  //@}
				  
				  /** @name Buffers */
				  //@{
				  std::shared_ptr<unsigned char> myRecvBuffer;
				  std::deque<std::tuple<std::shared_ptr<const unsigned char>, int, std::shared_ptr<boost::asio::ip::udp::endpoint>>> mySendPacketQueue;
				  //@}


				  /** @name Threading */
				  //@{
				  std::vector<std::thread> myIocThreads;
				  //@}


				  /** @name Callback members */
				  //@{
				  /** Callback function, invoked when condition happens */
				  UdpCallbackType myOnReceive;
				  UdpCallbackType myOnSend;
				  UdpCallbackType myOnReceiveError;
				  UdpCallbackType myOnSendError;
				  //@}

				  
				};
			  
			}
		}
	}
#endif

