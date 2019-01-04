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
	 * @brief Base class to wrap C++ objects which we expose to Javascript engine.
	 */
	class FieldWrapper
	{
	public:
		/**
		 * @brief Type of wrapper.
		 */
		enum class WrapperType
		{
			SharedPtr, /**< Wrapper use std::shared_ptr as underlying object. */
			ChainedPtr /**< Wrapper use CPP::ChainedPtr as underlying object. */
		};
		
	public:
		/**
		 * @brief Constructor.
		 * @param[in] objectId is a unique ID of external object.
		 * @param[in] context Javascript virtual machine context.
		 * @param[in] handle Javascript object to which we attach C++ object.
		 * @param[in] functionName Javascript object's property name to which we attach C++ object.
		 * @param[in] wrapperType type of wrapper created by derived class
		 */
		FieldWrapper(uint32_t objectId,
					 JSContextRef context,
					 JSObjectRef handle,
					 const String& functionName,
					 WrapperType wrapperType);
		
		/**
		 * @brief Virtual destructor.
		 */
		virtual ~FieldWrapper();
		
		/**
		 * @brief Get unique Id of external object.
		 * @return uint32_t external object ID.
		 */
		uint32_t objectId() const;
		
		/**
		 * @brief Action to execute when wrapper is going out of scope.
		 * @details Can be used to remove wrapper from global list of
		 * wrapped objects.
		 * @param[in] aboutToDelete a pointer to callback function.
		 */
		void onDelete(std::function<void (FieldWrapper*)> aboutToDelete);
		
		/**
		 * @brief Get type of wrapper
		 * @see WrapperType
		 */
		WrapperType getType() const;
		
		/**
		 * Default constructor.
		 */
		FieldWrapper () = delete;
		
		/**
		 * @brief Copy constructor.
		 * @details We didn't expect from user to interchange data between FieldWrapper objects.
		 */
		FieldWrapper (const FieldWrapper& other) = delete;
		
	protected:
		/**
		 * @brief Perform wrap operation
		 */
		virtual void wrap();
		
		/**
		 * @brief A static callback function which will be called by Javascript engine garbage collector when Javascript object
		 * goes out of scope.
		 * @param[in] jsObject pointer to Javascript object which hold the native raw pointer.
		 */
		static void finalizeCallback (JSObjectRef jsObject);
		
	protected:
		uint32_t objectUniqueId; /**< Unique ID of wrapped object. */
		JSContextRef virtualMachineContext; /**< Context to Javascript virtual machine. */
		JSObjectRef javascriptObject; /**< Javascript object to which to add external C++ object. */
		JSObjectRef externalJavascriptObject; /**< Helper Javascript object which hold pointer to FieldWrapper. */
		String propertyName; /**< Name of the property of Javascript object to which we attach our helper Javascript object. */
		WrapperType type; /**< Type of this FieldWrapper class. */
		std::function<void (FieldWrapper*)> aboutDelete; /**< Function which will be called to notify when this FieldWrapper goes out of scope. */
	};
	
	/**
	 * @brief Class to wrap CPP::ChainedPtr derived C++ objects within Javascript engine.
	 * @details This class will hook on object destruction from C++ side
	 * as well as listen for garbage collector callback for corresponding Javascript object.
	 */
	class ChainedPtrWrapper : public FieldWrapper
	{
	public:
		/**
		 * Constructor.
		 * @param[in] objectId is a unique ID of external object.
		 * @param[in] context Javascript virtual machine context.
		 * @param[in] handle Javascript object to which we attach C++ object.
		 * @param[in] functionName Javascript object's property name to which we attach C++ object.
		 * @param[in] data a pointer to CPP::ChainedPtr derived class which will be
		 *  exposed to Javascript. Object can be deleted from C++ or Javascript side.
		 */
		ChainedPtrWrapper (uint32_t objectId,
						   JSContextRef context,
						   JSObjectRef handle,
						   const String& functionName,
						   CPP::ChainedPtr *data);
		
		/**
		 * Virtual destructor.
		 */
		virtual ~ChainedPtrWrapper ();
		
		/**
		 * @brief Get underlying external object.
		 * @return CPP::ChainedPtr* a pointer to external object.
		 */
		CPP::ChainedPtr* objectPtr();
		
	protected:
		/**
		 * @brief Hook on C++ object destruction and subscribe to garbage collector event.
		 */
		virtual void wrap ();
		
		/**
		 * @brief Notification on raw pointer destruction.
		 * @details Function will be called when wrapped object were destroyed from C++ side.
		 */
		void objectDestroyed();
		
	protected:
		CPP::ChainedPtrHook* objectHook; /**< Helper hook object on CPP::ChainedPtr to be notified on C++ object destruction */
		CPP::ChainedPtr* object; /**< Real data pointer which will be wrapped */
	};
	
	/**
	 * @brief Template class for wrapping arbitrary C++ objects within Javascript engine.
	 */
	template<class T>
	class SharedPtrWrapper : public FieldWrapper
	{
	public:
		/**
		 * @brief Constructor.
		 * @param[in] objectId is a unique ID of external object.
		 * @param[in] context Javascript virtual machine context.
		 * @param[in] handle Javascript object to which we attach C++ object.
		 * @param[in] functionName Javascript object's property name to which we attach C++ object.
		 * @param[in] data a shared pointer(std::shared_ptr) to an object which will be
		 * exposed to Javascript engine, wrapper will reset it during Javascript garbage collector callback.
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
		 * @brief Virtual destructor.
		 */
		virtual ~SharedPtrWrapper ()
		{
			// Reset underlying object
			object.reset();
		}
		
		/**
		 * @brief Get underlying external object.
		 * @return std::shared_ptr<T> a shared pointer to external object.
		 */
		std::shared_ptr<T> objectPtr()
		{
			return object;
		}
		
	protected:
		/**
		 * @brief Hook on C++ object destruction and subscribe to garbage collector event.
		 */
		virtual void wrap ()
		{
			//Hook on C++ object destruction and subscribe to garbage collector event.
			FieldWrapper::wrap();
		}
		
	protected:
		std::shared_ptr<T> object; /**< Real data shared pointer which will be wrapped */
	};
	
	/**
	 * @brief Implementation of ExternalsManager for JavaScriptCore engine.
	 * @see ExternalsManager for more information.
	 */
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
			if(!context || !handle || functionName.empty() || !object)
				return;
			
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
			if(!context || !handle || functionName.empty())
				return std::shared_ptr<T>();
			
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
