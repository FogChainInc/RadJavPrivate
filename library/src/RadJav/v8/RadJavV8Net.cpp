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
#include "v8/RadJavV8Net.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef HTTP_USE_CURL
#include <curl/curl.h>
#endif

#include "cpp/RadJavCPPNet.h"

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"

	#include "cpp/RadJavCPPNetWebServer.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace Net
		{
			void NetCallbacks::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "httpRequest", NetCallbacks::httpRequest);
			}

			void NetCallbacks::httpRequest(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Value> uri = V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0);
				v8::Local<v8::Value> timeout = V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1);
				v8::Local<v8::Value> promise;

				try
				{
					String struri = parseV8Value(uri);

					PromiseThread *thread = RJNEW PromiseThread ();
					thread->onStart = [struri, args, thread]()
						{
							RadJAV::CPP::Net::HttpRequest req(struri.c_str());
							req.connect();
							req.send();
							String str = req.receivedData();
							req.close();

							v8::Local<v8::String> v8str = str.toV8String(V8_JAVASCRIPT_ENGINE->isolate);
							v8::Local<v8::Array> ary = v8::Array::New (V8_JAVASCRIPT_ENGINE->isolate);
							ary->Set(0, v8str);
							thread->setResolveArgs(V8_JAVASCRIPT_ENGINE->isolate, ary);

							thread->resolvePromise();
						};
					promise = thread->createV8Promise(V8_JAVASCRIPT_ENGINE, args.This ());
					V8_JAVASCRIPT_ENGINE->addThread(thread);
				}
				catch (std::exception const& ex)
				{
					promise = v8::Undefined(args.GetIsolate());
					RadJav::throwException((String)"HttpRequest failed: " + (String)ex.what ());
				}

				args.GetReturnValue().Set(promise);
			}

			void NetCallbacks::completeHttpRequest(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
				v8::Local<v8::Function> resolve = v8::Local<v8::Function>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 0));
				v8::Local<v8::Function> reject = v8::Local<v8::Function>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 1));
				v8::Local<v8::Array> ary = v8::Local<v8::Array>::Cast(V8_JAVASCRIPT_ENGINE->v8GetArgument(args, 2));
				String uri = parseV8Value(ary->Get(0));
				RJBOOL post = V8_JAVASCRIPT_ENGINE->v8ParseBool (ary->Get(1));

				v8::Local<v8::Integer> timeout;
				RJLONG timeoutValue = 10;
				v8::Persistent<v8::Function> *resolvep = RJNEW v8::Persistent<v8::Function>();

				resolvep->Reset(args.GetIsolate(), resolve);

				if (ary->Length() > 2)
				{
					timeout = v8::Local<v8::Integer>::Cast(ary->Get(2));
					timeoutValue = timeout->Value();
				}

				SimpleThread *thread = RJNEW SimpleThread();
				thread->onStart = [thread, uri, post, timeoutValue, resolvep]()
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
						v8::Persistent<v8::Array> *results = NULL;

						if (str != NULL)
						{
							numArgs = 1;
							results = RJNEW v8::Persistent<v8::Array>();
							v8::Local<v8::Array> ary = v8::Array::New(V8_JAVASCRIPT_ENGINE->isolate);
							ary->Set(0, str->toV8String(V8_JAVASCRIPT_ENGINE->isolate));
							results->Reset(V8_JAVASCRIPT_ENGINE->isolate, ary);
						}

						V8_JAVASCRIPT_ENGINE->callFunctionOnNextTick(RJNEW AsyncFunctionCall(resolvep, results));

						DELETEOBJ(str);
						V8_JAVASCRIPT_ENGINE->removeThread(thread);
					};
				 
				//HttpThread *thread = RJNEW HttpThread(uri, post, timeoutValue, resolvep);
				//V8_JAVASCRIPT_ENGINE->addThread(thread);
			}

			RJINT NetCallbacks::curlWrite(RJCHAR *data, RJUINT size, RJUINT nmemb, String *output)
			{
				if (output == NULL)
					return (0);

				output->append(data, (size * nmemb));

				return (size * nmemb);
			}

			/*HttpThread::HttpThread(String uri, RJBOOL post, RJLONG timeout, v8::Persistent<v8::Function> *resolvep)
				: Thread()
			{
				this->uri = uri;
				this->post = post;
				this->timeout = timeout;
				this->resolvep = resolvep;
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
				v8::Persistent<v8::Array> *results = NULL;

				if (str != NULL)
				{
					numArgs = 1;
					results = RJNEW v8::Persistent<v8::Array>();
					v8::Local<v8::Array> ary = v8::Array::New(V8_JAVASCRIPT_ENGINE->isolate);
					ary->Set(0, str->toV8String(V8_JAVASCRIPT_ENGINE->isolate));
					results->Reset(V8_JAVASCRIPT_ENGINE->isolate, ary);
				}

				V8_JAVASCRIPT_ENGINE->callFunctionOnNextTick(RJNEW AsyncFunctionCall (resolvep, results));

				DELETEOBJ(str);
				V8_JAVASCRIPT_ENGINE->removeThread(this);

				return (0);
			}*/
		}
	}
}
#endif

