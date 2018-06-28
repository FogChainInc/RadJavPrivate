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
#include "v8/RadJavV8Externals.h"
#include "cpp/RadJavCPPChainedPtr.h"
#include "RadJav.h"
#include "v8/RadJavV8JavascriptEngine.h"

#ifdef USE_V8

namespace RadJAV
{
	FieldWrapper::FieldWrapper(uint32_t objectId, WrapperType wrapperType)
	: objectUniqueId(objectId),
	  type(wrapperType),
	  isolate(nullptr)
	{}
	
	FieldWrapper::~FieldWrapper()
	{
		// Notify user when wrapper goes out of scope
		if(aboutDelete)
			aboutDelete(this);
		
		// Clear Persistent handle
		if (!persistent.IsEmpty() &&
			persistent.IsNearDeath())
		{
			// Clear GC callback
			persistent.ClearWeak();
			
			// Clear V8 Persistent object
			persistent.Reset();
		}
		
		isolate = nullptr;
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
	
	void FieldWrapper::wrap(const v8::Local<v8::Object> &handle,
							const v8::Local<v8::Object>& objectInstance)
	{
		// Grab isolate pointer
		// We sure that it will be destroyed after this wrapper
		isolate = handle->GetIsolate();
		
		// Associate Persistent with object
		persistent.Reset(isolate, objectInstance);
		
		// Mark Persistent as weak to receive callback from garbage collector
		persistent.SetWeak(this, weakCallback, v8::WeakCallbackType::kParameter);
		persistent.MarkIndependent();
	}
	
	void FieldWrapper::weakCallback (const v8::WeakCallbackInfo<FieldWrapper> &data)
	{
		FieldWrapper *wrapper = data.GetParameter();
		DELETEOBJ(wrapper);
	}
	
	ChainedPtrWrapper::ChainedPtrWrapper (const v8::Local<v8::Object> &handle,
										  const v8::Local<v8::Object>& objectInstance,
										  uint32_t objectId,
										  CPP::ChainedPtr *data)
	: FieldWrapper(objectId, FieldWrapper::WrapperType::ChainedPtr),
	  object(data)
	{
		wrap(handle, objectInstance);
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
	
	void ChainedPtrWrapper::wrap (const v8::Local<v8::Object> &handle,
								  const v8::Local<v8::Object>& objectInstance)
	{
		// Init Persistent object and set weak callback
		FieldWrapper::wrap(handle, objectInstance);
		
		// Hook on raw pointer to be notified when it is destroyed
		objectHook = RJNEW CPP::ChainedPtrHook(object, [&]
											   {
												   objectDestroyed();
											   });
		
		// We know that this is ugly but we didn't have a better way for now
		isolate->AdjustAmountOfExternalAllocatedMemory( sizeof(object));
	}
	
	void ChainedPtrWrapper::objectDestroyed()
	{
		// We know that this is ugly but we didn't have a better way for now
		isolate->AdjustAmountOfExternalAllocatedMemory( -(int64_t)sizeof(object));

		// Object to which we subscribed going out of scope
		objectHook = nullptr;
		object = nullptr;
		
		// We did not clear persistent in that case, assume that
		// we still be able to receive garbage collector weak callbacks
		// for that object. Persistent in that case will be cleared in
		// our destructor.
	}

	ExternalsManager::ExternalsManager()
	{}
	
	ExternalsManager::~ExternalsManager()
	{
		while (!externals.empty())
		{
			// Remove objects which were not cleared by V8 garbage collector
			auto item = --externals.end();
			FieldWrapper* wrapper = item->second;
			externals.erase(item);
			DELETEOBJ(wrapper);
		}
	}
	
	void ExternalsManager::set(const v8::Local<v8::Object>& handle, const String& functionName, CPP::ChainedPtr* object)
	{
		uint32_t objectId = nextId();
		v8::Local<v8::Object> objectInstance = newObjectInstance(handle);
		
		v8::Local<v8::Integer> val = v8::Uint32::NewFromUnsigned(handle->GetIsolate(), objectId);
		objectInstance->SetInternalField(0, val);
		
		ChainedPtrWrapper* wrapper = RJNEW ChainedPtrWrapper(handle, objectInstance, objectId, object);
		wrapper->onDelete( [&](FieldWrapper* wrapper)
						  {
							  auto pos = externals.find( wrapper->objectId());
							  if(pos != externals.end())
							  {
								  // Remove wrapped object from the list of external objects
								  externals.erase(pos);
							  }
						  });
		
		// Put wrapped object into the list of external objects
		externals[objectId] = wrapper;
		
		// Assotiate JavaScript variable with wrapped external object
		handle->Set( functionName.toV8String( handle->GetIsolate()), objectInstance);
	}
	
	CPP::ChainedPtr* ExternalsManager::get(const v8::Local<v8::Object>& handle, const String& functionName)
	{
		uint32_t objectId = getObjectId( handle, functionName);
		
		auto pos = externals.find( objectId);
		
		if(pos == externals.end())
			return nullptr;
		
		if(pos->second->getType() == FieldWrapper::WrapperType::ChainedPtr)
		{
			ChainedPtrWrapper* wrapper = dynamic_cast<ChainedPtrWrapper*>(pos->second);
			return wrapper->objectPtr();
		}
		
		return nullptr;
	}
	
	void ExternalsManager::clear(const v8::Local<v8::Object>& handle, const String& functionName)
	{
		v8::Isolate* isolate = handle->GetIsolate();
		v8::Handle<v8::Value> value = handle->Get(functionName.toV8String(isolate));
		
		if (V8_JAVASCRIPT_ENGINE->v8IsNull(value) == true)
			return;
		
		v8::Handle<v8::Object> object = value->ToObject(isolate);
		
		v8::Local<v8::Integer> val = v8::Uint32::New (isolate, 0);
		object->SetInternalField(0, val);
	}
	
	uint32_t ExternalsManager::getObjectId(const v8::Local<v8::Object>& handle, const String& functionName)
	{
		v8::Isolate* isolate = handle->GetIsolate();
		v8::Handle<v8::Value> value = handle->Get(functionName.toV8String(isolate));
		
		if (V8_JAVASCRIPT_ENGINE->v8IsNull(value) == true)
			return 0;
		
		v8::Handle<v8::Object> object = value->ToObject(isolate);
		
		v8::Local<v8::Value> objectIdValue = object->GetInternalField(0);
		
		v8::Handle<v8::Uint32> external = v8::Handle<v8::Uint32>::Cast(objectIdValue);
		
		uint32_t objectId = external->Value();
		
		return objectId;
	}
	
	v8::Local<v8::Object> ExternalsManager::newObjectInstance(const v8::Local<v8::Object>& handle)
	{
		v8::Local<v8::ObjectTemplate> objectTemplate = v8::ObjectTemplate::New(handle->GetIsolate());
		objectTemplate->SetInternalFieldCount(1);
		
		v8::Local<v8::Object> objectInstance = objectTemplate->NewInstance (handle->CreationContext()).ToLocalChecked();
		
		return objectInstance;
	}
	
	uint32_t ExternalsManager::nextId()
	{
		static uint32_t objectId = 0;
		
		return ++objectId;
	}
}

#endif

