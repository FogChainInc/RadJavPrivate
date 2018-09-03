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
#ifndef _RADJAV_JS_EXTERNALS_H_
#define _RADJAV_JS_EXTERNALS_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#ifdef USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCExternalsDetails.h"
#elif defined USE_V8
	#include "v8/RadJavV8ExternalsDetails.h"
#endif

#if defined USE_V8 || defined USE_JAVASCRIPTCORE

namespace RadJAV
{
	namespace CPP
	{
		class ChainedPtr;
		class ChainedPtrHook;
	}
	
	/**
	 * Main class for managing external C++ objects within withing Javascript engine.
	 */
	class ExternalsManager
	{
	public:
		/**
		 * Contructor.
		 */
		ExternalsManager();

		/**
		 * Destructor.
		 */
		~ExternalsManager();
		
		ExternalsManager(const ExternalsManager& ) = delete;
		ExternalsManager& operator = (const ExternalsManager&) = delete;

#ifdef USE_V8
		/**
		 * Add relation between C++ object and corresponding Javascript object field.
		 * Object will be destroyed during Javascript engine garbage collector notification or directly from C++ side.
		 * @param handle is a context to which we plan to add new C++ object.
		 * @param functionName a name of the property of Javascript object to bind external object with.
		 * @param object a pointer to external object derived from ChainedPtr class.
		 */
		void set(const v8::Local<v8::Object>& handle, const String& functionName, CPP::ChainedPtr* object);
		
		/**
		 * Template function to add a relation between C++ object and corresponding Javascript object field.
		 * Object will be reset during Javascript garbage collector callback or
		 *  when ExternalsManager will go out of scope.
		 * @param handle is a context to which we plan to add new C++ object.
		 * @param functionName a name of the property of Javascript object to bind external object with.
		 * @param object a shared pointer to external object.
		 */
		template<class T>
		void set(const v8::Local<v8::Object>& handle, const String& functionName, std::shared_ptr<T> object)
		{
			impl->set(handle, functionName, object);
		}
		
		/**
		 * Get external object from Javascript object's field.
		 * If C++ external object is out of scope function will return nullptr.
		 * @param handle is a context(Javascript object) for which we query.
		 * @param functionName a name of the property of Javascript object from which to retrieve
		 *  external object.
		 * @return ChainedPtr a pointer to ChainedPtr derived class which where stored by set().
		 */
		CPP::ChainedPtr* get(const v8::Local<v8::Object>& handle, const String& functionName);
		
		/**
		 * Template function to get external object from Javascript object's field.
		 * If C++ external object where freed by garbage collector callback then
		 * function will return empty shared pointer.
		 * @param handle is a context(Javascript object) for which we query.
		 * @param functionName a name of the property of Javascript object from which to retrieve
		 *  external object.
		 * @return shared_ptr<T> a shared pointer to arbitrary C++ object previously stored by set().
		 */
		template<class T>
		std::shared_ptr<T> get(const v8::Local<v8::Object>& handle, const String& functionName)
		{
			return impl->get<T>(handle, functionName);
		}
		
		/**
		 * Clear Javascript object's field which hold external object ID.
		 * Underlying C++ object will not be freed/reset during that call.
		 *  Subsequent calls to get() for this functionName will return nullptr.
		 * @param handle is a context(Javascript object) for which we clear external object ID.
		 * @param functionName a name of the property of Javascript object from which to retrieve
		 *  external object.
		 * @note function is usefull in cases where we trying to delete wrapped C++ object from
		 *  C++ side, lets say by destroy() function which were called from Javascript side.
		 */
		void clear(const v8::Local<v8::Object>& handle, const String& functionName);

#elif defined USE_JAVASCRIPTCORE
		/**
		 * Add relation between C++ object and corresponding Javascript object field.
		 * Object will be destroyed during Javascript engine garbage collector notification or directly from C++ side.
		 * @param context is a context of Javascript virtual machine.
		 * @param handle is a Javascript object to which we plan to add new C++ object.
		 * @param functionName a name of the property of Javascript object to bind external object with.
		 * @param object a pointer to external object derived from ChainedPtr class.
		 */
		void set(JSContextRef context, JSObjectRef handle, const String& functionName, CPP::ChainedPtr* object);
		
		/**
		 * Template function to add a relation between C++ object and corresponding Javascript object field.
		 * Object will be reset during Javascript garbage collector callback or
		 *  when ExternalsManager will go out of scope.
		 * @param context is a context of Javascript virtual machine.
		 * @param handle is a Javascript object to which we plan to add new C++ object.
		 * @param functionName a name of the property of Javascript object to bind external object with.
		 * @param object a shared pointer to external object.
		 */
		template<class T>
		void set(JSContextRef context, JSObjectRef handle, const String& functionName, std::shared_ptr<T> object)
		{
			impl->set(context, handle, functionName, object);
		}
		
		/**
		 * Get external object from Javascript object's field.
		 * If C++ external object is out of scope function will return nullptr.
		 * @param context is a context of Javascript virtual machine.
		 * @param handle is a Javascript object from which we get C++ object.
		 * @param functionName a name of the property of Javascript object from which to retrieve
		 *  external object.
		 * @return ChainedPtr a pointer to ChainedPtr derived class which where stored by set().
		 */
		CPP::ChainedPtr* get(JSContextRef context, JSObjectRef handle, const String& functionName);
		
		/**
		 * Template function to get external object from Javascript object's field.
		 * If C++ external object where freed by garbage collector callback then
		 * function will return empty shared pointer.
		 * @param context is a context of Javascript virtual machine.
		 * @param handle is a Javascript object from which we get C++ object.
		 * @param functionName a name of the property of Javascript object from which to retrieve
		 *  external object.
		 * @return shared_ptr<T> a shared pointer to arbitrary C++ object previously stored by set().
		 */
		template<class T>
		std::shared_ptr<T> get(JSContextRef context, JSObjectRef handle, const String& functionName)
		{
			return impl->get<T>(context, handle, functionName);
		}
		
		/**
		 * Clear Javascript object's field which hold external object ID.
		 * Underlying C++ object will not be freed/reset during that call.
		 *  Subsequent calls to get() for this functionName will return nullptr.
		 * @param context is a context of Javascript virtual machine.
		 * @param handle is a Javascript object for which we get clear object ID related to C++ object.
		 * @param functionName a name of the property of Javascript object from which to retrieve
		 *  external object.
		 * @note function is usefull in cases where we trying to delete wrapped C++ object from
		 *  C++ side, lets say by destroy() function which were called from Javascript side.
		 */
		void clear(JSContextRef context, JSObjectRef handle, const String& functionName);
#endif
	private:
#ifdef USE_V8
		ExternalsManagerV8Impl* impl;
#elif defined USE_JAVASCRIPTCORE
		ExternalsManagerJscImpl* impl;
#endif
	};
}

#endif
#endif

