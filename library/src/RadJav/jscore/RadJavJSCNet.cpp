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
#include "jscore/RadJavJSCNet.h"

#include "RadJav.h"

#ifdef HTTP_USE_CURL
#include <curl/curl.h>
#endif

#ifdef USE_NET_BEAST
#include "cpp/RadJavCPPNet.h"
#endif

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPNetWebServer.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace Net
		{
			void NetCallbacks::createV8Callbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "httpRequest", NetCallbacks::httpRequest);
				JSC_CALLBACK(object, "httpPost", NetCallbacks::httpPost);
			}

			JSValueRef NetCallbacks::httpPost(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef uri = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef timeout = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				JSValueRef post = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 2);
				
				if (!uri)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("URI cannot be null!");
					return JSValueMakeUndefined(ctx);
				}
				
				RJUINT numberOfArguments = 2;

				JSValueRef ary[3];
				ary[0] = uri;
				ary[1] = JSValueMakeBoolean(ctx, true);

				if (timeout)
				{
					ary[2] = timeout;
					numberOfArguments++;
				}
				
				JSStringRef funcName = JSStringCreateWithUTF8CString("callback");
				JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, funcName, NetCallbacks::completeHttpRequest);
				JSStringRelease(funcName);
				
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, func, numberOfArguments, ary);

				return promise;
			}

			JSValueRef NetCallbacks::httpRequest(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef uri = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef timeout = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);

				JSValueRef ary[3];
				ary[0] = uri;
				ary[1] = JSValueMakeBoolean(ctx, false);

				RJUINT numberOfArguments = 2;

				if (timeout)
				{
					ary[2] = timeout;
					numberOfArguments++;
				}

				JSStringRef funcName = JSStringCreateWithUTF8CString("callback");
				JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, funcName, NetCallbacks::completeHttpRequest);
				JSStringRelease(funcName);
				
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, func, numberOfArguments, ary);

				return promise;
			}

			JSValueRef NetCallbacks::completeHttpRequest(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSObjectRef resolve = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(
										JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0));
				
				JSObjectRef reject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(
										JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1));
				
				JSObjectRef ary = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(
										JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 2));

				if (ary)
				{
					String uri = parseJSCValue(ctx, JSObjectGetPropertyAtIndex(ctx, ary, 0, nullptr));
					RJBOOL post = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean( JSObjectGetPropertyAtIndex(ctx, ary, 1, nullptr));
					RJLONG timeoutValue = 10;

					JSValueRef timeout = JSObjectGetPropertyAtIndex(ctx, ary, 2, nullptr);
					if (!JSC_JAVASCRIPT_ENGINE->jscIsNull(timeout))
					{
						timeoutValue = JSC_JAVASCRIPT_ENGINE->jscValueToInt(timeout);
					}
					
					HttpThread *thread = RJNEW HttpThread(uri, post, timeoutValue, ctx, resolve);
					JSC_JAVASCRIPT_ENGINE->addThread(thread);
				}
				
				return JSValueMakeUndefined(ctx);
			}

			RJINT NetCallbacks::curlWrite(RJCHAR *data, RJUINT size, RJUINT nmemb, String *output)
			{
				if (output == NULL)
					return (0);

				output->append(data, (size * nmemb));

				return (size * nmemb);
			}

			HttpThread::HttpThread(String uri, RJBOOL post, RJLONG timeout, JSContextRef ctx, JSObjectRef resolvep)
				: Thread()
			{
				this->uri = uri;
				this->post = post;
				this->timeout = timeout;
				this->context = ctx;
				this->resolvep = resolvep;
				
				JSValueProtect(ctx, resolvep);
			}
			
			HttpThread::~HttpThread()
			{
				JSValueUnprotect(context, resolvep);
			}
			
#ifdef GUI_USE_WXWIDGETS
			wxThread::ExitCode HttpThread::Entry()
#else
			RJINT HttpThread::Entry()
#endif
			{
				String *str = NULL;

#ifdef HTTP_USE_CURL
				CURL *curl = curl_easy_init();

				if (curl != NULL)
				{
					str = RJNEW String();
					curl_easy_setopt(curl, CURLOPT_URL, uri.c_str());
					curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, true);
					curl_easy_setopt(curl, CURLOPT_WRITEDATA, str);
					curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, NetCallbacks::curlWrite);
					curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, timeout);
					CURLcode result = curl_easy_perform(curl);
				}

				curl_easy_cleanup(curl);
#endif
#ifdef USE_NET_BEAST
				//call the http request using Beast
				try
				{
					RadJAV::CPP::Net::HttpRequest req(uri.c_str());
					req.connect();
					req.send(post);
					*str = req.receivedData();
					req.close();
				}
				catch (std::exception const& e)
				{
					RadJav::throwException("HttpRequest failed");
				}
#endif

				RJINT numArgs = 0;
				JSValueRef args[1];
				
				if (str != NULL)
				{
					numArgs = 1;
					JSStringRef strCopy = str->toJSCString();
					args[0] = JSValueMakeString(context, strCopy);
					JSStringRelease(strCopy);
				}

				JSC_JAVASCRIPT_ENGINE->callFunctionOnNextTick(RJNEW AsyncFunctionCall (resolvep, numArgs, args));

				DELETEOBJ(str);
				JSC_JAVASCRIPT_ENGINE->removeThread(this);

				return (0);
			}
		}
	}
}
