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
#ifndef _RADJAV_GUI_CPP_EVENT_H_
#define _RADJAV_GUI_CPP_EVENT_H_

#include "RadJav.h"

#ifdef GUI_USE_WXWIDGETS
    #include <wx/object.h>
#endif

#ifdef USE_V8
    #include "v8/RadJavV8JavascriptEngine.h"
#endif

#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCJavascriptEngine.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
                template<class P>
                class RADJAV_EXPORT VariantObject
				#ifdef GUI_USE_WXWIDGETS
					: public wxObject
				#endif
                {
                public:
                    VariantObject(P data): object(data) {}
                    virtual ~VariantObject() {
                        if( std::is_pointer<P>::value)
						{
							#ifdef USE_JAVASCRIPTCORE
								//TODO: Do better here to unprotect JS function object
								JSValueUnprotect(JSC_JAVASCRIPT_ENGINE->globalContext, JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(object));
							#else
								DELETEOBJ(object);
							#endif
						}
                    }
                    
                    VariantObject(const VariantObject& other) = delete;
                    VariantObject& operator = (const VariantObject& other) = delete;
                    
                protected:
                    P object;
                };

            #ifdef USE_V8
                template<class UserData = v8::Persistent<v8::Value>* >
                class RADJAV_EXPORT GuiEvent : public VariantObject<UserData>
                {
                public:
                    GuiEvent(UserData data): VariantObject<UserData>(data) {}
                    
                    v8::Local<v8::Value> operator ()(RJINT numArgs = 0, v8::Local<v8::Value> *args = NULL)
                    {
                        // Execute a persistent function.
                        v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast( VariantObject<UserData>::object->Get(V8_JAVASCRIPT_ENGINE->isolate));
                        v8::Local<v8::Value> result = v8::Undefined (V8_JAVASCRIPT_ENGINE->isolate);
                        
                        if (V8_JAVASCRIPT_ENGINE->v8IsNull(function) == false)
                            result = function->Call(V8_JAVASCRIPT_ENGINE->globalContext->Global(), numArgs, args);
                        
                        return (result);
                    }
                };
            #endif

            #ifdef USE_JAVASCRIPTCORE
                template<class UserData = JSObjectRef >
                class RADJAV_EXPORT GuiEvent : public VariantObject<UserData>
                {
                public:
                    GuiEvent(UserData data): VariantObject<UserData>(data)
					{
						#ifdef USE_JAVASCRIPTCORE
							//TODO: Do better here to protect JS function object
							JSValueProtect(JSC_JAVASCRIPT_ENGINE->globalContext, JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(data));
						#endif
					}
                    
                    JSValueRef operator ()(RJINT numArgs = 0, JSValueRef *args = NULL)
                    {
                        JSObjectRef function = VariantObject<UserData>::object;
                        JSValueRef result = nullptr;
                        JSValueRef exception = nullptr;

                        if (JSC_JAVASCRIPT_ENGINE->jscIsNull (function) == false)
                        {
                            result = JSObjectCallAsFunction (JSC_JAVASCRIPT_ENGINE->globalContext, function, JSC_JAVASCRIPT_ENGINE->globalObj, numArgs, args, &exception);
                        }

                        return (result);
                    }
                };
            #endif

                typedef GuiEvent<> Event;
		}
	}
}

#endif
