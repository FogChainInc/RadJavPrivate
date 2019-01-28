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
#include "cpp/RadJavCPPNetUdpClient.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
		        UdpClient::UdpClient(std::map<std::string, std::string> parms)
			{
			  mySocket = std::make_shared<boost::asio::ip::udp::socket>(myIoc);
			  mySocket -> open(boost::asio::ip::udp::v4());

			  
			  myMaxDatagramSize = 2048;
			  myRecvBuffer = 
			    std::shared_ptr<unsigned char>(new unsigned char[myMaxDatagramSize],
							   std::default_delete<unsigned char[]>());
			  
			}

			UdpClient::~UdpClient()
			{
			  close();
			}

		  

		        void UdpClient::connect(std::string host, std::string service)
			{
			  //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;

			  boost::asio::ip::udp::resolver resolver{myIoc};
			  boost::asio::ip::udp::endpoint remoteEndpoint =
			    *resolver.resolve(boost::asio::ip::udp::v4(), host, service).begin();
			  
			  mySocket -> connect(remoteEndpoint);
			}

		        void UdpClient::send(std::string message)
			{
			  //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  send(message.c_str(), message.size());
			}

		  
		        void UdpClient::send(const void *message, int msgLen)
			{
			  //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  boost::asio::ip::udp::resolver resolver{myIoc};

			  mySocket -> send(boost::asio::buffer(message, msgLen));

			  
			}
		  
		        void UdpClient::send(std::string message, std::string host, std::string service)
			{
			  //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  send(message.c_str(), message.size(), host, service);
			}

		  
		        void UdpClient::send(const void *message, int msgLen, std::string host, std::string service)
			{
			  //std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  boost::asio::ip::udp::resolver resolver{myIoc};

			  boost::asio::ip::udp::endpoint remoteEndpoint =
			    *resolver.resolve(boost::asio::ip::udp::v4(), host, service).begin();

			  mySocket -> send_to(boost::asio::buffer(message, msgLen), remoteEndpoint);
			}

		        void UdpClient::receive(std::function <void (const void *buf, int bufLen)> binSetter,
						std::string host, std::string service)
			{

			  std::cout << "RECEIVING from " << host << ':' << service << std::endl;
			   
			  boost::asio::ip::udp::resolver resolver{myIoc};
			  boost::asio::ip::udp::endpoint senderEndpoint =
			    *resolver.resolve(boost::asio::ip::udp::v4(), host, service).begin();

			  size_t len = mySocket -> receive_from(boost::asio::buffer(myRecvBuffer.get(), myMaxDatagramSize), senderEndpoint);

			  std::cout << "REceived " << len << " bytes" << std::endl;

			  binSetter(myRecvBuffer.get(), len);
			}

		        void UdpClient::receive(std::function <void (const void *buf, int bufLen)> binSetter)
			{
			  size_t len = mySocket -> receive(boost::asio::buffer(myRecvBuffer.get(), myMaxDatagramSize));

			  std::cout << "REceived " << len << " bytes" << std::endl;

			  binSetter(myRecvBuffer.get(), len);
			 }
		  
			void UdpClient::close()
			{
			  mySocket -> close();
			}
		}
	}
}

