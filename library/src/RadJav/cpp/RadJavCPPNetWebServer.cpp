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

#include "cpp/RadJavCPPNetWebServer.h"

//TODO: resolve issue with boost conflicts and remove this proxy
#include "cpp/boost_server.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			WebServer::WebServer()
			{
				port = 80;
				_serverType = WebServerTypes::HTTP;
				this->ptr = static_cast<void*> (new RadJAV::CPP::Net::BoostServer(port, _serverType));
				//listen();
				//serve();
				//stop();
			}

			void WebServer::listen()
			{
				static_cast<RadJAV::CPP::Net::BoostServer*>(this->ptr)->listen();
			}

			void WebServer::serve()
			{
				static_cast<RadJAV::CPP::Net::BoostServer*>(this->ptr)->serve();
			}

			void WebServer::stop()
			{
				static_cast<RadJAV::CPP::Net::BoostServer*>(this->ptr)->stop();
			}
			WebServer::~WebServer()
			{
				delete static_cast<RadJAV::CPP::Net::BoostServer*>(this->ptr);
			}
		}
	}
}

