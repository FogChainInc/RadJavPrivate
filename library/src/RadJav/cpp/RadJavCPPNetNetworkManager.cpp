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

#include "cpp/RadJavCPPNetNetworkManager.h"

#include <algorithm>

namespace RadJAV
{
	namespace CPP
	{
		namespace Net
		{
			namespace
			{
				// We can make it configurable from build script
				///Maximum threads(contexts) to process sockets
				const int MaxNetworkThreads = 10;
			}
			
			NetworkManager::NetworkManager()
			{
			}
			
			NetworkManager::~NetworkManager()
			{
				//Wait until all operations has been finished
				while(run_one());
					
				while (_contextsInfo.size())
				{
					auto contextInfo = _contextsInfo.begin();
					DELETEOBJ(*contextInfo);
					_contextsInfo.erase(contextInfo);
				}
			}
			
			bool NetworkManager::run_one()
			{
				bool requireMoreProcessing = false;
				
				for(auto contextInfo: _contextsInfo)
				{
					const bool contextStopped = contextInfo->_context.stopped();
					const bool contextActive = contextInfo->_active;

					if (contextStopped && !contextActive)
						continue;
						
					if (contextStopped && contextActive)
						contextInfo->_context.restart();
					
					// Process outstanding operations
					try
					{
						const bool hasPendingOperations = contextInfo->_context.run_one() > 0;
						if (!hasPendingOperations)
						{
							contextInfo->_active = false;
							contextInfo->_jobs = 0;
						}
						
						requireMoreProcessing |= hasPendingOperations;
					}
					catch (boost::system::system_error error)
					{
						// Error during context execution
					}
				}
				
				return requireMoreProcessing;
			}
			
			boost::asio::io_context& NetworkManager::getContext()
			{
				// Check if we run out of maximum allowed threads
				if (_contextsInfo.size() >= MaxNetworkThreads)
				{
					auto relaxedContext = std::min_element(_contextsInfo.begin(),
														   _contextsInfo.end(),
														   [](ContextInfo* a, ContextInfo* b)-> bool {
															   return a->_jobs < b->_jobs;
														   });
					
					(*relaxedContext)->_active = true;
					(*relaxedContext)->_jobs++;

					return (*relaxedContext)->_context;
				}
				
				auto contextInfo = RJNEW ContextInfo();
				contextInfo->_active = true;
				contextInfo->_jobs = 1;

				_contextsInfo.push_back(contextInfo);
				
				return contextInfo->_context;
			}
			
			void NetworkManager::contextReleased(boost::asio::io_context& context)
			{
				auto contextInfo = std::find_if(_contextsInfo.begin(), _contextsInfo.end(), [&](ContextInfo* ctx)->bool {
					return &context == &ctx->_context;
				});
				
				if (contextInfo != _contextsInfo.end())
				{
					unsigned int& jobs = (*contextInfo)->_jobs;
					
					if (jobs > 0)
						jobs--;
				}
			}
		}
	}
}
