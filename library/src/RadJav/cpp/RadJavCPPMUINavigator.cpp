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
#include "cpp/RadJavCPPMUINavigator.h"
#include "cpp/RadJavCPPMUIView.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			#ifdef GUI_USE_WXWIDGETS
				NavigatorFrame::NavigatorFrame(wxWindow *parent, const wxString &text, const wxPoint &pos, const wxSize &size)
					: wxStaticText(parent, wxID_ANY, text, pos, size)
				{
				}

				void NavigatorFrame::onClick(wxMouseEvent &event)
				{
					CPP::GUI::Event *pevent = (CPP::GUI::Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
			#endif

			#ifdef USE_V8
				Navigator::Navigator(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject(jsEngine, args), rootView(nullptr)
				{
					#ifdef RADJAV_MOBILE
						impl = nullptr;
					#endif

					v8::Local<v8::Object> rootWinJs = jsEngine->v8GetObject(args.This(), "rootWin");
					if(!jsEngine->v8IsNull(rootWinJs))
					{
						rootView = (View*)jsEngine->v8GetExternal(rootWinJs, "_appObj");
					}
				}
			#elif defined USE_JAVASCRIPTCORE
				Navigator::Navigator(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
					: GObject(jsEngine, thisObj, numArgs, args), rootView(nullptr)
            	{
					#ifdef RADJAV_MOBILE
						impl = nullptr;
					#endif

					JSObjectRef rootWinJs = jsEngine->jscGetObject(thisObj, "rootWin");
					if(!jsEngine->jscIsNull(rootWinJs))
					{
						rootView = (View*)jsEngine->jscGetExternal(jsEngine->globalContext, rootWinJs, "_appObj");
					}
            	}
            #endif

			Navigator::Navigator(View* view, String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
				rootView = view;

				#ifdef RADJAV_MOBILE
					impl = nullptr;
				#endif
			}

			void Navigator::create()
			{
				#ifdef RADJAV_MOBILE
					ViewFrame* root = nullptr;

					if(rootView)
						root = static_cast<ViewFrame*>(rootView->_appObj);

					impl = RJNEW NavigatorFrame(root);
					linkWith(impl);
				#endif

				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					NavigatorFrame *object = RJNEW NavigatorFrame(parentWin, _text.towxString(),
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);

					_appObj = object;
					linkWith(object);
					setup();
				#endif
			}

			void Navigator::push(View* view, bool replace)
			{
				#ifdef RADJAV_MOBILE
					//We can't deal with parented Views
					if (view->getParent())
						return;

					if (impl)
						impl->push(view ? static_cast<ViewFrame*>(view->_appObj) : nullptr, replace);
				#endif
			}

			void Navigator::pop(View* view)
			{
				#ifdef RADJAV_MOBILE
					//We can't deal with parented Views
					if (view->getParent())
						return;

					if (impl)
						impl->pop(view ? static_cast<ViewFrame*>(view->_appObj) : nullptr);
				#endif
			}

			void Navigator::pop()
			{
				#ifdef RADJAV_MOBILE
					if (impl)
						impl->pop();
				#endif
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void Navigator::on(String event, RJ_FUNC_TYPE func)
				{
					#ifdef RADJAV_MOBILE
						if (_appObj)
						{
							_appObj->addNewEvent(event, func);
						}
					#endif

					#ifdef GUI_USE_WXWIDGETS
					#endif
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::MUI::NavigatorFrame, wxStaticText)
	wxEND_EVENT_TABLE()
#endif

