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
#include "cpp/RadJavCPPEvent.h"
#include "cpp/RadJavCPPChainedPtr.h"
#include "cpp/RadJavCPPNet.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			class WebSocketClientImpl;
			
			/**
			 * @ingroup group_net_cpp
			 * @brief WebSocketClient class.
			 */
			class WebSocketClient : public Events
			{
			public:
				/**
				 * @brief A constructor
				 * @details Constructor which intended to be used by Javascript engine
				 */
				#ifdef USE_V8
					WebSocketClient(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
				#elif defined USE_JAVASCRIPTCORE
					WebSocketClient(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[]);
				#endif
				
				/**
				 * @brief A constructor
				 * @details Construct new instance of WebSocketClient
				 * @param[in] networkManager the reference to NetworkManager which handle ASIO contexts
				 */
				WebSocketClient(NetworkManager& networkManager);
				
				/**
				 * @brief A destructor
				 */
				~WebSocketClient();
				
				/**
				 * @brief Connect to a remote host
				 * @details Connect to remote host using specific URI
				 * @param[in] uri an URI of the remote host
				 * @example ws://127.0.0.1:9898/ws
				 */
				void connect(const std::string& uri);

				/**
				 * @brief Connect to a remote host
				 * @details Connect to remote host using specific address, port and path
				 * @param[in] host remote host address or host name
				 * @param[in] port remote host port number
				 * @param[in] resource remote path. Example: "/"
				 */
				void connect(const std::string& host,
							 const std::string& port,
							 const std::string& resource);
				
				/**
				 * @brief Send text data to a remote host
				 * @details Before calling this function make sure that socket is connected to remote host
				 * @param[in] data the payload which will be transferred using text opcode
				 */
				void send(const std::string& data);

				/**
				 * @brief Send binary data to a remote host
				 * @details Before calling this function make sure that socket is connected to remote host
				 * @param[in] data the payload which will be transferred using binary opcode
				 * @param[in] size the size of the payload
				 */
				void send(const unsigned char* data, unsigned int size);
				
				/**
				 * @brief Close socket connection
				 * @details Before opening new connection using the same WebSocketClient object after calling this function
				 * you must wait for onClose, or onError callbacks
				 */
				void close();

				/**
				 * @brief Callback which indicate that socket connection has been sucessfully established
				 * @details Set the handler to be notified when connection is established
				 * @param[in] func pointer to handler function
				 */
				void onOpen(std::function<void(void)> func);

				/**
				 * @brief Callback which indicate that new message has been received from remote host
				 * @details Set the handler to be notified on new message from remote host
				 * @param[in] func pointer to handler function for text message
				 * @param[in] funcBinary pointer to handler function for binary message
				 * @note Only one callback will be called depending on opcode of received message
				 */
				void onMessage(std::function<void(const std::string&)> func,
							   std::function<void(const unsigned char*, std::size_t)> funcBinary);
				
				/**
				 * @brief Callback which indicate that socket has been closed
				 * @details Set the handler to be notified when socket closed. Initiator of this event is a call to close() function
				 * @param[in] func pointer to handler function
				 * @note Function will not be called if socket was closed by remote peer, in this case onError callback will be triggered
				 */
				void onClose(std::function<void(void)> func);

				/**
				 * @brief Callback which indicate that there was some socket error
				 * @details Set the handler to be notified when error occur. When handler is called the underlining socket already has been closed
				 * so user can try to connect again using the same object.
				 * @param[in] func pointer to handler function
				 * @note This function will also be called when remote host close the connection
				 */
				void onError(std::function<void(int, const std::string&)> func);

				/**
				 * @brief Register Javascript callback
				 * @details Set the Javascript handler function for specific notification
				 * @param[in] event name of the event. Can be "open", "message", "error" and "close"
				 * @param[in] func Javascript callback function
				 */
				#if defined USE_V8 || defined USE_JAVASCRIPTCORE
					void on(String event, RJ_FUNC_TYPE func);
				#endif

			private:
				void createImpl();
				void releaseImpl();

				void onConnectedCallback();
				void onReadCallback(const std::string& data);
				void onReadBinaryCallback(const unsigned char* data, std::size_t size);
				void onClosedCallback();
				void onErrorCallback(int errorCode, const std::string& description);

			private:
				std::function<void(void)> _onOpen;
				std::function<void(const std::string&)> _onMessage;
				std::function<void(const unsigned char*, std::size_t)> _onMessageBinary;
				std::function<void(void)> _onClose;
				std::function<void(int, const std::string&)> _onError;

				std::shared_ptr<WebSocketClientImpl> _impl;
				URI _uri;
				NetworkManager& _networkManager;
			};
		}
	}
}
#endif

