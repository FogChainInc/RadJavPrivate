/*
 MIT-LICENSE
 Copyright (c) 2018 Higher Edge Software, LLC
 
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
#ifndef _RADJAV_CPP_NET_NETWORK_MANAGER_H_
#define _RADJAV_CPP_NET_NETWORK_MANAGER_H_

#include "RadJavPreprocessor.h"

#include <boost/asio/io_context.hpp>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			/**
			 * @brief Network manager
			 * @details Handle ASIO contexts. When a new socket created by the user it will query for the context to use.
			 * NetworkManager will run contexts and try to balance load on them. User need to call run_one() in each main loop step
			 * to make network routines work properly. When application is shutting down NetworkManager will stop when there is no
			 * out-standing asynchronous operations exists this means that it will run until all contexts will finish their jobs.
			 */
			class NetworkManager
			{
			public:
				/**
				 * @brief A constructor
				 */
				NetworkManager();

				/**
				 * @brief A destructor
				 * @details Will return only when all contexts will finish their jobs (f.e. all sockets has been closed properly)
				 */
				~NetworkManager();
				
				/**
				 * @brief Process network messages
				 * @details Main function to make network work by executing ASIO contexts
				 * @return true if there is still work to do, false otherwise
				 * @note Regardless whether it return true or false still need to call it in main application loop because
				 * new operations maybe be added by instantiating one of the sockets
				 */
				bool run_one();
				
				/**
				 * @brief Get ASIO context for specific connection
				 * @details Usually this API used by the sockets and user must not call it directly. NetworkManager will try to
				 * balance load on contexts and provide most relaxed context
				 * @return boost::asio::io_context& reference to ASIO context which can be used for socket creation
				 */
				boost::asio::io_context& getContext();
				
				/**
				 * @brief Notify NetworkManager on released context
				 * @details This is a hint for to do a better job while providing new context for the sockets
				 * @param[in] context reference to ASIO context which has been released
				 * @note Calling this function does not mean that specific context released fully
				 * f.e. socket which call this function will indicate that all work is done and socket is in closing state
				 * which it turn still require some processing (running a context). Context can be reused by NetworkManager to
				 * provide it to a newly created connection(socket)
				 */
				void contextReleased(boost::asio::io_context& context);
				
			private:
				class ContextInfo
				{
				public:
					bool _active;
					unsigned int _jobs;
					boost::asio::io_context _context;
				};
				
				using ContextsInfo = std::vector<ContextInfo*>;
				
				ContextsInfo _contextsInfo;
			};
		}
	}
}
#endif
