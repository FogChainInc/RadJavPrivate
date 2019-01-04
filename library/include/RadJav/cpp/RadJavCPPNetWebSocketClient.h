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
#ifndef _RADJAV_CPP_NET_WEBSOCKETCLIENT_H_
#define _RADJAV_CPP_NET_WEBSOCKETCLIENT_H_

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
			 * @ingroup group_net_cpp
			 * @brief WebSocketClient class.
			 */
			class RADJAV_EXPORT WebSocketClient : public ChainedPtr
			{
				public:
					WebSocketClient();

					#ifdef USE_V8
					static void on(String event_, v8::Local<v8::Function> func_);
					#elif defined USE_JAVASCRIPTCORE
					static void on(String event_, JSObjectRef func_);
					#endif

					void connect(String host_, String port_);

					void send(String message_);

					String receive();

					void close();

				private:
					//the io_context is required for all I/O
					boost::asio::io_context m_ioc;

					//these objects perform our I/O
					boost::asio::ip::tcp::resolver resolver{ m_ioc };
					boost::beast::websocket::stream<boost::asio::ip::tcp::socket> m_ws{ m_ioc };

					String m_received_message;
			};
		}
	}
}
#endif

