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
typedef JSObjectRef JSObject;

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
		 * @param functionName name of the Javascript object's field.
		 * @param wrapperType type of wrapper created by derived class
		 */
		FieldWrapper(uint32_t objectId, const std::string& functionName, WrapperType wrapperType);
		
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
		 * Get field name of Javascript object.
		 * @return std::string field name of Javascript object.
		 */
		const std::string& functionName() const;

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
		//static void weakCallback (const v8::WeakCallbackInfo<FieldWrapper> &data);
		
	protected:
		uint32_t objectUniqueId;
		std::string fieldName;
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
		 * @param handle a context to which we will add new V8 object.
		 * @param functionName name of the Javascript object's field.
		 * @param objectId a unique ID of external object.
		 * @param data a pointer to ChainedPtr derived class which will be
		 *  exposed to V8. Object can be deleted from C++ or V8 side.
		 */
		ChainedPtrWrapper (const JSObject &handle,
						   const std::string &functionName,
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
		 * Hook on C++ object destruction.
		 */
		void wrap ();
		
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
		 * @param functionName name of the Javascript object's field.
		 * @param objectId is a unique ID of external object.
		 * @param data a shared pointer to an object which will be
		 *  exposed to V8, wrapper will reset it during V8 garbage collector weak callback
		 */
		SharedPtrWrapper (const JSObject &handle,
						  const std::string &functionName,
						  uint32_t objectId,
						  std::shared_ptr<T> data)
		: FieldWrapper(objectId, functionName, FieldWrapper::WrapperType::SharedPtr),
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
		void wrap ()
		{
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
		
		void set(const JSObject& handle, const String& functionName, CPP::ChainedPtr* object);
		
		template<class T>
		void set(const JSObject& handle, const String& functionName, std::shared_ptr<T> object)
		{
			LOCK_GUARD(s_mutexExternalsAccess);

			uint32_t objectId = getObjectId(handle, functionName);
			if(!objectId)
			{
				//We never exposed any C++ objects to this Javascript object before
				//Lets select new ID for this Javascript object
				objectId = nextId();
			}
			
			SharedPtrWrapper<T>* wrapper = RJNEW SharedPtrWrapper<T>(handle, functionName, objectId, object);
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
		
		CPP::ChainedPtr* get(const JSObject& handle, const String& functionName);
		
		template<class T>
		std::shared_ptr<T> get(const JSObject& handle, const String& functionName)
		{
			LOCK_GUARD(s_mutexExternalsAccess);

			FieldWrapper* wrapper = getWrapper(handle, functionName);
			
			if(wrapper &&
			   wrapper->getType() == FieldWrapper::WrapperType::SharedPtr)
			{
				SharedPtrWrapper<T>* sharedPtrWrapper = static_cast<SharedPtrWrapper<T>*>(wrapper);
				return sharedPtrWrapper->objectPtr();
			}
			
			return std::shared_ptr<T>();
		}
		
		void clear(const JSObject& handle, const String& functionName);
		
	private:
		uint32_t getObjectId(const JSObject& handle, const String& functionName);
		FieldWrapper* getWrapper(const JSObject& handle, const String& functionName);
		
	private:
		uint32_t nextId();
		
	private:
		std::map<uint32_t, std::map<std::string, FieldWrapper*> > externals;
#ifdef GUI_USE_WXWIDGETS
		wxMutex s_mutexExternalsAccess;
#else
		std::mutex s_mutexExternalsAccess;
#endif
	};
}

#endif
#endif

