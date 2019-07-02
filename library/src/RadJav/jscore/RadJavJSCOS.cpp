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
#include "jscore/RadJavJSCOS.h"

#include "RadJav.h"
#include "RadJavString.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPIO.h"
#include "cpp/RadJavCPPOS.h"

namespace RadJAV
{
	namespace JSC
	{
        JSObjectRef OS::onReadyFunction = NULL;

		void OS::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
            JSC_CALLBACK(object, "onReady", OS::onReady);
            JSC_CALLBACK(object, "openWebBrowserURL", OS::openWebBrowserURL);
            JSC_CALLBACK(object, "exec", OS::exec);
            JSC_CALLBACK(object, "getDocumentsPath", OS::getDocumentsPath);
            JSC_CALLBACK(object, "getTempPath", OS::getTempPath);
            JSC_CALLBACK(object, "getUserDataPath", OS::getUserDataPath);
            JSC_CALLBACK(object, "getApplicationPath", OS::getApplicationPath);
            JSC_CALLBACK(object, "getCurrentWorkingPath", OS::getCurrentWorkingPath);
            JSC_CALLBACK(object, "setCurrentWorkingPath", OS::setCurrentWorkingPath);
            JSC_CALLBACK(object, "saveFileAs", OS::saveFileAs);
            JSC_CALLBACK(object, "openFileAs", OS::openFileAs);
		}

		JSValueRef OS::onReady(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
		{
            onReadyFunction = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject (args[0]);

            return (JSValueMakeUndefined(context));
		}
        
        JSValueRef OS::openWebBrowserURL(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String url = parseJSCValue(context, args[0]);

            CPP::OS::openWebBrowserURL(url);

            return (JSValueMakeUndefined(context));
        }
        
        JSValueRef OS::exec(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String command = parseJSCValue(context, args[0]);
            RJINT output = CPP::OS::exec(command);

            return (JSValueMakeNumber(context, output));
        }

        JSValueRef OS::getDocumentsPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = CPP::OS::getDocumentsPath ();

            return (str.toJSCValue(context));
        }
        
        JSValueRef OS::getTempPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = CPP::OS::getTempPath ();
            
            return (str.toJSCValue(context));
        }
        
        JSValueRef OS::getUserDataPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = CPP::OS::getUserDataPath ();
            
            return (str.toJSCValue(context));
        }

        JSValueRef OS::getApplicationPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = CPP::OS::getApplicationPath ();

            return (str.toJSCValue(context));
        }
        
        JSValueRef OS::getCurrentWorkingPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = CPP::OS::getCurrentWorkingPath ();
            
            return (str.toJSCValue(context));
        }
        
        JSValueRef OS::setCurrentWorkingPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = parseJSCValue(context, args[0]);
            
            CPP::OS::setCurrentWorkingPath(str);
            
            return (JSValueMakeUndefined(context));
        }
        
        JSValueRef OS::saveFileAs(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = "";
            String message = "";
            String defaultDir = "";
            String defaultFile = "";
            String wildcard = "";
            RJBOOL overwritePrompt = true;

            if (numArgs > 0)
            {
                if (JSValueIsString (context, args[0]) == true)
                    message = parseJSCValue(context, args[0]);

                if (JSValueIsObject (context, args[0]) == true)
                {
                    JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject (context, args[0]);

                    message = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "message");
                    defaultDir = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "defaultDir");
                    defaultFile = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "defaultFile");
                    wildcard = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "wildcard");
                    overwritePrompt = JSC_JAVASCRIPT_ENGINE->jscGetBool(obj, "overwritePrompt");
                }
            }
            
            String path = CPP::OS::saveFileAs(message, defaultDir, defaultFile, wildcard, overwritePrompt);

            return (path.toJSCValue(context));
        }

        JSValueRef OS::openFileAs(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = "";
            String message = "";
            String defaultDir = "";
            String defaultFile = "";
            String wildcard = "";
            RJBOOL fileMustExist = true;
            
            if (numArgs > 0)
            {
                if (JSValueIsString (context, args[0]) == true)
                    message = parseJSCValue(context, args[0]);
                
                if (JSValueIsObject (context, args[0]) == true)
                {
                    JSObjectRef obj = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject (context, args[0]);
                    
                    message = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "message");
                    defaultDir = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "defaultDir");
                    defaultFile = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "defaultFile");
                    wildcard = JSC_JAVASCRIPT_ENGINE->jscGetString(obj, "wildcard");
                    fileMustExist = JSC_JAVASCRIPT_ENGINE->jscGetBool(obj, "fileMustExist");
                }
            }

            String path = CPP::OS::openFileAs(message, defaultDir, defaultFile, wildcard, fileMustExist);

            return (path.toJSCValue(context));
        }

		#ifndef USE_IOS
		void OS::SystemProcess::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
			JSC_CALLBACK(object, "_init", OS::SystemProcess::_init);
			JSC_CALLBACK(object, "execute", OS::SystemProcess::execute);
			JSC_CALLBACK(object, "kill", OS::SystemProcess::kill);
			JSC_CALLBACK(object, "on", OS::SystemProcess::on);
		}
		
		JSValueRef OS::SystemProcess::_init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			std::shared_ptr<CPP::OS::SystemProcess> appObject(RJNEW CPP::OS::SystemProcess(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments),
															  [](CPP::OS::SystemProcess* p) {
																  DELETEOBJ(p);
															  });
			
			JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
			
			return JSValueMakeUndefined(ctx);
		}
		
		JSValueRef OS::SystemProcess::execute(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			std::shared_ptr<CPP::OS::SystemProcess> appObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CPP::OS::SystemProcess>(ctx, thisObject, "_appObj");
			
			if (!appObject)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "SystemProcess not initialized");
				return undefined;
			}
			
			appObject->execute();
			
			return undefined;
		}
		
		JSValueRef OS::SystemProcess::kill(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			std::shared_ptr<CPP::OS::SystemProcess> appObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CPP::OS::SystemProcess>(ctx, thisObject, "_appObj");
			
			if (!appObject)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "SystemProcess not initialized");
				return undefined;
			}
			
			appObject->kill();
			
			return undefined;
		}
		
		JSValueRef OS::SystemProcess::on(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(ctx);
			std::shared_ptr<CPP::OS::SystemProcess> appObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<CPP::OS::SystemProcess>(ctx, thisObject, "_appObj");
			
			if (!appObject)
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "SystemProcess not initialized");
				return undefined;
			}
			
			JSValueRef eventJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
			JSValueRef funcJs = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 1);
			JSObjectRef funcObjJs = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(funcJs);

			if( !eventJs ||
			    !funcObjJs ||
			   !JSValueIsString(ctx, eventJs) ||
			   !JSObjectIsFunction(ctx, funcObjJs))
			{
				JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Event name and function is required");
				return undefined;
			}
			
			appObject->on(parseJSCValue(ctx, eventJs), funcObjJs);
			
			return undefined;
		}
		#endif
	}
}
