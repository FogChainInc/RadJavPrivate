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

#ifndef _RADJAV_CPP_PERSISTENT_H_
#define _RADJAV_CPP_PERSISTENT_H_

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#elif defined USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCJavascriptEngine.h"
#endif


namespace RadJAV
{
	namespace CPP
	{
		/**
		 * @ingroup group_basic_cpp
		 * @brief Persistent class.
		 * @details Class that make hard reference to Javascript object.
		 * Therefore prevent it from garbage colletion.
		 */
		class RADJAV_EXPORT Persistent
		{
		public:
			#ifdef USE_V8
				Persistent(v8::Local<v8::Object> jsObject);
			#elif defined USE_JAVASCRIPTCORE
				Persistent(JSObjectRef object);
			#endif

			//Strict usage for now
			Persistent() = delete;
			Persistent(const Persistent&) = delete;
			Persistent(Persistent&&) = delete;
			const Persistent& operator =(const Persistent&) = delete;

			virtual ~Persistent();

			#ifdef USE_V8
				v8::Local<v8::Object> get() const;
			#elif defined USE_JAVASCRIPTCORE
				JSObjectRef get() const;
			#endif

		protected:
			#ifdef USE_V8
				v8::Persistent<v8::Object> *persistent;
			#elif defined USE_JAVASCRIPTCORE
				JSObjectRef persistent;
			#endif
		};
	}
}

#endif
