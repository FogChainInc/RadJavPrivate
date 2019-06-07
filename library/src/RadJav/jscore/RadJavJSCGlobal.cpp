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
#include "jscore/RadJavJSCGlobal.h"

#include "RadJav.h"
#include "RadJavString.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPIO.h"

namespace RadJAV
{
	namespace JSC
	{
		void Global::createJSCCallbacks(JSContextRef context, JSObjectRef object)
		{
            JSC_CALLBACK(object, "setTimeout", Global::setTimeout);
			JSC_CALLBACK(object, "clearTimeout", Global::clearTimeout);
            JSC_CALLBACK(object, "alert", Global::alert);
            JSC_CALLBACK(object, "confirm", Global::confirm);
            JSC_CALLBACK(object, "prompt", Global::prompt);
            JSC_CALLBACK(object, "include", Global::include);
            JSC_CALLBACK(object, "exit", Global::exit);
            JSC_CALLBACK(object, "quit", Global::exit);
		}
        
        JSValueRef Global::setTimeout(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
			JSValueRef undefined = JSValueMakeUndefined(context);
			
			if (numArgs < 2)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Function and timout were recuired");
				return undefined;
			}
			
            JSObjectRef nfunc = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(args[0]);
            JSValueRef time = args[1];
			
            RJUINT id = JSC_JAVASCRIPT_ENGINE->addTimeout(nfunc, JSC_JAVASCRIPT_ENGINE->jscValueToInt(time));
            
            return JSValueMakeNumber(context, id);
        }

		JSValueRef Global::clearTimeout(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
		{
			JSValueRef undefined = JSValueMakeUndefined(context);

			if (!numArgs)
			{
				JSC_JAVASCRIPT_ENGINE->throwException("Timer ID is recuired");
				return undefined;
			}
			
			JSValueRef idJs = args[0];
			
			if (idJs &&
				JSValueIsNumber(context, idJs))
			{
				JSC_JAVASCRIPT_ENGINE->clearTimeout(JSC_JAVASCRIPT_ENGINE->jscValueToInt(idJs));
			}
			
			return JSValueMakeUndefined(context);
		}

		JSValueRef Global::alert(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
		{
			String message = "";
			String title = "";

            message = parseJSCValue (context, args[0]);

            if (numArgs > 1)
                title = parseJSCValue (context, args[1]);

			RadJav::showMessageBox(message, title);

            return (JSValueMakeUndefined(context));
		}
        
        JSValueRef Global::confirm(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String message = "";
            String title = "";

            JSValueRef msg = args[0];
            
            if (JSC_JAVASCRIPT_ENGINE->jscIsNull(context, msg) == false)
                message = parseJSCValue(context, msg);
            
            if (numArgs > 1)
            {
                JSValueRef title2 = args[1];
                
                if (JSC_JAVASCRIPT_ENGINE->jscIsNull(title2) == false)
                    title = parseJSCValue(context, title2);
            }

            JSValueRef retVal = NULL;

            #ifdef GUI_USE_WXWIDGETS
                RJINT result = wxMessageBox(message.towxString(), title.towxString(), wxOK | wxCANCEL);
                RJBOOL output = false;
            
                if (result == wxOK)
                    output = true;
            
                JSValueRef result2 = JSValueMakeBoolean(context, output);
                retVal = result2;
            #endif
            
            return (retVal);
        }

        JSValueRef Global::prompt(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String message = "";
            String title = "";
            String defaultArg = "";
            
            JSValueRef msg = args[0];
            
            if (JSC_JAVASCRIPT_ENGINE->jscIsNull(context, msg) == false)
                message = parseJSCValue(context, msg);
            
            if (numArgs > 1)
            {
                JSValueRef defaultArg2 = args[1];
                
                if (JSC_JAVASCRIPT_ENGINE->jscIsNull(defaultArg2) == false)
                    defaultArg = parseJSCValue(context, defaultArg2);
            }
            
            if (numArgs > 2)
            {
                JSValueRef title2 = args[2];
                
                if (JSC_JAVASCRIPT_ENGINE->jscIsNull(title2) == false)
                    title = parseJSCValue(context, title2);
            }
            
            JSValueRef retVal = NULL;
            
            #ifdef GUI_USE_WXWIDGETS
                String result = parsewxString(wxGetTextFromUser(message.towxString(), title.towxString(), defaultArg.towxString()));
            
                JSValueRef result2 = result.toJSCValue(context);
                retVal = result2;
            #endif
            
            return (retVal);
        }

        JSValueRef Global::include(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            JSValueRef file = args[0];

            if (JSC_JAVASCRIPT_ENGINE->jscIsNull(file) == true)
            {
                JSC_JAVASCRIPT_ENGINE->throwException(context, exception, "First argument when calling an include function cannot be empty!");

                return (JSValueMakeUndefined(context));
            }
            
            String path = parseJSCValue(context, file);
            String contents = "";
            
            try
            {
                contents = CPP::IO::TextFile::readFile(path);
            }
            catch (Exception ex)
            {
                JSC_JAVASCRIPT_ENGINE->throwException(context, exception, ex.getMessage ());
                
                return (JSValueMakeUndefined(context));
            }
            
            JSC_JAVASCRIPT_ENGINE->executeScript(contents, path);
            
            JSObjectRef _emptyResolve = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_emptyResolve");
            JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObj, _emptyResolve);

            return (promise);
        }

        JSValueRef Global::collectGarbage(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            RadJav::javascriptEngine->collectGarbage();

            return (JSValueMakeUndefined(context));
        }

        JSValueRef Global::exit(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            RJINT exitCode = 0;

            if (numArgs > 0)
                exitCode = JSC_JAVASCRIPT_ENGINE->jscValueToInt (args[0]);

            RadJav::javascriptEngine->exit (exitCode);

            return (JSValueMakeUndefined(context));
        }
	}
}
