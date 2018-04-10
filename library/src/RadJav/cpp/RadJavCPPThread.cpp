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
#include "cpp/RadJavCPPThread.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		#ifdef USE_V8
			Thread::Thread(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}
		#endif

		Thread::Thread()
		{
		}

		void Thread::start()
		{
			// Execute the thread.

			#ifdef USE_V8
				// Call the persistent thread.
			#endif
		}

		void Thread::close()
		{
			// End the thread.
		}

		#ifdef USE_V8
			void Thread::on(String event, v8::Local<v8::Function> func)
			{
				if (event == "thread")
				{
					thread = RJNEW v8::Persistent<v8::Value>();
					thread->Reset(func->GetIsolate (), func);
				}
			}
		#endif
	}
}

