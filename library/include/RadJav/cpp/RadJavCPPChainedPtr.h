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
#ifndef _RADJAV_CPP_CHAINEDPTR_H_
#define _RADJAV_CPP_CHAINEDPTR_H_

#include "RadJavPreprocessor.h"
#include <vector>
#include <functional>

namespace RadJAV
{
	namespace CPP
	{
		/**
		 * Base class for objects which can be chained in terms of object destructor
		 */
		class RADJAV_EXPORT ChainedPtr
		{
		public:
			ChainedPtr();
			ChainedPtr(const ChainedPtr& other) = delete;
			ChainedPtr& operator = (const ChainedPtr& other) = delete;
			
			virtual ~ChainedPtr();
			
		protected:
			/**
			 * Hard link with another ChainedPtr object.
			 * Derived class which call this method will be destroyed during
			 * destruction of other. Also if our class will be destroyed it will
			 * destroy other automatically.
			 * @param other a pointer to other ChainedPtr derived class with which we will link.
			 * @return void.
			 */
			void linkWith(ChainedPtr* other);
			
			/**
			 * Remove hard link with another ChainedPtr object.
			 * Derived class which call this method will not be destroyed during
			 * destruction of other. Also derived class needs to manage other's object lifetime
			 * manually after calling this method.
			 * @param other a pointer to other ChainedPtr derived class with which we unlink.
			 * @return void.
			 */
			void unlinkWith(ChainedPtr* other);
			
		private:
			void link(ChainedPtr* other);
			void unlink(ChainedPtr* other);
			void exclude(ChainedPtr* other);
			
		private:
			std::vector<ChainedPtr*> linkedWith; /*!< Vector of ChainedPtr pointers which hold parent objects. */
			std::vector<ChainedPtr*> toDelete; /*!< Vector of ChainedPtr pointers which hold child object */
		};

		/**
		 * Wrapper class to notify user when hooked ChainedPtr class does out of scope
		 */
		class RADJAV_EXPORT ChainedPtrHook : public ChainedPtr
		{
		public:
			/**
			 * A constructor.
			 * User must provide valid pointer to ChainedPtr derived class to hook on with
			 * and callback function which will be called when this and hooked object goes
			 * out of scope.
			 * @param target a pointer to ChainedPtr derived class which will be hooked.
			 * @param aboutToDelete a callback function which will be called when object goes out of scope.
			 */
			ChainedPtrHook(ChainedPtr* target, std::function<void ()> aboutToDelete);
			virtual ~ChainedPtrHook();

			ChainedPtrHook() = delete;
			ChainedPtrHook(const ChainedPtrHook&) = delete;
			ChainedPtrHook& operator = (const ChainedPtrHook& other) = delete;
			
			/**
			 * Get pointer of underlying hooked object.
			 * @return ChainedPtr a pointer to underlying hooked object.
			 */
			ChainedPtr* object() const;
		protected:
			std::function<void ()> aboutDelete; /*!< Pointer to callback function which will be called on destructor. */
			
			ChainedPtr* hookedObject; /*!< Pointer to ChainedPtr derived class on which we hook. */
		};
	}
}

#endif
