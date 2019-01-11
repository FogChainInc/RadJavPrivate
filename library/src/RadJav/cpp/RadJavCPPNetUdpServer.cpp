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
#include "cpp/RadJavCPPNetUdpServer.h"

#include "RadJav.h"
#include "RadJavString.h"
#include "cpp/RadJavCPPNetWebServer.h"
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{


		  UdpServer::UdpServer(std::map<std::string, std::string> &parms) 
		  {
		    //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;

		    if (parms.find("threads") == parms.end() || parms["threads"] == "")
		      parms["threads"] = "1";

		    if (parms.find("fastServerMode") == parms.end() || parms["fastServerMode"] == "")
		      parms["fastServerMode"] = "false";

		    if (parms.find("maxDatagramSize") == parms.end() || parms["maxDatagramSize"] == "")
		      parms["maxDatagramSize"] = "2048";


		    if (parms["fastServerMode"] == "false")
		      myFastServerMode = false;
		    else if (parms["fastServerMode"] == "true")
		      myFastServerMode = true;
		    else
		      throw std::invalid_argument("Invalid parameter fastServerMode " + parms["fastServerMode"] + ", only 'true' or 'false' is allowed");

		    std::cout << "myFastServerMode: " << myFastServerMode << std::endl;

		    { 
		      std::string::size_type size;
			    
		      myNoOfIocThreads = std::stoi(parms["threads"], &size);
		      if (parms["threads"].size() != size || myNoOfIocThreads < 1)
			throw std::invalid_argument("Invalid parameter threads " + parms["threads"] + ", only positive numeric values >= 1 are allowed");
		    }			      

		    { 
		      std::string::size_type size;
			    
		      myMaxDatagramSize = std::stoi(parms["maxDatagramSize"], &size);
		      if (parms["maxDatagramSize"].size() != size || myMaxDatagramSize < 1)
			throw std::invalid_argument("Invalid parameter maxDatagramSize " + parms["maxDatagramSize"] + ", only positive numeric values >= 1 are allowed");
		    }			      
		  } // End of UdpServer

		  UdpServer::~UdpServer()
		  {
		    close();
		  } // End of ~UdpServer

		  void UdpServer::close()
		  {
		    if (myIoc)
		      myIoc -> stop();
		    if (mySocket)
		      {
			mySocket -> close();
			mySocket = nullptr;
		      }
		  }  // End of close
		  
		  void UdpServer::listen(const std::string &host, const std::string &service)
		  {
		    std::cout << "L1: " << host << ":" << service << std::endl << std::flush;
		    //auto const address = boost::asio::ip::make_address(host);


		    std::cout << "L2" << std::endl << std::flush;
		    std::cout << "L3" << std::endl << std::flush;

		    if (myOnSend) {
		      std::cout << "OnSend is set" << std::endl;
		    }

		    myIoc = std::make_shared<boost::asio::io_context>(myNoOfIocThreads);

		    boost::asio::ip::udp::resolver resolver(*myIoc);
		    auto localEndpoint = *resolver.resolve(boost::asio::ip::udp::v4(), host, service).begin();
		    
		    mySocket = std::make_shared<boost::asio::ip::udp::socket>(*myIoc, localEndpoint);
		    
		    myWriteStrand = std::make_shared<boost::asio::io_context::strand>(*myIoc);
			  

		    myRecvBuffer = 
		      std::shared_ptr<unsigned char>(new unsigned char[myMaxDatagramSize],
						     std::default_delete<unsigned char[]>());
		    
		    myReceiveFromEndpoint = std::make_shared<boost::asio::ip::udp::endpoint>();
		    // that was intended for endpoint for writing  myReceiveFromEndpoint = std::make_shared<boost::asio::ip::udp::endpoint>();

		    startReceive();
			  
		    for  (int i=0; i<myNoOfIocThreads; i++)
		      {
			std::cout << "----> Thread: " << i << std::endl;
			std::thread th{[this] { myIoc -> run(); }};
			myIocThreads.push_back(std::move(th));
		      }
		  } // End of listen
		  
		  void UdpServer::startReceive()
		  {
		    std::cout << "A1" << std::endl << std::flush;

		    mySocket -> async_receive_from(
						   boost::asio::buffer(myRecvBuffer.get(), myMaxDatagramSize), *myReceiveFromEndpoint,
						   boost::bind(&UdpServer::handleReceive, this,
							       boost::asio::placeholders::error,
							       boost::asio::placeholders::bytes_transferred
							       ));

		    std::cout << "A2" << std::endl << std::flush;
		  } // End of startReceive

		  void UdpServer::handleReceive(const boost::system::error_code& error,
						std::size_t bytesTransferred
						)
		  {

		    //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  
		    auto recvBuffer = myRecvBuffer;
		    auto receiveFromEndpoint = myReceiveFromEndpoint;
			  
		    // If we are in the fast server mode, replace buffers and restart the wait.
		    if (myFastServerMode)
		      {
			myRecvBuffer = 
			  std::shared_ptr<unsigned char>(new unsigned char[myMaxDatagramSize],
							 std::default_delete<unsigned char[]>());

			myReceiveFromEndpoint =  std::make_shared<boost::asio::ip::udp::endpoint>();
			startReceive();
		      }
			  

		    std::cout << "BB begin: " << receiveFromEndpoint -> address().to_string() << ":" << receiveFromEndpoint -> port()
			      << std::endl << std::flush;

		    if (!error)
		      {

			std::cout << "BB2" << std::endl << std::flush;
			std::cout << "Got request, bytes: " << bytesTransferred << std::endl ;
			std::cout << "Thread: " << std::this_thread::get_id() << std::endl;

			if (myOnReceive) 
			  myOnReceive(recvBuffer, bytesTransferred, receiveFromEndpoint -> address().to_string(), receiveFromEndpoint -> port());			    
			  
		      }
		    else
		      {
			std::cout << "BB: whoops" << std::endl << std::flush;
			if (myOnReceiveError)
			  myOnReceiveError(recvBuffer, bytesTransferred, receiveFromEndpoint -> address().to_string(), receiveFromEndpoint -> port());			    
			      
		      }

			  
		    if (!myFastServerMode)
		      startReceive();
			  
		  } // End of handleReceive


		  void UdpServer::setOnReceiveCallback(UdpCallbackType callback)
		  {
		    myOnReceive = callback;
		  } // End of setOnReceiveCallback
		  
		  void UdpServer::setOnSendCallback(UdpCallbackType callback)
		  {
		    myOnSend = callback;
		  } // End of sendOnSendCallback

		  void UdpServer::send(std::shared_ptr<const unsigned char> data, int dataSize, const std::string &address, const std::string &service)
		  {
		    boost::asio::ip::udp::resolver resolver(*myIoc);
		    std::shared_ptr<boost::asio::ip::udp::endpoint> sendToEndpoint =
		      std::make_shared<boost::asio::ip::udp::endpoint>(*resolver.resolve(boost::asio::ip::udp::v4(), address, service).begin());

		    boost::asio::post(*myIoc, boost::asio::bind_executor(*myWriteStrand,
									 [this, data, dataSize, sendToEndpoint] ()
									 {
									   queueMessage(data, dataSize, sendToEndpoint);
									 }
									 ));

						      

		  } // End of send binary

		  void UdpServer::send(const void *data, int dataSize, const std::string &address, const std::string &service)
		  {
		    auto dataCache = std::shared_ptr<unsigned char>(new unsigned char[dataSize],
								    std::default_delete<unsigned char[]>());
		    std::memcpy(dataCache.get(), data, dataSize);

		    send(dataCache, dataSize, address, service);

		  }

		  void UdpServer::send(const std::string &msg, const std::string &address, const std::string &service)
		  {
		    int dataSize = msg.size() + 1;
		    auto dataCache = std::shared_ptr<unsigned char>(new unsigned char[dataSize],
								    std::default_delete<unsigned char[]>());
		    std::memcpy(dataCache.get(), msg.c_str(), dataSize);

		    send(dataCache, dataSize, address, service);

		  } // End of send string
		  
		  
		  void UdpServer::queueMessage(std::shared_ptr<const unsigned char> data, int dataSize, std::shared_ptr<boost::asio::ip::udp::endpoint> sendToEndpoint)
		  {
		    bool write_in_progress = !mySendPacketQueue.empty();
		    mySendPacketQueue.push_back(std::make_tuple(data, dataSize, sendToEndpoint));

		    if (! write_in_progress)
		      startPacketSend();
		  } // End of queueMessage


		  void UdpServer::startPacketSend()
		  {
		    auto dataPtr = std::get<0>(mySendPacketQueue.front());
		    auto dataSize = std::get<1>(mySendPacketQueue.front());
		    auto sendToEndpointPtr = std::get<2>(mySendPacketQueue.front());

		    mySocket -> async_send_to(boost::asio::buffer(dataPtr.get(), dataSize), *sendToEndpointPtr,
					      boost::asio::bind_executor(*myWriteStrand,
									 boost::bind(&UdpServer::handleSendDone, this,
										     dataPtr, dataSize,
										     boost::asio::placeholders::error,
										     boost::asio::placeholders::bytes_transferred,
										     sendToEndpointPtr
										     )
									 )
					      );
		  } // End of startPacketSend
		  
		    
		  void UdpServer::handleSendDone(std::shared_ptr<const unsigned char> data, int dataSize,
						 const boost::system::error_code& error,
						 std::size_t bytesTransferred,
						 std::shared_ptr<boost::asio::ip::udp::endpoint> sendToEndpoint
						 )
		  {
		    if (!error)
		      {

			if (myOnSend)
			  myOnSend(data, bytesTransferred, sendToEndpoint -> address().to_string(), sendToEndpoint -> port());			    
			
			mySendPacketQueue.pop_front();
			if (!mySendPacketQueue.empty()) { startPacketSend(); }
		      }
		    else
		      {
			if (myOnSendError)
			  myOnSendError(data, bytesTransferred, sendToEndpoint -> address().to_string(), sendToEndpoint -> port());			    

		      }
		  } // End of handleSendDone
		   
		}
	}
}

