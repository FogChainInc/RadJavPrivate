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
	FieldWrapper::FieldWrapper(uint32_t objectId,
							   JSContextRef context,
							   JSObjectRef handle,
							   const String& functionName,
							   WrapperType wrapperType)
	: objectUniqueId(objectId),
	  virtualMachineContext(context),
	  javascriptObject(handle),
	  propertyName(functionName),
	  type(wrapperType)
	{}
	
	FieldWrapper::~FieldWrapper()
	{
		// Notify user when wrapper goes out of scope
		if(aboutDelete)
			aboutDelete(this);

		//Clear private data from external property
		JSObjectSetPrivate(externalJavascriptObject, nullptr);
	}
	
	uint32_t FieldWrapper::objectId() const
	{
		return objectUniqueId;
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
		//Define External class
		JSClassRef externalClassRef;
		
		JSClassDefinition externalClassDef = kJSClassDefinitionEmpty;
		externalClassDef.className = "External";
		externalClassDef.attributes = kJSClassAttributeNone;
		externalClassDef.finalize = finalizeCallback;
		
		externalClassRef = JSClassCreate(&externalClassDef);

		//Create new External object with private data pointed to this wrapper object
		externalJavascriptObject = JSObjectMake(virtualMachineContext, externalClassRef, this);
		JSClassRelease(externalClassRef);
		
		//Assign External class object to requested property
		JSStringRef property = propertyName.toJSCString();
		JSObjectSetProperty(virtualMachineContext, javascriptObject, property, externalJavascriptObject, kJSPropertyAttributeNone, nullptr);
		JSStringRelease(property);
	}
	
	void FieldWrapper::finalizeCallback (JSObjectRef jsObject)
	{
		FieldWrapper *wrapper = static_cast<FieldWrapper*>(JSObjectGetPrivate(jsObject));
		DELETEOBJ(wrapper);
	}
	
	ChainedPtrWrapper::ChainedPtrWrapper (uint32_t objectId,
										  JSContextRef context,
										  JSObjectRef handle,
										  const String& functionName,
										  CPP::ChainedPtr *data)
	: FieldWrapper(objectId, context, handle, functionName, FieldWrapper::WrapperType::ChainedPtr),
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
		// Export new external property object with finalize callback
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
		
		// We did not clear external property in that case, assume that
		// we still be able to receive garbage collector finalize callback
		// for that object. External property in that case will be cleared in
		// our destructor.
	}

	ExternalsManagerJscImpl::ExternalsManagerJscImpl()
	{}
	
	ExternalsManagerJscImpl::~ExternalsManagerJscImpl()
	{
		while (!externals.empty())
		{
			// Remove objects which were not cleared by Javascript engine's garbage collector
			auto item = --externals.end();
			FieldWrapper* wrapper = item->second;
			externals.erase(item);
			DELETEOBJ(wrapper);
		}
	}
	
	void ExternalsManagerJscImpl::set(JSContextRef context, JSObjectRef handle, const String& functionName, CPP::ChainedPtr* object)
	{
		LOCK_GUARD(s_mutexExternalsAccess);
		
		uint32_t objectId = nextId();
		
		ChainedPtrWrapper* wrapper = RJNEW ChainedPtrWrapper(objectId, context, handle, functionName, object);
		wrapper->onDelete( [&](FieldWrapper* wrapper)
						  {
							  LOCK_GUARD(s_mutexExternalsAccess);
							  
							  auto pos = externals.find( wrapper->objectId());
							  if(pos != externals.end())
							  {
								  // Remove wrapped object from the list of external objects
								  externals.erase(pos);
							  }
						  });
		
		// Put wrapped object into the list of external objects
		externals[objectId] = wrapper;
	}
	
	CPP::ChainedPtr* ExternalsManagerJscImpl::get(JSContextRef context, JSObjectRef handle, const String& functionName)
	{
		LOCK_GUARD(s_mutexExternalsAccess);

		FieldWrapper* wrapper = getWrapper(context, handle, functionName);
		
		if (wrapper &&
			wrapper->getType() == FieldWrapper::WrapperType::ChainedPtr)
		{
			ChainedPtrWrapper* chainedPtrWrapper = static_cast<ChainedPtrWrapper*>(wrapper);
			return chainedPtrWrapper->objectPtr();
		}
		
		return nullptr;
	}
	
	void ExternalsManagerJscImpl::clear(JSContextRef context, JSObjectRef handle, const String& functionName)
	{
		LOCK_GUARD(s_mutexExternalsAccess);

		FieldWrapper* wrapper = getWrapper(context, handle, functionName);
		
		if (wrapper)
		{
			DELETEOBJ(wrapper);
		}
		
		JSObjectSetPrivate(handle, nullptr);
	}
	
	FieldWrapper* ExternalsManagerJscImpl::getWrapper(JSContextRef context, JSObjectRef handle, const String& functionName)
	{
		JSStringRef property = functionName.toJSCString();
		JSValueRef propertyValue = JSObjectGetProperty(context, handle, property, nullptr);
		JSStringRelease(property);
		
		if (JSValueIsUndefined(context, propertyValue))
			return nullptr;
		
		if (JSValueIsObject(context, propertyValue))
		{
			JSObjectRef propertyObject = JSValueToObject(context, propertyValue, nullptr);
			if (!propertyObject)
				return nullptr;
			
			return static_cast<FieldWrapper*>(JSObjectGetPrivate(propertyObject));
		}
		
		return nullptr;
	}
	
	uint32_t ExternalsManagerJscImpl::nextId()
	{
		static uint32_t objectId = 0;
		
		return ++objectId;
	}
}

#endif

