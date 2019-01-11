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
#include "cpp/RadJavCPPNetTcpClient.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
		        TcpClient::TcpClient(std::map<std::string, std::string> parms)
			{
			  mySocket = std::make_shared<boost::asio::ip::tcp::socket>(myIoc);
			  mySocket -> open(boost::asio::ip::tcp::v4());

			  
			  myBufferSize = 2048;
			  myRecvBuffer = 
			    std::shared_ptr<unsigned char>(new unsigned char[myBufferSize],
							   std::default_delete<unsigned char[]>());
			  
			}

			TcpClient::~TcpClient()
			{
			  close();
			}

		  

		        void TcpClient::connect(std::string host, std::string service)
			{
			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;

			  boost::asio::ip::tcp::resolver resolver{myIoc};
			  boost::asio::ip::tcp::endpoint remoteEndpoint =
			    *resolver.resolve(boost::asio::ip::tcp::v4(), host, service).begin();
			  
			  mySocket -> connect(remoteEndpoint);
			}

		        void TcpClient::send(std::string message)
			{
			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  send(message.c_str(), message.size());
			}

		  
		        void TcpClient::send(const void *message, int msgLen)
			{
			  std::cout << __PRETTY_FUNCTION__ << std::endl << std::flush;
			  boost::asio::ip::tcp::resolver resolver{myIoc};


			  std::cout << "S2" << std::endl << std::flush;
			  mySocket -> send(boost::asio::buffer(message, msgLen));
			  std::cout << "S3" << std::endl << std::flush;
			  
			}
		  

		        void TcpClient::receive(std::function <void (const void *buf, int bufLen)> binSetter)
			{
			  size_t len = mySocket -> receive(boost::asio::buffer(myRecvBuffer.get(), myBufferSize));

			  std::cout << "REceived " << len << " bytes" << std::endl;

			  binSetter(myRecvBuffer.get(), len);
			 }
		  
			void TcpClient::close()
			{
			  mySocket -> close();
			}
		}
	}
}

