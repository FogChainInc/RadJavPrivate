/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#include "RadJavThread.h"

#include "RadJavException.h"

#include "RadJav.h"

#ifdef USE_V8
    #include "v8/RadJavV8JavascriptEngine.h"
#endif
#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCJavascriptEngine.h"
#endif

namespace RadJAV
{
#ifdef GUI_USE_WXWIDGETS
	Thread::Thread()
		: wxThread(wxTHREAD_DETACHED)
	{
		hasStarted = false;
	}
#else
	Thread::Thread()
		: std::thread()
	{
		hasStarted = false;
	}
#endif

SimpleThread::SimpleThread()
	: Thread ()
{
}

#ifdef GUI_USE_WXWIDGETS
wxThread::ExitCode SimpleThread::Entry()
{
	onStart();

	return (0);
}
#else
RJINT SimpleThread::Entry()
{
	onStart();

	return (0);
}
#endif

PromiseThread::PromiseThread()
	: SimpleThread ()
{
	#ifdef USE_V8
		resolveArgs = NULL;
		rejectArgs = NULL;
	#endif
    #ifdef USE_JAVASCRIPTCORE
        resolveNumArgs = 0;
        resolveArgs = NULL;
        rejectNumArgs = 0;
        rejectArgs = NULL;
    #endif
}

#ifdef USE_V8
	void PromiseThread::runPromise(const v8::FunctionCallbackInfo<v8::Value> &args)
	{
		v8::Local<v8::Function> resolve = v8::Local<v8::Function>::Cast (args[0]);
		v8::Local<v8::Function> reject = v8::Local<v8::Function>::Cast (args[1]);
		v8::Local<v8::Array> args2 = v8::Local<v8::Array>::Cast (args[2]);
		v8::Local<v8::External> passThrough = v8::Local<v8::External>::Cast (args2->Get (0));
		PromiseThread *thread = (PromiseThread *)passThrough->Value();

		thread->resolvep = RJNEW v8::Persistent<v8::Function>();
		thread->rejectp = RJNEW v8::Persistent<v8::Function>();

		thread->resolvep->Reset(args.GetIsolate(), resolve);
		thread->rejectp->Reset(args.GetIsolate(), reject);
	}

	v8::Local<v8::Object> PromiseThread::createV8Promise(V8JavascriptEngine *engine, v8::Local<v8::Object> context)
	{
		this->engine = engine;
		v8::Local<v8::External> passThrough = v8::External::New(engine->isolate, this);
		v8::MaybeLocal<v8::Function> func = v8::Function::New(context->CreationContext(), PromiseThread::runPromise);
		v8::Local<v8::Function> func2 = func.ToLocalChecked();
		v8::Local<v8::Array> args = v8::Array::New(engine->isolate);
		args->Set (0, passThrough);
		v8::Local<v8::Object> promise = engine->createPromise(context, func2, args);

		return (promise);
	}

	void PromiseThread::setResolveArgs(v8::Isolate *isolate, v8::Local<v8::Array> args)
	{
		resolveArgs = RJNEW v8::Persistent<v8::Array>();
		resolveArgs->Reset(isolate, args);
	}

	void PromiseThread::setRejectArgs(v8::Isolate *isolate, v8::Local<v8::Array> args)
	{
		rejectArgs = RJNEW v8::Persistent<v8::Array>();
		rejectArgs->Reset(isolate, args);
	}
#endif
#ifdef USE_JAVASCRIPTCORE
    JSValueRef PromiseThread::runPromise(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
    {
        /*JSValueRef resolve = args[0];
        JSValueRef reject = args[1];
        v8::Local<v8::Array> args2 = v8::Local<v8::Array>::Cast (args[2]);
        v8::Local<v8::External> passThrough = v8::Local<v8::External>::Cast (args2->Get (0));
        PromiseThread *thread = (PromiseThread *)passThrough->Value();
        
        thread->resolvep = RJNEW v8::Persistent<v8::Function>();
        thread->rejectp = RJNEW v8::Persistent<v8::Function>();
        
        thread->resolvep->Reset(args.GetIsolate(), resolve);
        thread->rejectp->Reset(args.GetIsolate(), reject);*/

        return (JSValueMakeUndefined(context));
    }

    JSObjectRef PromiseThread::createJSCPromise(JSCJavascriptEngine *engine, JSObjectRef context)
    {
        this->engine = engine;

        

        /*v8::Local<v8::External> passThrough = v8::External::New(engine->isolate, this);
        v8::MaybeLocal<v8::Function> func = v8::Function::New(context->CreationContext(), PromiseThread::runPromise);
        v8::Local<v8::Function> func2 = func.ToLocalChecked();
        v8::Local<v8::Array> args = v8::Array::New(engine->isolate);
        args->Set (0, passThrough);
        v8::Local<v8::Object> promise = engine->createPromise(context, func2, args);*/
        JSObjectRef obj = NULL;
        
        return (obj);
    }
#endif

	void PromiseThread::resolvePromise()
	{
		#ifdef USE_V8
			V8_JAVASCRIPT_ENGINE->callFunctionOnNextTick(RJNEW AsyncFunctionCall(resolvep, resolveArgs));
		#endif
        #ifdef USE_JAVASCRIPTCORE
            JSC_JAVASCRIPT_ENGINE->callFunctionOnNextTick(RJNEW AsyncFunctionCall (resolvep, resolveNumArgs, resolveArgs));
        #endif

		onComplete();
	}

	void PromiseThread::rejectPromise()
	{
		#ifdef USE_V8
			V8_JAVASCRIPT_ENGINE->callFunctionOnNextTick(RJNEW AsyncFunctionCall(rejectp, rejectArgs));
		#endif
        #ifdef USE_JAVASCRIPTCORE
            JSC_JAVASCRIPT_ENGINE->callFunctionOnNextTick(RJNEW AsyncFunctionCall (rejectp, rejectNumArgs, rejectArgs));
        #endif

		onComplete();
	}
}

