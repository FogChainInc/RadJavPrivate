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
		 * @brief Base class for objects which can be chained in terms of object destructor.
		 * @details This class is a base class for objects which we plan to export into Javascript world.
		 * Mainly it was created to reduce memory leaks while exposing GUI controls. When you link different
		 * ChainedPtr based objects and delete one of them then all linked ChainedPtr objects will be destroyed as well.
		 * This works at any direction.
		 * @warning If we link with another ChainedPtr based object there is no need to delete it directly in destructor.
		 * ChainedPtr will do this automatically when our class goes out of scope(deleted).
		 */
		class RADJAV_EXPORT ChainedPtr
		{
		public:
			/**
			 * @brief Constructor
			 */
			ChainedPtr();
			
			/**
			 * @brief Copy consctructor.
			 * @details Currently we didn't expect the user to use copyable exposed derived classes so this constructor was removed.
			 */
			ChainedPtr(const ChainedPtr& other) = delete;
			
			/**
			 * @brief Assignment operator.
			 * @details Currently we didn't expect the user to use assignable exposed derived classes so this operator was removed.
			 */
			ChainedPtr& operator = (const ChainedPtr& other) = delete;
			
			/**
			 * @brief Virtual destructor.
			 * @details Destructor will go through all linked objects and delete them all one by one.
			 */
			virtual ~ChainedPtr();
			
		protected:
			/**
			 * @brief Hard link with another ChainedPtr object.
			 * @details Derived class which call this method will be destroyed during
			 * destruction of other. Also if our class will be destroyed it will
			 * destroy other automatically.
			 * @param[in] other a pointer to other ChainedPtr derived class with which we will link.
			 * @return void.
			 */
			void linkWith(ChainedPtr* other);
			
			/**
			 * @brief Remove hard link with another ChainedPtr object.
			 * @details Derived class which call this method will not be destroyed during
			 * destruction of other. Also derived class needs to manage other's object lifetime
			 * manually after calling this method.
			 * @param[in] other a pointer to other ChainedPtr derived class with which we unlink.
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
		 * @brief Wrapper class to notify user when hooked ChainedPtr class goes out of scope.
		 * @details This class mainly used by ExternalsManager to determine when exposed C++ object(to Javascript side) was deleted.
		 * @note When we directly delete ChainedPtrHook object this will lead to destruction of underlying hooked object. This
		 * behaviour also used by ExternalsManager when Javascript object is going out of scope.
		 */
		class RADJAV_EXPORT ChainedPtrHook : public ChainedPtr
		{
		public:
			/**
			 * @brief A constructor.
			 * @details User must provide valid pointer to ChainedPtr derived class to hook on with
			 * and callback function which will be called when this and hooked object goes
			 * out of scope.
			 * @param[in] target a pointer to ChainedPtr derived class which will be hooked.
			 * @param[in] aboutToDelete a callback function which will be called when object goes out of scope.
			 */
			ChainedPtrHook(ChainedPtr* target, std::function<void ()> aboutToDelete);
			
			/**
			 *	@brief Virtual destructor
			 */
			virtual ~ChainedPtrHook();
			
			/**
			 * @brief Constructor.
			 * @details deleted.
			 */
			ChainedPtrHook() = delete;
			
			/**
			 * @brief Copy constructor.
			 * @details deleted.
			 */
			ChainedPtrHook(const ChainedPtrHook&) = delete;
			
			/**
			 * @brief Assignment operator.
			 * @details deleted.
			 */
			ChainedPtrHook& operator = (const ChainedPtrHook& other) = delete;
			
			/**
			 * @brief Get pointer of underlying hooked object.
			 * @return ChainedPtr a pointer to underlying hooked object.
			 */
			ChainedPtr* object() const;
		protected:
			std::function<void ()> aboutDelete; /**< Pointer to callback function which will be called on destructor. */
			
			ChainedPtr* hookedObject; /**< Pointer to ChainedPtr derived class on which we hook. */
		};
	}
}

#endif
