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
#ifndef _RADJAV_CPP_NET_TCPSERVER_H_
	#define _RADJAV_CPP_NET_TCPSERVER_H_

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
				 * @bug In a test case with 10 threads, and a small buffer (10 bytes), if we flood the TCP server with echo requests (write to server 
				 *   something that is 3 or so times larger than the buffer and  send data back in the onReceive callback, in some cases the data comes 
				 *   back garbled. This issue doesn't not occur when the number of threads is set to 1.
				 */
			  
			        //@{
			        /**
				 * @typedef TcpCallbackType
				 *
				 * @brief Defines callback signature.
				 */
			        typedef std::function <void (const std::string &sessionId, std::shared_ptr<const unsigned char> buf, int bufLen)> TcpCallbackType;
			        typedef std::function <void (const std::string &sessionId)> TcpAcceptCallbackType;
				//@}
							     
							     

				/**
				 * @class TcpServer
				 * 
				 * @brief The class implements TCP server functionality.
				 * 
				 * The class constructor takes a set of parameters allowing for configuration of object behavior, such as number of delivery threads,
				 * performance, etc.
				 */				  
			        class RADJAV_EXPORT TcpServer : public ChainedPtr
				{

				  class Session
				    : public std::enable_shared_from_this<Session>
				  {
				  public:
				    /**
				     * @brief Constructs a session object
				     *
				     * @param socket ASIO socket to read from/write to.
				     * @param sessionID A string representing the session ID.
				     * @param parent A link/pointer to the parent TcpServer object.
				     */				  
				    Session(boost::asio::ip::tcp::socket socket,
					    const std::string &sessionId,
					    TcpServer *parent
					    );

				    /**
				     * @brief Closes socket, destroys object.
				     */				  
				    ~Session();

				    /**
				     * @brief Starts the session
				     */				  
				    void start();

				    /** @name Sending */
				    //@{
				    /**
				     * @brief Initiate sending data from a buffer on heap to a remote target.
				     *
				     * @param host String representation of host name or IP4 address.
				     * @param service Service name or port number.
				     * @return void.
				     *
				     * Note, the method does not block.
				     */				  
				    void send(std::shared_ptr<const unsigned char> data, int dataSize);

				    /**
				     * @brief Initiate sending data from source owned by caller to a remote target.
				     *
				     * @param host String representation of host name or IP4 address. 
				     * @param service Service name or port number.
				     * @return void.
				     *
				     * Note, the method does not block. The data will be cached internally.
				     */				  
				    void send(const void *data, int dataSize);

				    /**
				     * @brief Initiate sending a string to a remote target.
				     * 
				     * @param host String representation of host name or IP4 address. 
				     * @param service Service name or port number.
				     * @return void.
				     *
				     * Note, the method does not block. The data wil be cached internally.
				     */				  
				    void send(const std::string &msg);
				    //@}

				  protected:

				    /** @name Packet receiving */
				    //@{
				    /**
				     * @brief Initiates waiting for a message.
				     */
				    void startReceive();
				    //@}

				    /** @name Packet sending */
				    //@{

				    /**
				     * @brief Adds message to be sent to queue.
				     */
				    void queueMessage(const std::string &sessionId, std::shared_ptr<const unsigned char> data, int dataSize);
						     
				    /** 
				     * @brief Called when object is ready to send out another message.
				     */
				    void startPacketSend();

				    /** 
				     * @brief Called after the message was sent out.Executes callback if provided.
				     */
				    void handleSendDone(std::shared_ptr<const unsigned char> data, int dataSize,
							const boost::system::error_code& ec,
							std::size_t bytes_transferred
							);

				    /**
				     * @brief Captures the message and executes callback if provided.
				     */
				    void handleReceive(const boost::system::error_code& error,
						       std::size_t bytes_transferred
						       );
				    //@}

				  protected:
				    
				    /** @name ASIO Objects */
				    //@{
				    boost::asio::io_context::strand myWriteStrand;
				    boost::asio::ip::tcp::socket mySocket;
				    //@}
				    
				    /** @name Buffers */
				    //@{
				    std::shared_ptr<unsigned char> myRecvBuffer;

				    // buffer, bufferSize, endpoint
				    std::deque<std::tuple<std::string, std::shared_ptr<const unsigned char>, int>> mySendPacketQueue;
				    //@}
				    
				  private:
				    std::string mySessionId;

				    TcpServer *myParent;
				    
				  };

				public:
				  /** 
				   * @brief Constructs a TcpServer object.
				   *
				   * @param parms A list of key<->value pairs.
				   *
				   * The following parameters are supported.
				   *   - threads [1] - the number of delivery threads for ASIO context.
				   *   - bufferSize [2048] - size of internal buffer.
				   *   - fastServerMode [false] - when disabled (set to false), the object will not be ready to accept another message
				   *     until processing the current message, including execution of provided onReceive function has finished. When enabled 
				   *     (set to true), the object will cache incoming buffer on the heap and will resume waiting for messages. In this mode
				   *     the object might consume more memory, but is more responsive.
				   */				  
				  TcpServer(std::map<std::string, std::string> &parms);
				  
				  /** 
				   * @brief Destroys the object.
				   */
				  ~TcpServer();
				  
				  /** 
				   * @brief Closes the socket. No more messages will be accepted or sent from the object.
				   *
				   * @return void
				   */
				  void close();

				  /**
				   * @brief Discards the previous listen operation and associated structures, initializes new state and begins to listen for incoming messages.
				   * 
				   * @param host String representation of host name or IP4 address. Passing "0.0.0.0" will cause listening on all interfaces.
				   * @param service Service name or port number.
				   * @return void.
				   *
				   * Note, the method does not block.
				   */				  
				  void listen(const std::string& host, const std::string &service);

				  /**
				   * @brief Sets onAccept handler.
				   *
				   * @param TcpAcceptCallbackType callback.
				   * @return void
				   * 
				   * User code can call this function at any time, user can also overwrite the callback with new one. The callback function gets access
				   * to the sessionID.
				   */
				  void setOnAcceptCallback(TcpAcceptCallbackType callback);

				  /**
				   * @brief Sets onReceive handler.
				   *
				   * @param TcpCallbackType callback.
				   * @return void
				   * 
				   * User code can call this function at any time, user can also overwrite the callback with new one. The callback function gets access
				   * to the message received by the object.
				   */
				  void setOnReceiveCallback(TcpCallbackType callback);
				  
				  /**
				   * @brief Sets onSend handler.
				   *
				   * @param TcpCallbackType callback.
				   * @return void
				   * 
				   * User code can call this function at any time, user can also overwrite the callback with new one. The callback function gets access
				   * to the message sent by the object, as well as remote endpoint parameters (hostname/IP and service/port). Note, all write requests are
				   * serialized, hence this calback can be used to indicate when the particular message was actually sent.
				   */
				  void setOnSendCallback(TcpCallbackType callback);

				  /** @name Sending */
				  //@{
				  /**
				   * @brief Initiate sending data from a buffer on heap to a remote target.
				   *
				   * @param sessionId Identifies which session should send data.
				   * @param data Data to be sent.
				   * @param dataSize Data size/length in bytes.
				   * @return void.
				   *
				   * Note, the method does not block.
				   */				  
				  void send(const std::string &sessionId, std::shared_ptr<const unsigned char> data, int dataSize);


				  /**
				   * @brief Initiate sending data from source owned by caller to a remote target.
				   *
				   * @param sessionId Identifies which session should send data.
				   * @param data Data to be sent.
				   * @param dataSize Data size/length in bytes.
				   * @return void.
				   *
				   * Note, the method does not block. The data will be cached internally.
				   */				  
				  void send(const std::string &sessionId, const void *data, int dataSize);

				  /**
				   * @brief Initiate sending a string to a remote target.
				   * 
				   * @param sessionId Identifies which session should send data.
				   * @param msg String to be sent.
				   * @return void.
				   *
				   * Note, the method does not block. The data wil be cached internally.
				   */				  
				  void send(const std::string &sessionId, const std::string &msg);
				  //@}


				protected:
				  /** @name Acceptor */
				  //@{
				  void doAccept();
				  //@}


				protected:
				  /** @name Parametrization */
				  //@{
				  bool myFastServerMode{false};
				  int myNoOfIocThreads{1};
				  int myBufferSize{2048};
				  //@}

				  /** @name ASIO Objects */
				  //@{
				  std::shared_ptr<boost::asio::io_context> myIoc;
				  std::shared_ptr<boost::asio::ip::tcp::acceptor> myAcceptor;
				  std::shared_ptr<boost::asio::ip::tcp::socket> mySocket;
				  //@}
				  
				  /** @name Buffers */
				  //@{
				  std::shared_ptr<unsigned char> myRecvBuffer;

				  /** @name Threading */
				  //@{
				  std::vector<std::thread> myIocThreads;
				  //@}


				  /** @name Callback Members */
				  //@{
				  /** Callback function, invoked when condition happens */
				  TcpAcceptCallbackType myOnAccept;
				  TcpCallbackType myOnReceive;
				  TcpCallbackType myOnSend;
				  TcpCallbackType myOnReceiveError;
				  TcpCallbackType myOnSendError;
				  //@}

				  
				  /** @name Session Management */
				  //@{
				  std::map<std::string, std::shared_ptr<Session>> mySessions;
				  //@}

				}; 
			  
			} // End of Net
		} // End of CPP
	} // End of RadJAV

#endif

