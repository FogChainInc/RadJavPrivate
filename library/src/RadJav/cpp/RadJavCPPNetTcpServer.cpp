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
#include "cpp/RadJavCPPNetTcpServer.h"

#include "RadJav.h"
#include "RadJavString.h"
#include "cpp/RadJavCPPNetWebServer.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{


		  TcpServer::TcpServer(std::map<std::string, std::string> &parms) 
		  {
		    std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;

		    if (parms.find("threads") == parms.end() || parms["threads"] == "")
		      parms["threads"] = "1";

		    if (parms.find("fastServerMode") == parms.end() || parms["fastServerMode"] == "")
		      parms["fastServerMode"] = "false";

		    if (parms.find("bufferSize") == parms.end() || parms["bufferSize"] == "")
		      parms["bufferSize"] = "2048";


		    if (parms["fastServerMode"] == "false")
		      myFastServerMode = false;
		    else if (parms["fastServerMode"] == "true")
		      myFastServerMode = true;
		    else
		      throw std::invalid_argument("Invalid parameter fastServerMode " + parms["fastServerMode"] + ", only 'true' or 'false' is allowed");


		    { 
		      std::string::size_type size;
			    
		      myNoOfIocThreads = std::stoi(parms["threads"], &size);
		      if (parms["threads"].size() != size || myNoOfIocThreads < 1)
			throw std::invalid_argument("Invalid parameter threads " + parms["threads"] + ", only positive numeric values >= 1 are allowed");
		    }			      

		    { 
		      std::string::size_type size;
			    
		      myBufferSize = std::stoi(parms["bufferSize"], &size);
		      if (parms["bufferSize"].size() != size || myBufferSize < 1)
			throw std::invalid_argument("Invalid parameter bufferSize " + parms["bufferSize"] + ", only positive numeric values >= 1 are allowed");
		    }

		    std::cout << "myFastServerMode: " << myFastServerMode << std::endl;
		    std::cout << "myBufferSize: " << myBufferSize << std::endl;
		    
		  } // End of TcpServer

		  TcpServer::~TcpServer()
		  {
		    close();
		  } // End of ~TcpServer

		  void TcpServer::close()
		  {
		    if (myIoc)
		      myIoc -> stop();
		    if (mySocket)
		      {
			mySocket -> close();
			mySocket = nullptr;
		      }
		  }  // End of close
		  
		  void TcpServer::listen(const std::string &host, const std::string &service)
		  {
		    std::cout << "L1: " << host << ":" << service << std::endl << std::flush;
		    //auto const address = boost::asio::ip::make_address(host);


		    if (myOnSend) {
		      std::cout << "OnSend is set" << std::endl;
		    }

		    

		    myIoc = std::make_shared<boost::asio::io_context>(myNoOfIocThreads);

		    boost::asio::ip::tcp::resolver resolver(*myIoc);
		    auto localEndpoint = *resolver.resolve(boost::asio::ip::tcp::v4(), host, service).begin();

		    
		    
		    mySocket = std::make_shared<boost::asio::ip::tcp::socket>(*myIoc);
		    myAcceptor = std::make_shared<boost::asio::ip::tcp::acceptor>(*myIoc, localEndpoint);
		    
		    doAccept();
			  
		    for  (int i=0; i<myNoOfIocThreads; i++)
		      {
			std::cout << "----> Thread: " << i << std::endl;
			std::thread th{[this] { myIoc -> run(); }};
			myIocThreads.push_back(std::move(th));
		      }
		  } // End of listen

		  void TcpServer::doAccept()
		  {
		    std::cout << __PRETTY_FUNCTION__ << std::endl;
		    
		    myAcceptor -> async_accept(
					       [this](boost::system::error_code ec, boost::asio::ip::tcp::socket socket)
					       {
						 if (!ec)
						   {
						     std::string sessionId = boost::uuids::to_string(boost::uuids::random_generator()());
						     auto session = std::make_shared<Session>(std::move(socket),
											      sessionId,
											      this
											      );
						     if (myOnAccept)
						       myOnAccept(sessionId);
						     
						     mySessions[sessionId] = session;
						     session -> start();
						   }
						 
						 doAccept();
					       });

		  } // End of doAccept
		  
		  void TcpServer::Session::startReceive()
		  {
		    std::cout << "A1" << std::endl << std::flush;

		    auto self(shared_from_this());			  
		    // Read a message into our buffer
		    mySocket.async_receive(boost::asio::buffer(myRecvBuffer.get(), myParent -> myBufferSize),
					  boost::bind(&Session::handleReceive,
						      self,
						      boost::asio::placeholders::error,
						      boost::asio::placeholders::bytes_transferred
						      )
					  );

		    
		    std::cout << "A2" << std::endl << std::flush;
		  } // End of startReceive

		  void TcpServer::Session::handleReceive(const boost::system::error_code& ec,
							 std::size_t bytesTransferred
							 )
		  {

		    std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  
		    auto recvBuffer = myRecvBuffer;

			  
		    // If we are in the fast server mode, replace buffers and restart the wait.
		    if (myParent -> myFastServerMode)
		      {
			myRecvBuffer = 
			  std::shared_ptr<unsigned char>(new unsigned char[myParent -> myBufferSize],
							 std::default_delete<unsigned char[]>());

			startReceive();
		      }
			  

		    //std::cout << "BB begin: " << receiveFromEndpoint -> address().to_string() << ":" << receiveFromEndpoint -> port()
		    //	      << std::endl << std::flush;

		    if (!ec)
		      {

			std::cout << "BB2" << std::endl << std::flush;
			std::cout << "Got request, bytes: " << bytesTransferred << std::endl ;
			std::cout << "Thread: " << std::this_thread::get_id() << std::endl;

			if (myParent -> myOnReceive)
			  myParent -> myOnReceive(mySessionId, recvBuffer, bytesTransferred);

		      }
		    else
		      {
			std::cout << "BB: whoops: " << ec.message() << std::endl << std::flush;
			std::cout << ec.category().name() << ':' << ec.value() << std::endl;

			if (ec == boost::asio::error::basic_errors::connection_reset ||
			    ec == boost::asio::error::misc_errors::eof
			    )
			  {
			    auto session = myParent -> mySessions.find(mySessionId);
			    if (session != myParent -> mySessions.end())
			      {
				std::cout << "BOOM, erasing session " << mySessionId << std::endl;
				mySocket.close();
				myParent -> mySessions.erase(session);
				return; 
			      }
			    else
			      {
				// No worries if the session is already gone.
			      }
			  }
			else // Other error
			  throw std::runtime_error("Error while receiving data from TCP socket: " + ec.message());

			/*
			if (myParent -> myOnReceiveError)
			  myParent -> myOnReceiveError(mySessionId, recvBuffer, bytesTransferred);
			*/
			      
		      } // End of ec is error

			  
		    if (!myParent -> myFastServerMode)
		      startReceive();
			  
		  } // End of handleReceive


		  void TcpServer::setOnAcceptCallback(TcpAcceptCallbackType callback)
		  {
		    myOnAccept = callback;
		  } // End of setOnAcceptCallback

		  void TcpServer::setOnReceiveCallback(TcpCallbackType callback)
		  {
		    myOnReceive = callback;
		  } // End of setOnReceiveCallback
		  
		  void TcpServer::setOnSendCallback(TcpCallbackType callback)
		  {
		    myOnSend = callback;
		  } // End of sendOnSendCallback


		  void TcpServer::send(const std::string &sessionId, std::shared_ptr<const unsigned char> data, int dataSize)
		  {
		    auto session = mySessions.find(sessionId);
		    if (session != mySessions.end())
		      session -> second -> send(data, dataSize);
		    else
		      throw std::invalid_argument("Session " + sessionId + " not found.");


		  } // End of send shared_ptr

		  void TcpServer::send(const std::string &sessionId, const void *data, int dataSize)
		  {
		    std::cout << __PRETTY_FUNCTION__ << std::endl;
		    
		    auto session = mySessions.find(sessionId);
		    if (session != mySessions.end())
		      session -> second -> send(data, dataSize);
		    else
		      throw std::invalid_argument("Session " + sessionId + " not found.");
		    
		  } // End of send void*

		  void TcpServer::send(const std::string &sessionId, const std::string &msg)
		  {
		    auto session = mySessions.find(sessionId);
		    if (session != mySessions.end())
		      session -> second -> send(msg);
		    else
		      throw std::invalid_argument("Session " + sessionId + " not found.");
		    
		  } // End of send string
		  

		  void TcpServer::Session::send(std::shared_ptr<const unsigned char> data, int dataSize)
		  {
		    std::cout  << __PRETTY_FUNCTION__ << ": Queuing" << std::endl;
		    
		    myParent -> myIoc -> post(boost::asio::bind_executor(myWriteStrand,
									 [this, data, dataSize] ()
									 {
									   queueMessage(mySessionId, data, dataSize);
									 }
									 ));

		  } // End of send binary

		  void TcpServer::Session::send(const void *data, int dataSize)
		  {
		    auto dataCache = std::shared_ptr<unsigned char>(new unsigned char[dataSize],
								    std::default_delete<unsigned char[]>());
		    std::memcpy(dataCache.get(), data, dataSize);

		    send(dataCache, dataSize);

		  } // End of send binary

		  void TcpServer::Session::send(const std::string &msg)
		  {
		    int dataSize = msg.size() + 1;
		    auto dataCache = std::shared_ptr<unsigned char>(new unsigned char[dataSize],
								    std::default_delete<unsigned char[]>());
		    std::memcpy(dataCache.get(), msg.c_str(), dataSize);

		    send(dataCache, dataSize);

		  } // End of send string
		  
		  
		  void TcpServer::Session::queueMessage(const std::string &sessionId,
							std::shared_ptr<const unsigned char> data,
							int dataSize
							)
		  {
		    bool write_in_progress = !mySendPacketQueue.empty();
		    mySendPacketQueue.push_back(std::make_tuple(sessionId, data, dataSize));

		    if (! write_in_progress)
		      startPacketSend();
		  } // End of queueMessage


		  void TcpServer::Session::startPacketSend()
		  {
		    auto sessionId = std::get<0>(mySendPacketQueue.front());
		    auto dataPtr = std::get<1>(mySendPacketQueue.front());
		    auto dataSize = std::get<2>(mySendPacketQueue.front());

		    auto self(shared_from_this());			  
		    // Write data out
		    mySocket.async_send(boost::asio::buffer(dataPtr.get(), dataSize),
					boost::asio::bind_executor(myWriteStrand,
								  boost::bind(&Session::handleSendDone,
									      self,
									      dataPtr, dataSize,
									      boost::asio::placeholders::error,
									      boost::asio::placeholders::bytes_transferred
									      )
								  )
				       );
		    
		  } // End of startPacketSend
		  
		    
		  void TcpServer::Session::handleSendDone(std::shared_ptr<const unsigned char> data, int dataSize,
							  const boost::system::error_code& error,
							  std::size_t bytesTransferred
							  )
		  {
		    if (!error)
		      {

			if (myParent -> myOnSend)
			  myParent -> myOnSend(mySessionId, data, bytesTransferred);			    
			
			mySendPacketQueue.pop_front();
			if (!mySendPacketQueue.empty()) { startPacketSend(); }
		      }
		    else
		      {
			if (myParent -> myOnSendError)
			  myParent -> myOnSendError(mySessionId, data, bytesTransferred);			    
		      }
		  } // End of handleSendDone


		  TcpServer::Session::Session(boost::asio::ip::tcp::socket socket,
					      const std::string &sessionId,
					      TcpServer *parent
					      )
		    : mySocket{std::move(socket)},
		      mySessionId{sessionId},
		      myParent{parent},
		      myWriteStrand{*parent -> myIoc}
		  {
		    std::cout << __PRETTY_FUNCTION__ << std::endl;
		    myRecvBuffer = 
		      std::shared_ptr<unsigned char>(new unsigned char[myParent -> myBufferSize],
						     std::default_delete<unsigned char[]>());
		    
		  } // End of constructor

		  TcpServer::Session::~Session()
		  {
		    std::cout << "DELETING: " << mySessionId << std::endl;
		    mySocket.close();
		  } // End of destructor
		  
		  void TcpServer::Session::start()
		  {
		    //doRead();
		    startReceive();
		  }
		  
		  
		} // End of Net
	} // End of CPP
} // End of RadJAV
