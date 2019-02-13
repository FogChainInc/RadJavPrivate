/*
	MIT-LICENSE
	Copyright (c) 2017-2019 FogChain, Corp

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
#include "cpp/RadJavCPPEvent.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		Events::Events()
		{
			events = RJNEW HashMap<std::string, Event* >();
		}

		Events::~Events()
		{
			DELETEOBJ(events);
		}

		#ifdef GUI_USE_WXWIDGETS
		#ifdef USE_V8
		Event* Events::createEvent(String event, v8::Local<v8::Function> function)
		{
			// Create a persistent function to execute asych later.
			v8::Persistent<v8::Value> *persistent = RJNEW v8::Persistent<v8::Value>();
			persistent->Reset(function->GetIsolate(), function);

			Event* evt = RJNEW Event(persistent);

			if (events->size() > 0)
			{
				auto found = events->find(event);
				auto end = events->end();

				if (found != end)
				{
					Event *evtToRemove = events->at(event);
					DELETEOBJ(evtToRemove);

					events->erase(event);
				}
			}

			events->insert(HashMapPair<std::string, Event *>(event, evt));

			return evt;
		}

		void Events::addNewEvent(String event, wxWindow *object, v8::Local<v8::Function> func)
		{
		}
		#elif defined USE_JAVASCRIPTCORE
		Event* Events::createEvent(String event, JSObjectRef function)
		{
			// Create a persistent function to execute asych later.
			Event* evt = RJNEW Event(function);

			if (events->size() > 0)
			{
				auto found = events->find(event);
				auto end = events->end();

				if (found != end)
				{
					Event *evtToRemove = events->at(event);
					DELETEOBJ(evtToRemove);

					events->erase(event);
				}
			}

			events->insert(HashMapPair<std::string, Event *>(event, evt));

			return evt;
		}

		void Events::addNewEvent(String event, wxWindow *object, JSObjectRef func)
		{
		}
		#endif

		#elif defined USE_IOS || defined USE_ANDROID
		#ifdef USE_V8
		Event* Events::createEvent(String event, v8::Local<v8::Function> function)
		{
			// Create a persistent function to execute async later.
			v8::Persistent<v8::Value> *persistent = RJNEW v8::Persistent<v8::Value>();
			persistent->Reset(function->GetIsolate(), function);

			Event* evt = RJNEW Event(persistent);

			if (events->size() > 0)
			{
				auto found = events->find(event);
				auto end = events->end();

				if (found != end)
				{
					Event *evtToRemove = events->at(event);
					DELETEOBJ(evtToRemove);

					events->erase(event);
				}
			}

			events->insert(HashMapPair<std::string, Event *>(event, evt));

			return evt;
		}
		#elif defined USE_JAVASCRIPTCORE
		Event* Events::createEvent(String event, JSObjectRef function)
		{
			// Create a persistent function to execute asych later.
			Event* evt = RJNEW Event(function);

			if (events->size() > 0)
			{
				auto found = events->find(event);
				auto end = events->end();

				if (found != end)
				{
					Event *evtToRemove = events->at(event);
					DELETEOBJ(evtToRemove);

					events->erase(event);
				}
			}

			events->insert(HashMapPair<std::string, Event *>(event, evt));

			return evt;
		}
		#endif

		void Events::addNewEvent(String event,
		#ifdef USE_V8
			v8::Local<v8::Function> func
		#elif defined USE_JAVASCRIPTCORE
			JSObjectRef func
		#endif
		)
		{
			Event* evt = createEvent(event, func);

			bindEvent(event, evt);

			/* TODO: Add relevant implementation
			if (event == "click")
			{
				object->Bind(wxEVT_LEFT_UP, Events::onClick, -1, -1, createEvent(event, func));
			}

			if (event == "keyup")
			{
				object->Bind(wxEVT_KEY_UP, Events::onKeyUp, -1, -1, createEvent(event, func));
			}

			if (event == "keydown")
			{
				object->Bind(wxEVT_KEY_DOWN, Events::onKeyDown, -1, -1, createEvent(event, func));
			}

			if (event == "dblclick" || event == "doubleclick" )
			{
				object->Bind(wxEVT_LEFT_DCLICK, Events::onDoubleClick, -1, -1, createEvent(event, func));
			}

			if (event == "rightclick" || event == "contextmenu")
			{
				object->Bind(wxEVT_RIGHT_UP, Events::onRightClick, -1, -1, createEvent(event, func));
			}

			if (event == "mousedown")
			{
				object->Bind(wxEVT_AUX1_DOWN, Events::onMouseAux1Down, -1, -1, createEvent(event, func));
			}

			if (event == "mouseup")
			{
				object->Bind(wxEVT_AUX1_UP, Events::onMouseAux1Up, -1, -1, createEvent(event, func));
			}

			if (event == "mouseenter")
			{
				object->Bind(wxEVT_ENTER_WINDOW, Events::onMouseEnterWindow, -1, -1, createEvent(event, func));
			}

			if (event == "mouseleave")
			{
				object->Bind(wxEVT_LEAVE_WINDOW, Events::onMouseLeaveWindow, -1, -1, createEvent(event, func));
			}

			if (event == "mousemove")
			{
				object->Bind(wxEVT_MOTION, Events::onMouseMotion, -1, -1, createEvent(event, func));
			}

			if (event == "mousewheel")
			{
				object->Bind(wxEVT_MOUSEWHEEL, Events::onMouseWheel, -1, -1, createEvent(event, func));
			}

			if (event == "rightdblclick" || event == "rightdoubleclick")
			{
				object->Bind(wxEVT_RIGHT_DCLICK, Events::onRightDoubleClick, -1, -1, createEvent(event, func));
			}

			if (event == "rightdown")
			{
				object->Bind(wxEVT_RIGHT_DOWN, Events::onRightDown, -1, -1, createEvent(event, func));
			}

			if (event == "middleclick")
			{
				object->Bind(wxEVT_MIDDLE_UP, Events::onMiddleClick, -1, -1, createEvent(event, func));
			}

			if (event == "middledblclick" || event == "middledoubleclick")
			{
				object->Bind(wxEVT_MIDDLE_UP, Events::onMiddleDoubleClick, -1, -1, createEvent(event, func));
			}

			if (event == "middledown")
			{
				object->Bind(wxEVT_MIDDLE_DOWN, Events::onMiddleDown, -1, -1, createEvent(event, func));
			}

			if (event == "focus")
			{
				object->Bind(wxEVT_SET_FOCUS, Events::onFocusSet, -1, -1, createEvent(event, func));
			}

			if (event == "focusout")
			{
				object->Bind(wxEVT_KILL_FOCUS, Events::onFocusOut, -1, -1, createEvent(event, func));
			}
			 */
		}
		#endif

		#ifdef USE_V8
		v8::Local<v8::Value> Events::executeEvent(Event *pevent, RJINT numArgs, v8::Local<v8::Value> *args)
		{
			return (*pevent)(numArgs, args);
		}

		v8::Local<v8::Value> Events::executeEvent(const String& event, RJINT numArgs, v8::Local<v8::Value> *args)
		{
			if (events)
			{
				auto evt = events->find(event);
				if (evt != events->end())
				{
					return executeEvent(evt->second, numArgs, args);
				}
			}

			return v8::Undefined(V8_JAVASCRIPT_ENGINE->isolate);
		}

		void Events::executeEvent(const String &pevent, String message)
		{
			v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[1];
			args[0] = message.toV8String (V8_JAVASCRIPT_ENGINE->isolate);

			executeEvent(pevent, 1, args);

			DELETEARRAY(args);
		}

		#elif defined USE_JAVASCRIPTCORE
		JSValueRef Events::executeEvent(Event *pevent, RJINT numArgs, JSValueRef *args)
		{
			return (*pevent)(numArgs, args);
		}

		JSValueRef Events::executeEvent(const String& event, RJINT numArgs, JSValueRef *args)
		{
			if (events)
			{
				auto evt = events->find(event);
				if (evt != events->end())
				{
					return executeEvent(evt->second, numArgs, args);
				}
			}

			return JSValueMakeUndefined(JSC_JAVASCRIPT_ENGINE->globalContext);
		}

		void Events::executeEvent(const String &pevent, String message)
		{
			/// @todo Translate this over to JSC.
			/*v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[1];
			args[0] = message.toV8String(V8_JAVASCRIPT_ENGINE->isolate);

			executeEvent(pevent, 1, args);

			DELETEARRAY(args);*/
		}
		#endif
	}
}