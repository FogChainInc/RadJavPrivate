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
#ifndef _RADJAV_CPP_THREAD_H_
	#define _RADJAV_CPP_THREAD_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

    #ifdef USE_V8
        #include "v8/RadJavV8JavascriptEngine.h"
    #endif
    #ifdef USE_JAVASCRIPTCORE
        #include "jscore/RadJavJSCJavascriptEngine.h"
    #endif

	#include "cpp/RadJavCPPChainedPtr.h"

	namespace RadJAV
	{
		namespace CPP
		{
			/**
			 * @ingroup group_thread_cpp
			 * @brief Thread class.
			 */
			class RADJAV_EXPORT Thread : public ChainedPtr
			{
				public:
					#ifdef USE_V8
						Thread(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
					#endif
                    #ifdef USE_JAVASCRIPTCORE
                        Thread(JSCJavascriptEngine *jsEngine, RJINT numArgs, JSValueRef *args);
                    #endif
					Thread();

					void start();
					void close();

					#ifdef USE_V8
						void on(String event, v8::Local<v8::Function> func);
				
						v8::Persistent<v8::Value> *thread;
					#endif
                    #ifdef USE_JAVASCRIPTCORE
                        void on(String event, JSObjectRef func);
                
                        JSObjectRef thread;
                    #endif
			};
		}
	}
#endif

