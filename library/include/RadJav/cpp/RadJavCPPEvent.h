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
#ifndef _RADJAV_CPP_EVENT_H_
	#define _RADJAV_CPP_EVENT_H_

	#include "RadJav.h"
	#include "RadJavString.h"
	#include "RadJavHashMap.h"

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
			/**
			* @ingroup group_gui_cpp
			* @brief VariantObject class.
			*/
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
				class RADJAV_EXPORT AppEvent : public VariantObject<UserData>
				{
					public:
						AppEvent(UserData data): VariantObject<UserData>(data) {}
                    
						v8::Local<v8::Value> operator ()(RJINT numArgs = 0, v8::Local<v8::Value> *args = NULL)
						{
							// Execute a persistent function.
							v8::Local<v8::Value> functionValue = VariantObject<UserData>::object->Get(V8_JAVASCRIPT_ENGINE->isolate);
							v8::Local<v8::Function> function = v8::Local<v8::Function>::Cast(functionValue);
							v8::Local<v8::Value> result = v8::Undefined (V8_JAVASCRIPT_ENGINE->isolate);
                        
							if (V8_JAVASCRIPT_ENGINE->v8IsNull(function) == false)
								result = function->Call(V8_JAVASCRIPT_ENGINE->globalContext->Global(), numArgs, args);
                        
							return (result);
						}
				};
			#endif

			#ifdef USE_JAVASCRIPTCORE
				template<class UserData = JSObjectRef >
				class RADJAV_EXPORT AppEvent : public VariantObject<UserData>
				{
					public:
						AppEvent(UserData data): VariantObject<UserData>(data)
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

			typedef AppEvent<> Event;

			/// Base class for events utils/handling
			class RADJAV_EXPORT Events
			{
				public:
					Events();
					Events(const Events& other) = delete;
					Events& operator =(const Events& other) = delete;

					virtual ~Events();

					/// Handle events.
					virtual void cppOn(const String &event, std::function<void *(Array<void *>)> function);

					/// Create a C++ event.
					virtual void createCppEvent(const String &event, std::function<void *(Array<void *>)> function);

					#ifdef GUI_USE_WXWIDGETS
						#ifdef USE_V8
							/// Create a V8 JS event.
							virtual Event* createEvent(String event, v8::Local<v8::Function> function);
							/// Create a V8 JS wxWidgets event.
							virtual void addNewEvent(String event, wxWindow *object, v8::Local<v8::Function> func);
						#elif defined USE_JAVASCRIPTCORE
							/// Create a JavaScriptCore JS event.
							virtual Event* createEvent(String event, JSObjectRef function);
							/// Create a JavaScriptCore JS wxWidgets event.
							virtual void addNewEvent(String event, wxWindow *object, JSObjectRef func);
						#endif
					#endif

					#if defined USE_IOS || defined USE_ANDROID
						#ifdef USE_V8
							virtual Event* createEvent(String event, v8::Local<v8::Function> function);
							virtual void addNewEvent(String event, v8::Local<v8::Function> func);
						#elif defined USE_JAVASCRIPTCORE
							virtual Event* createEvent(String event, JSObjectRef function);
							virtual void addNewEvent(String event, JSObjectRef func);
						#endif

						/// @todo Add iOS and Android specific events handling
						virtual bool bindEvent(const String& eventName, const Event* event) = 0;
					#endif

					#ifdef USE_V8
						/// Call a V8 JS event.
						static v8::Local<v8::Value> executeEvent(Event *pevent, RJINT numArgs = 0, v8::Local<v8::Value> *args = NULL);
						/// Call a V8 JS event.
						v8::Local<v8::Value> executeEvent(const String& event, RJINT numArgs = 0, v8::Local<v8::Value> *args = NULL);
					#elif defined USE_JAVASCRIPTCORE
						/// Call a JavaScriptCore JS event.
						static JSValueRef executeEvent(Event *pevent, RJINT numArgs = 0, JSValueRef *args = NULL);
						/// Call a JavaScriptCore JS event.
						JSValueRef executeEvent(const String& event, RJINT numArgs = 0, JSValueRef *args = NULL);
					#endif

					/// Call an event. Executes events for both JavaScript and C++.
					void executeEvent(const String &event, String message);

					/// Call a C++ event.
					void *executeCppEvent(const String &event);
					/// Call a C++ event.
					void *executeCppEvent(const String &event, const String &data);
					/// Call a C++ event.
					void *executeCppEvent(const String &event, Array<void *> args);

					/// The JavaScript events.
					HashMap<std::string, Event* > *events;
					/// The C++ events.
					HashMap<std::string, std::function<void *(Array<void *>)> > *cppEvents;
			};
		}
	}
#endif
