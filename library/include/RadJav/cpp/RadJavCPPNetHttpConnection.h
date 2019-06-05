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
#ifndef _RADJAV_CPP_NET_HTTPCONNECTION_H_
#define _RADJAV_CPP_NET_HTTPCONNECTION_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"
#include "cpp/RadJavCPPEvent.h"

#include <boost/beast/http.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <functional>
#include <memory>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			class HttpConnectionImpl;
			class WebSocketConnection;
			
			class HttpConnection : public Events
			{
				friend WebSocketConnection;
			public:
				//TODO: make HttpConnection act like an HttpRequest
				
				HttpConnection(ContextManager& contextManager,
							   boost::asio::ip::tcp::socket&& socket,
							   int timeoutInMilliseconds = 5000);

				~HttpConnection();
				
				void close();
				
				void onRequest(const std::function<void(HttpConnection&,
														const boost::beast::http::request<boost::beast::http::string_body>&,
														boost::beast::http::response<boost::beast::http::string_body>&)>& func);
				
				void onUpgrade(const std::function<void(HttpConnection&,
														const boost::beast::http::request<boost::beast::http::string_body>&)>& func);
				
				void onUpgraded(const std::function<void(HttpConnection&)>& func);
				void onClose(const std::function<void(HttpConnection&)>& func);
				void onError(const std::function<void(HttpConnection&,
													  int, const std::string&)>& func);
				
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					void on(String event, RJ_FUNC_TYPE func);
				#endif
				
				ContextManager& getContextManager();
				
				void clearCallbacks();
				
			private:
				void releaseImpl();
				
				void onRequestCallback(const boost::beast::http::request<boost::beast::http::string_body>& request,
									   boost::beast::http::response<boost::beast::http::string_body>& response);
				void onUpgradeCallback(const boost::beast::http::request<boost::beast::http::string_body>& request);
				void onUpgradedCallback();
				void onClosedCallback();
				void onErrorCallback(int errorCode, const std::string& description);
				
				void upgradeHandled();
				boost::asio::ip::tcp::socket* getSocket();
				
			private:
				std::function<void(HttpConnection&,
								   const boost::beast::http::request<boost::beast::http::string_body>&,
								   boost::beast::http::response<boost::beast::http::string_body>&)> _onRequest;
				
				std::function<void(HttpConnection&,
								   const boost::beast::http::request<boost::beast::http::string_body>&)> _onUpgrade;
				
				std::function<void(HttpConnection&)> _onUpgraded;
				std::function<void(HttpConnection&)> _onClose;
				std::function<void(HttpConnection&,
								   int, const std::string&)> _onError;
				
				ContextManager& _contextManager;
				std::weak_ptr<HttpConnectionImpl> _impl;
			};
		}
	}
}

#endif
