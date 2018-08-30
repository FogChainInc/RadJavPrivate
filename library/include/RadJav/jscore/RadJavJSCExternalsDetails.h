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
#ifndef _RADJAV_JSC_EXTERNALS_DETAILS_H_
#define _RADJAV_JSC_EXTERNALS_DETAILS_H_

#ifdef USE_JAVASCRIPTCORE
#include "RadJavPreprocessor.h"
#include "RadJavString.h"
#include "RadJavThread.h"

#include <map>
#include <JavaScriptCore/JavaScriptCore.h>

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
		 * @param context Javascript virtual machine context.
		 * @param handle Javascript object to which we attach C++ object.
		 * @param functionName Javascript object's property name to which we attach C++ object.
		 * @param wrapperType type of wrapper created by derived class
		 */
		FieldWrapper(uint32_t objectId,
					 JSContextRef context,
					 JSObjectRef handle,
					 const String& functionName,
					 WrapperType wrapperType);
		
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
		virtual void wrap();
		
		/**
		 * A static weak callback function which will be called by V8 garbage collector.
		 * @param data reference to FieldWrapper class which hold the
		 *  native raw pointer.
		 * @return void.
		 */
		static void finalizeCallback (JSObjectRef jsObject);
		
	protected:
		uint32_t objectUniqueId;
		JSContextRef virtualMachineContext;
		JSObjectRef javascriptObject;
		JSObjectRef externalJavascriptObject;
		String propertyName;
		WrapperType type;
		std::function<void (FieldWrapper*)> aboutDelete;
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
		 * @param objectId is a unique ID of external object.
		 * @param context Javascript virtual machine context.
		 * @param handle Javascript object to which we attach C++ object.
		 * @param functionName Javascript object's property name to which we attach C++ object.
		 * @param data a pointer to ChainedPtr derived class which will be
		 *  exposed to V8. Object can be deleted from C++ or V8 side.
		 */
		ChainedPtrWrapper (uint32_t objectId,
						   JSContextRef context,
						   JSObjectRef handle,
						   const String& functionName,
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
		 */
		virtual void wrap ();
		
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
		 * @param objectId is a unique ID of external object.
		 * @param context Javascript virtual machine context.
		 * @param handle Javascript object to which we attach C++ object.
		 * @param functionName Javascript object's property name to which we attach C++ object.
		 * @param data a shared pointer to an object which will be
		 *  exposed to V8, wrapper will reset it during V8 garbage collector weak callback
		 */
		SharedPtrWrapper (uint32_t objectId,
						  JSContextRef context,
						  JSObjectRef handle,
						  const String& functionName,
						  std::shared_ptr<T> data)
		: FieldWrapper(objectId, context, handle, functionName, FieldWrapper::WrapperType::SharedPtr),
		  object(data)
		{
			wrap();
		}
		
		/**
		 * A destructor.
		 */
		virtual ~SharedPtrWrapper ()
		{
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
		 * Hook on C++ object destruction and subscribe to garbage collector event.
		 */
		virtual void wrap ()
		{
			// Init Persistent object and set weak callback
			FieldWrapper::wrap();
		}
		
	protected:
		std::shared_ptr<T> object;
	};
	
	class ExternalsManagerJscImpl
	{
	public:
		ExternalsManagerJscImpl();

		~ExternalsManagerJscImpl();
		
		ExternalsManagerJscImpl(const ExternalsManagerJscImpl& ) = delete;
		ExternalsManagerJscImpl& operator = (const ExternalsManagerJscImpl&) = delete;
		
		void set(JSContextRef context, JSObjectRef handle, const String& functionName, CPP::ChainedPtr* object);
		
		template<class T>
		void set(JSContextRef context, JSObjectRef handle, const String& functionName, std::shared_ptr<T> object)
		{
			LOCK_GUARD(s_mutexExternalsAccess);

			uint32_t objectId = nextId();
			
			SharedPtrWrapper<T>* wrapper = RJNEW SharedPtrWrapper<T>( objectId, context, handle, functionName, object);
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
		}
		
		CPP::ChainedPtr* get(JSContextRef context, JSObjectRef handle, const String& functionName);
		
		template<class T>
		std::shared_ptr<T> get(JSContextRef context, JSObjectRef handle, const String& functionName)
		{
			LOCK_GUARD(s_mutexExternalsAccess);

			FieldWrapper* wrapper = getWrapper(context, handle, functionName);
			
			if (wrapper &&
				wrapper->getType() == FieldWrapper::WrapperType::SharedPtr)
			{
				SharedPtrWrapper<T>* sharedPtrWrapper = static_cast<SharedPtrWrapper<T>*>(wrapper);
				return sharedPtrWrapper->objectPtr();
			}
			
			return std::shared_ptr<T>();
		}
		
		void clear(JSContextRef context, JSObjectRef handle, const String& functionName);
		
	private:
		FieldWrapper* getWrapper(JSContextRef context, JSObjectRef handle, const String& functionName);
		
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

