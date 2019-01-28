/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#ifndef _RADJAV_GUI_V8_NET_UDPSERVER_H_
	#define _RADJAV_GUI_V8_NET_UDPSERVER_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"
	#include "cpp/RadJavCPPChainedPtr.h"

	namespace RadJAV
	{
		namespace V8B
		{
			namespace Net
			{
				/** @class UdpServer
				 * 
				 * @brief The class embeds UdpServer functionality into JavaScript engine.
				 */				  
				class RADJAV_EXPORT UdpServer : public CPP::ChainedPtr
				{
					public:

				                /** 
						 * @brief Invoked during initialization, creates V8 callbacks.
						 */
						static void createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object);

				                /** 
						 * @brief Implements javascript constructor.
						 */
						static void _init(const v8::FunctionCallbackInfo<v8::Value> &args);
						
				                /** 
						 * @brief Implements javascript send.
						 *
						 * @param message A string or ArrayBuffer message.
						 * @param host A string representing host or IP4 address.
						 * @param service A string representing service name or port number.
						 */
						static void send(const v8::FunctionCallbackInfo<v8::Value> &args);

				                /** 
						 * @brief Implements javascript listen.
						 *
						 * @param host A string representing host or IP4 address.
						 * @param service A string representing service name or port number.
						 *
						 * After calling this method, the object is ready to accept datagrams.
						 */
						static void listen(const v8::FunctionCallbackInfo<v8::Value> &args);

						
				                /** 
						 * @brief Implements javascript close.
						 *
						 * The method closes the socket and cleans resources.
						 */
						static void close(const v8::FunctionCallbackInfo<v8::Value> &args);

				                /** 
						 * @brief Implements javascript onReceive
						 *
						 * @param Javascript callback function accepting the datagram.
						 */
						static void onReceive(const v8::FunctionCallbackInfo<v8::Value> &args);

						
				                /** 
						 * @brief Does nothing for now.
						 */
						static void on(const v8::FunctionCallbackInfo<v8::Value> &args);
				};

			}
		}
	}
#endif

