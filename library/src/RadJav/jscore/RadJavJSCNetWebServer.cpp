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
			using CppWebServer = CPP::Net::WebServer;

			void WebServer::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", WebServer::init);
				JSC_CALLBACK(object, "_start", WebServer::start);
				JSC_CALLBACK(object, "_stop", WebServer::stop);
				JSC_CALLBACK(object, "_on", WebServer::on);
			}

			JSValueRef WebServer::init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<CppWebServer> webServer(RJNEW CppWebServer(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments),
														[](CppWebServer* p) {
															DELETEOBJ(p);
														});

				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", webServer);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef WebServer::start(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				std::shared_ptr<CppWebServer> webServer = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppWebServer>(ctx, thisObject, "_appObj");
				
				if (!webServer)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "WebServer not initialized");
					return undefined;
				}
				
				JSValueRef uriJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef portJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				
				if (!uriJs ||
					!portJs ||
					!JSValueIsString(ctx, uriJs) ||
					!JSValueIsNumber(ctx, portJs))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "URI and port arguments is required");
					return undefined;
				}
				
				RJINT port = JSC_JAVASCRIPT_ENGINE->jscParseInt(portJs);
				
				webServer->start(parseJSCValue(ctx, uriJs), port);

				return undefined;
			}

			JSValueRef WebServer::stop(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				std::shared_ptr<CppWebServer> webServer = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppWebServer>(ctx, thisObject, "_appObj");
				
				if (!webServer)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "WebServer not initialized");
					return undefined;
				}
				
				webServer->stop();
				
				return undefined;
			}
			
			JSValueRef WebServer::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				std::shared_ptr<CppWebServer> webServer = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CppWebServer>(ctx, thisObject, "_appObj");
				
				if (!webServer)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "WebServer not initialized");
					return undefined;
				}
				
				JSValueRef eventJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				JSValueRef funcJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
				JSObjectRef funcObjJs = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, funcJs);

				if (!eventJs ||
					!funcObjJs ||
					!JSValueIsString(ctx, eventJs) ||
					!JSObjectIsFunction(ctx, funcObjJs))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Event name and function is required");
					return undefined;
				}
				
				webServer->on(parseJSCValue(ctx, eventJs), funcObjJs);
				
				return undefined;
			}
		}
	}
}
