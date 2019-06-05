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
					
				while (_activeContexts.size())
				{
					auto contextInfo = _activeContexts.begin();
					DELETEOBJ(*contextInfo);
					_activeContexts.erase(contextInfo);
				}

				while (_inactiveContexts.size())
				{
					auto contextInfo = _inactiveContexts.begin();
					DELETEOBJ(*contextInfo);
					_inactiveContexts.erase(contextInfo);
				}
			}
			
			bool NetworkManager::run_one()
			{
				ContextsInfo contextsToProcess = _activeContexts;
				
				for(auto contextInfo : contextsToProcess)
				{
					const bool contextStopped = contextInfo->_context.stopped();
					
					if (contextStopped)
					{
						if (contextInfo->_requireRestart)
						{
							contextInfo->_context.restart();
							contextInfo->_requireRestart = false;
						}
						else
						{
							auto stoppedContext = std::find(_activeContexts.begin(), _activeContexts.end(), contextInfo);
							if (stoppedContext != _activeContexts.end())
							{
								_inactiveContexts.push_back(*stoppedContext);
								_activeContexts.erase(stoppedContext);
							}
							
							continue;
						}
					}
					
					// Process outstanding operations
					try
					{
						contextInfo->_context.poll_one();
					}
					catch (boost::system::system_error error)
					{
						// Error during context execution
					}
				}
				
				return !_activeContexts.empty();
			}
			
			boost::asio::io_context& NetworkManager::getContext()
			{
				// Check if we run out of maximum allowed threads
				std::size_t contextsSize = _activeContexts.size() + _inactiveContexts.size();
				if (contextsSize < MaxNetworkThreads)
				{
					auto newContext = RJNEW ContextInfo();
					
					_inactiveContexts.push_back(newContext);
					
					return newContext->_context;
				}

				ContextInfo* relaxedContext = nullptr;
				
				if (!_inactiveContexts.empty())
				{
					auto ctx = std::min_element(_inactiveContexts.begin(),
												_inactiveContexts.end(),
												[](ContextInfo* a, ContextInfo* b)-> bool {
													return a->_jobs < b->_jobs;
												});
					relaxedContext = *ctx;
				}
				else
				{
					auto ctx = std::min_element(_activeContexts.begin(),
												_activeContexts.end(),
												[](ContextInfo* a, ContextInfo* b)-> bool {
													return a->_jobs < b->_jobs;
												});
					relaxedContext = *ctx;
				}
				
				return relaxedContext->_context;
			}
			
			void NetworkManager::activateContext(boost::asio::io_context& context)
			{
				auto inactiveContext = std::find_if(_inactiveContexts.begin(), _inactiveContexts.end(), [&](ContextInfo* ctx)->bool {
					return &context == &ctx->_context;
				});
				
				if (inactiveContext != _inactiveContexts.end())
				{
					if ((*inactiveContext)->_context.stopped())
						(*inactiveContext)->_requireRestart = true;
					
					_activeContexts.push_back(*inactiveContext);
					_inactiveContexts.erase(inactiveContext);
					return;
				}
				
				auto activeContext = std::find_if(_activeContexts.begin(), _activeContexts.end(), [&](ContextInfo* ctx)->bool {
					return &context == &ctx->_context;
				});
				
				if (activeContext != _activeContexts.end())
				{
					(*activeContext)->_jobs++;
				}
			}

			void NetworkManager::contextReleased(boost::asio::io_context& context)
			{
				auto inactiveContext = std::find_if(_inactiveContexts.begin(),
													_inactiveContexts.end(),
													[&](ContextInfo* ctx)->bool {
														return &context == &ctx->_context;
													});

				if (inactiveContext != _inactiveContexts.end())
				{
					unsigned int& jobs = (*inactiveContext)->_jobs;
					
					if (jobs > 0)
						jobs--;
					
					return;
				}

				auto activeContext = std::find_if(_activeContexts.begin(),
												  _activeContexts.end(),
												  [&](ContextInfo* ctx)->bool {
													  return &context == &ctx->_context;
												  });

				if (activeContext != _activeContexts.end())
				{
					unsigned int& jobs = (*activeContext)->_jobs;
					
					if (jobs > 0)
						jobs--;
				}
			}
		}
	}
}
