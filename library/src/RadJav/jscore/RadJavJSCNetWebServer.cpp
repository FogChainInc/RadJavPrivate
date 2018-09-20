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
#include "jscore/RadJavJSCNetWebServer.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPNetWebServer.h"

#define NETTYPE CPP::Net::WebServer

namespace RadJAV
{
	namespace JSC
	{
		namespace Net
		{
			void WebServer::createV8Callbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", WebServer::_init);
				JSC_CALLBACK(object, "listen", WebServer::listen);
				JSC_CALLBACK(object, "serve", WebServer::serve);
				JSC_CALLBACK(object, "stop", WebServer::stop);
			}

			JSValueRef WebServer::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				NETTYPE *webServer = RJNEW NETTYPE();
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_webServer", webServer);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebServer::listen(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				JSValueRef portArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!portArg)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Port cannot be null!");
					return undefined;
				}
				
				RJINT port = JSC_JAVASCRIPT_ENGINE->jscValueToNumber(ctx, portArg);

				NETTYPE *webServer = (NETTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webServer");
				if (webServer)
				{
					webServer->port = port;
					webServer->listen();
				}
				
				return undefined;
			}

			JSValueRef WebServer::serve(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				JSValueRef funcArg = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!funcArg)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Function cannot be null!");
					return undefined;
				}

				NETTYPE *webServer = (NETTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webServer");

				if (!webServer)
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Web server not listening!");
					return undefined;
				}

				JSObjectRef func = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(funcArg);
				
				webServer->serve(func);
				
				return undefined;
			}

			JSValueRef WebServer::stop(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);

				NETTYPE *webServer = (NETTYPE *)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_webServer");

				if ((webServer == NULL) || (webServer->isAlive == false))
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Web server not listening!");
					return undefined;
				}

				webServer->stop();
				
				return undefined;
			}
		}
	}
}
