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
#include "jscore/RadJavJSCExternalsDetails.h"
#include "cpp/RadJavCPPChainedPtr.h"
#include "RadJav.h"
#include "jscore/RadJavJSCJavascriptEngine.h"

#ifdef USE_JAVASCRIPTCORE

namespace RadJAV
{
	FieldWrapper::FieldWrapper(uint32_t objectId, const std::string& functionName, WrapperType wrapperType)
	: objectUniqueId(objectId),
	  fieldName(functionName),
	  type(wrapperType)
	{}
	
	FieldWrapper::~FieldWrapper()
	{
		// Notify user when wrapper goes out of scope
		if(aboutDelete)
			aboutDelete(this);
	}
	
	uint32_t FieldWrapper::objectId() const
	{
		return objectUniqueId;
	}

	const std::string& FieldWrapper::functionName() const
	{
		return fieldName;
	}

	void FieldWrapper::onDelete(std::function<void (FieldWrapper*)> aboutToDelete)
	{
		aboutDelete = aboutToDelete;
	}
	
	FieldWrapper::WrapperType FieldWrapper::getType() const
	{
		return type;
	}
	
	void FieldWrapper::wrap()
	{
	}
	
	//void FieldWrapper::weakCallback (const v8::WeakCallbackInfo<FieldWrapper> &data)
	//{
	//	FieldWrapper *wrapper = data.GetParameter();
	//	DELETEOBJ(wrapper);
	//}
	
	ChainedPtrWrapper::ChainedPtrWrapper (const JSObject &handle,
										  const std::string& functionName,
										  uint32_t objectId,
										  CPP::ChainedPtr *data)
	: FieldWrapper(objectId, functionName, FieldWrapper::WrapperType::ChainedPtr),
	  object(data)
	{
		wrap();
	}
	
	ChainedPtrWrapper::~ChainedPtrWrapper ()
	{
		// Remove hooked raw pointer if not removed
		// by GC callback or CPP side
		if (objectHook)
		{
			DELETEOBJ(objectHook);
		}
	}
	
	CPP::ChainedPtr* ChainedPtrWrapper::objectPtr()
	{
		return object;
	}
	
	void ChainedPtrWrapper::wrap ()
	{
		FieldWrapper::wrap();
		
		// Hook on raw pointer to be notified when it is destroyed
		objectHook = RJNEW CPP::ChainedPtrHook(object, [&]
											   {
												   objectDestroyed();
											   });
	}
	
	void ChainedPtrWrapper::objectDestroyed()
	{
		// Object to which we subscribed going out of scope
		objectHook = nullptr;
		object = nullptr;
		
		// We did not clear Javascript object in that case, assume that
		// we still be able to receive garbage collector callbacks
		// for that object. Other stuff will be cleared in
		// our destructor.
	}

	ExternalsManagerJscImpl::ExternalsManagerJscImpl()
	{}
	
	ExternalsManagerJscImpl::~ExternalsManagerJscImpl()
	{
		while (!externals.empty())
		{
			// Remove objects which were not cleared by Javascript garbage collector
			auto item = --externals.end();
			while( !(*item).second.empty())
			{
				auto wrapperIter = --(*item).second.end();
				FieldWrapper* wrapper = wrapperIter->second;
				(*item).second.erase(wrapperIter);
				DELETEOBJ(wrapper);
			}
			externals.erase(item);
		}
	}
	
	void ExternalsManagerJscImpl::set(const JSObject& handle, const String& functionName, CPP::ChainedPtr* object)
	{
		LOCK_GUARD(s_mutexExternalsAccess);
		
		uint32_t objectId = getObjectId(handle, functionName);
		if(!objectId)
		{
			//We never exposed any C++ objects to this Javascript object before
			//Lets select new ID for this Javascript object
			objectId = nextId();
		}
		
		ChainedPtrWrapper* wrapper = RJNEW ChainedPtrWrapper(handle, functionName, objectId, object);
		wrapper->onDelete( [&](FieldWrapper* wrapper)
						  {
							  LOCK_GUARD(s_mutexExternalsAccess);

							  auto mapPos = externals.find( wrapper->objectId());
							  if(mapPos != externals.end())
							  {
								  // Remove wrapped object from the list of external objects
								  auto wrapperPos = (*mapPos).second.find( wrapper->functionName());
								  if(wrapperPos != (*mapPos).second.end())
								  {
									  (*mapPos).second.erase(wrapperPos);
								  }
								  
								  if( (*mapPos).second.empty())
									  externals.erase(mapPos);
							  }
						  });
		
		// Put wrapped object into the list of external objects
		externals[objectId][functionName] = wrapper;
	}
	
	CPP::ChainedPtr* ExternalsManagerJscImpl::get(const JSObject& handle, const String& functionName)
	{
		LOCK_GUARD(s_mutexExternalsAccess);

		FieldWrapper* wrapper = getWrapper(handle, functionName);
		
		if(wrapper &&
		   wrapper->getType() == FieldWrapper::WrapperType::ChainedPtr)
		{
			ChainedPtrWrapper* chainedPtrWrapper = dynamic_cast<ChainedPtrWrapper*>(wrapper);
			return chainedPtrWrapper->objectPtr();
		}
		
		return nullptr;
	}
	
	void ExternalsManagerJscImpl::clear(const JSObject& handle, const String& functionName)
	{
		LOCK_GUARD(s_mutexExternalsAccess);

		FieldWrapper* wrapper = getWrapper(handle, functionName);
		
		//In case of JavaScriptCore where we didn't receive callback per each exposed member variable we delete Wrapper
		//which in turn may delete underlying C++ object if not already
		DELETEOBJ(wrapper);
		
		//Corresponding map entry will be cleared by Wrapper itself during destruction
	}
	
	uint32_t ExternalsManagerJscImpl::getObjectId(const JSObject& handle, const String& functionName)
	{
		uint32_t* objectId = static_cast<uint32_t*>(JSObjectGetPrivate(handle));
		
		return objectId ? *objectId : 0;
	}
	
	FieldWrapper* ExternalsManagerJscImpl::getWrapper(const JSObject& handle, const String& functionName)
	{
		uint32_t objectId = getObjectId(handle, functionName);
		
		if(!objectId)
			return nullptr;
		
		auto pos = externals.find( objectId);
		
		if(pos == externals.end())
			return nullptr;
		
		auto wrapperPos = (*pos).second.find(functionName);
		if(wrapperPos == (*pos).second.end())
			return nullptr;

		return wrapperPos->second;
	}

	uint32_t ExternalsManagerJscImpl::nextId()
	{
		static uint32_t objectId = 0;
		
		return ++objectId;
	}
}

#endif

