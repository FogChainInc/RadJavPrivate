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

#ifdef USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCJavascriptEngine.h"

	#include "cpp/RadJavCPPIO.h"

    #ifdef GUI_USE_WXWIDGETS
        #include <wx/wx.h>
        #include <wx/stdpaths.h>
    #endif

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

            #ifdef GUI_USE_WXWIDGETS
                wxLaunchDefaultBrowser(url.towxString());
            #endif

            return (JSValueMakeUndefined(context));
        }
        
        JSValueRef OS::exec(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String command = parseJSCValue(context, args[0]);
            RJINT output = 0;

            #ifdef GUI_USE_WXWIDGETS
                output = wxExecute(command.towxString());
            #endif

            return (JSValueMakeNumber(context, output));
        }

        JSValueRef OS::getDocumentsPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = "";

            #ifdef GUI_USE_WXWIDGETS
                str = parsewxString(wxStandardPaths::Get().GetDocumentsDir());
            #endif

            return (str.toJSCValue(context));
        }
        
        JSValueRef OS::getTempPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = "";
            
            #ifdef GUI_USE_WXWIDGETS
                str = parsewxString(wxStandardPaths::Get().GetTempDir());
            #endif
            
            return (str.toJSCValue(context));
        }
        
        JSValueRef OS::getUserDataPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = "";
            
            #ifdef GUI_USE_WXWIDGETS
                str = parsewxString(wxStandardPaths::Get().GetUserConfigDir());
            #endif
            
            return (str.toJSCValue(context));
        }

        JSValueRef OS::getApplicationPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = "";

            #ifdef GUI_USE_WXWIDGETS
                str = parsewxString(wxStandardPaths::Get().GetExecutablePath());
            #endif

            return (str.toJSCValue(context));
        }
        
        JSValueRef OS::getCurrentWorkingPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = "";
            
            #ifdef GUI_USE_WXWIDGETS
                str = parsewxString(wxGetCwd());
            #endif
            
            return (str.toJSCValue(context));
        }
        
        JSValueRef OS::setCurrentWorkingPath(JSContextRef context, JSObjectRef func, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[], JSValueRef *exception)
        {
            String str = parseJSCValue(context, args[0]);
            
            #ifdef GUI_USE_WXWIDGETS
                wxSetWorkingDirectory(str);
            #endif
            
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
            String path = "";

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
            
            #ifdef GUI_USE_WXWIDGETS
                wxString wxmsg = wxFileSelectorPromptStr;
            
                if (message != "")
                    wxmsg = message.towxString();
            
                RJLONG style = wxFD_SAVE;
            
                if (overwritePrompt == true)
                    style |= wxFD_OVERWRITE_PROMPT;
            
                wxFileDialog fileDialog(NULL, wxmsg, defaultDir.towxString(), defaultFile.towxString(), wildcard.towxString(), style);
            
                if (fileDialog.ShowModal() != wxID_CANCEL)
                    path = parsewxString(fileDialog.GetPath());
            #endif

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
            String path = "";
            
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

            #ifdef GUI_USE_WXWIDGETS
                wxString wxmsg = wxFileSelectorPromptStr;

                if (message != "")
                    wxmsg = message.towxString();

                RJLONG style = wxFD_OPEN;

                if (fileMustExist == true)
                    style |= wxFD_FILE_MUST_EXIST;

                wxFileDialog fileDialog(NULL, wxmsg, defaultDir.towxString(), defaultFile.towxString(), wildcard.towxString(), style);

                if (fileDialog.ShowModal() != wxID_CANCEL)
                    path = parsewxString(fileDialog.GetPath());
            #endif

            return (path.toJSCValue(context));
        }
	}
}
#endif

