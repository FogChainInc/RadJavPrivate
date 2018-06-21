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
#include "cpp/RadJavCPPChainedPtr.h"

namespace RadJAV
{
	namespace CPP
	{
		ChainedPtr::ChainedPtr()
		{}
		
		ChainedPtr::~ChainedPtr()
		{
			// If during destruction we still have objects
			// with which we linked need to destroy them
			while(toDelete.size())
			{
				auto item = toDelete.begin();
				toDelete.erase(item);
				
				(*item)->unlink(this);
				delete (*item);
			}
			
			// If during destruction we still have object
			// which linked with us need to destroy them
			while(linkedWith.size())
			{
				auto linked = linkedWith.begin();
				linkedWith.erase(linked);
				
				// Exclude us from parent so it will not destroy
				// us again in it's destructor
				(*linked)->exclude(this);
				delete (*linked);
			}
		}
		
		void ChainedPtr::linkWith(ChainedPtr* other)
		{
			other->link(this);
			toDelete.push_back(other);
		}
		
		void ChainedPtr::unlinkWith(ChainedPtr* other)
		{
			other->unlink(this);
			std::remove( toDelete.begin(), toDelete.end(), other);
		}
		
		void ChainedPtr::link(ChainedPtr* other)
		{
			if(other == nullptr)
				return;
			
			if( std::find(linkedWith.begin(), linkedWith.end(), other) == linkedWith.end())
			{
				linkedWith.push_back(other);
			}
		}
		
		void ChainedPtr::unlink(ChainedPtr* other)
		{
			if(other == nullptr)
				return;
			
			auto item = std::find(linkedWith.begin(), linkedWith.end(), other);
			if( item != linkedWith.end())
			{
				linkedWith.erase(item);
			}
		}
		
		void ChainedPtr::exclude(ChainedPtr* other)
		{
			if(other == nullptr)
				return;
			
			auto item = std::find(toDelete.begin(), toDelete.end(), other);
			if( item != toDelete.end())
			{
				toDelete.erase(item);
			}
		}
		
		ChainedPtrHook::ChainedPtrHook(ChainedPtr* target, std::function<void ()> aboutToDelete)
		: aboutDelete(aboutToDelete),
		  hookedObject(target)
		{
			linkWith(target);
		}

		ChainedPtrHook::~ChainedPtrHook()
		{
			if(aboutDelete)
				aboutDelete();
		}
		
		ChainedPtr* ChainedPtrHook::object() const
		{
			return hookedObject;
		}
	}
}
