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
#ifndef _RADJAV_V8_EXTERNALS_DETAILS_H_
#define _RADJAV_V8_EXTERNALS_DETAILS_H_

#ifdef USE_V8
#include "RadJavPreprocessor.h"
#include "RadJavString.h"
#include "RadJavThread.h"

#include <map>
#include <v8.h>

namespace RadJAV
{
	namespace CPP
	{
		class ChainedPtr;
		class ChainedPtrHook;
	}
	
	/**
	 * Base class to wrap objects which we expose to V8.
	 */
	class FieldWrapper
	{
	public:
		/**
		 * Type of wrapper.
		 */
		enum class WrapperType
		{
			SharedPtr,
			ChainedPtr
		};
		
	public:
		/**
		 * A constructor.
		 * @param objectId is a unique ID of external object.
		 * @param wrapperType type of wrapper created by derived class
		 */
		FieldWrapper(uint32_t objectId, WrapperType wrapperType);
		
		/**
		 * A destructor.
		 */
		virtual ~FieldWrapper();
		
		/**
		 * Get unique Id of external object.
		 * @return uint32_t external object ID.
		 */
		uint32_t objectId() const;
		
		/**
		 * Action to execute when wrapper is going out of scope.
		 * Can be used to remove wrapper from global list of
		 * wrapped objects.
		 * @param aboutToDelete a pointer to callback function.
		 * @return void.
		 */
		void onDelete(std::function<void (FieldWrapper*)> aboutToDelete);
		
		WrapperType getType() const;
		
		FieldWrapper () = delete;
		FieldWrapper (const FieldWrapper& other) = delete;
		
	protected:
		virtual void wrap(const v8::Local<v8::Object> &handle,
						  const v8::Local<v8::Object>& objectInstance);
		
		/**
		 * A static weak callback function which will be called by V8 garbage collector.
		 * @param data reference to FieldWrapper class which hold the
		 *  native raw pointer.
		 * @return void.
		 */
		static void weakCallback (const v8::WeakCallbackInfo<FieldWrapper> &data);
		
	protected:
		v8::Persistent<v8::Object> persistent;
		uint32_t objectUniqueId;
		std::function<void (FieldWrapper*)> aboutDelete;
		WrapperType type;
		v8::Isolate* isolate;
	};
	
	/**
	 * Class to wrap ChainedPtr derived C++ objects within V8.
	 * This class will hook on object destruction from C++ side
	 * as well as listen for garbage collector weak callback
	 * for underlying object.
	 */
	class ChainedPtrWrapper : public FieldWrapper
	{
	public:
		/**
		 * A constructor.
		 * @param handle a context to which we will add new V8 object.
		 * @param objectInstance a newly created ObjectTemplate instance for external object.
		 * @param objectId a unique ID of external object.
		 * @param data a pointer to ChainedPtr derived class which will be
		 *  exposed to V8. Object can be deleted from C++ or V8 side.
		 */
		ChainedPtrWrapper (const v8::Local<v8::Object> &handle,
						   const v8::Local<v8::Object>& objectInstance,
						   uint32_t objectId,
						   CPP::ChainedPtr *data);
		
		/**
		 * A destructor.
		 */
		virtual ~ChainedPtrWrapper ();
		
		/**
		 * Get underlying external object.
		 * @return CPP::ChainedPtr a pointer to external object.
		 */
		CPP::ChainedPtr* objectPtr();
		
	protected:
		/**
		 * Hook on C++ object destruction and subscribe to garbage collector event.
		 * @param handle is a context to which we plan to add new V8 object.
		 * @param objectInstance a newly created ObjectTemplate instance for external object.
		 */
		virtual void wrap (const v8::Local<v8::Object> &handle,
						   const v8::Local<v8::Object>& objectInstance);
		
		/**
		 * Notification on raw pointer destruction.
		 * @return void.
		 */
		void objectDestroyed();
		
	protected:
		CPP::ChainedPtrHook* objectHook;
		CPP::ChainedPtr* object;
	};
	
	/**
	 * Template class to wrap arbitrary C++ objects within V8.
	 */
	template<class T>
	class SharedPtrWrapper : public FieldWrapper
	{
	public:
		/**
		 * A constructor.
		 * @param handle is a context to which we will add new V8 object.
		 * @param objectInstance a newly created ObjectTemplate instance for external object.
		 * @param objectId is a unique ID of external object.
		 * @param data a shared pointer to an object which will be
		 *  exposed to V8, wrapper will reset it during V8 garbage collector weak callback
		 */
		SharedPtrWrapper (const v8::Local<v8::Object> &handle,
						  const v8::Local<v8::Object>& objectInstance,
						  uint32_t objectId,
						  std::shared_ptr<T> data)
		: FieldWrapper(objectId, FieldWrapper::WrapperType::SharedPtr),
		  object(data)
		{
			wrap(handle, objectInstance);
		}
		
		/**
		 * A destructor.
		 */
		virtual ~SharedPtrWrapper ()
		{
			// We know that this is ugly but we didn't have a better way for now
			isolate->AdjustAmountOfExternalAllocatedMemory( -(int64_t)sizeof(object.get()));

			// Reset underlying object
			object.reset();
		}
		
		/**
		 * Get underlying external object.
		 * @return T* a pointer to external object.
		 */
		std::shared_ptr<T> objectPtr()
		{
			return object;
		}
		
	protected:
		/**
		 * Subscribe to garbage collector event.
		 * Object will be destroyed on weak callback call.
		 * @param handle is a context to which we plan to add new V8 object.
		 * @param objectInstance a newly created ObjectTemplate instance for external object.
		 */
		virtual void wrap (const v8::Local<v8::Object> &handle,
						   const v8::Local<v8::Object>& objectInstance)
		{
			// Init Persistent object and set weak callback
			FieldWrapper::wrap(handle, objectInstance);
			
			// We know that this is ugly but we didn't have a better way for now
			isolate->AdjustAmountOfExternalAllocatedMemory( sizeof(object.get()));
		}
		
	protected:
		std::shared_ptr<T> object;
	};
	
	class ExternalsManagerV8Impl
	{
	public:
		ExternalsManagerV8Impl();

		~ExternalsManagerV8Impl();
		
		ExternalsManagerV8Impl(const ExternalsManagerV8Impl& ) = delete;
		ExternalsManagerV8Impl& operator = (const ExternalsManagerV8Impl&) = delete;
		
		void set(const v8::Local<v8::Object>& handle, const String& functionName, CPP::ChainedPtr* object);
		
		template<class T>
		void set(const v8::Local<v8::Object>& handle, const String& functionName, std::shared_ptr<T> object)
		{
			LOCK_GUARD(s_mutexExternalsAccess);

			uint32_t objectId = nextId();
			v8::Local<v8::Object> objectInstance = newObjectInstance(handle);
			
			v8::Local<v8::Integer> val = v8::Uint32::NewFromUnsigned(handle->GetIsolate(), objectId);
			objectInstance->SetInternalField(0, val);
			
			SharedPtrWrapper<T>* wrapper = RJNEW SharedPtrWrapper<T>(handle, objectInstance, objectId, object);
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
		
		CPP::ChainedPtr* get(const v8::Local<v8::Object>& handle, const String& functionName);
		
		template<class T>
		std::shared_ptr<T> get(const v8::Local<v8::Object>& handle, const String& functionName)
		{
			LOCK_GUARD(s_mutexExternalsAccess);
			
			uint32_t objectId = getObjectId( handle, functionName);
			
			auto pos = externals.find( objectId);
			
			if(pos == externals.end())
				return std::shared_ptr<T>();
			
			if(pos->second->getType() == FieldWrapper::WrapperType::SharedPtr)
			{
				SharedPtrWrapper<T>* wrapper = static_cast<SharedPtrWrapper<T>*>(pos->second);
				return wrapper->objectPtr();
			}
			
			return std::shared_ptr<T>();
		}
		
		void clear(const v8::Local<v8::Object>& handle, const String& functionName);
		
	private:
		uint32_t getObjectId(const v8::Local<v8::Object>& handle, const String& functionName);
		
		v8::Local<v8::Object> newObjectInstance(const v8::Local<v8::Object>& handle);
		
	private:
		uint32_t nextId();
		
	private:
		std::map<uint32_t, FieldWrapper*> externals;
		#ifdef GUI_USE_WXWIDGETS
			wxMutex s_mutexExternalsAccess;
		#else
			std::mutex s_mutexExternalsAccess;
		#endif
	};
}

#endif
#endif

