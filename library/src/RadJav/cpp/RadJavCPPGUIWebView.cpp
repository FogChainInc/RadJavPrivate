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
#include "cpp/RadJavCPPGUIWebView.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				WebViewFrame::WebViewFrame(wxWebView *webView)
					: GObjectBase()
				{
					this->webView = webView;
				}

				void WebViewFrame::onPageLoaded(wxWebViewEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void WebViewFrame::onPageChange(wxWebViewEvent &event)
				{
					v8::Persistent<v8::Value> *pevent = (v8::Persistent<v8::Value> *)event.GetEventUserData();
					String url = parsewxString(event.GetURL());
					v8::Local<v8::Value> *args = RJNEW v8::Local<v8::Value>[1];
					args[0] = url.toV8String(V8_JAVASCRIPT_ENGINE->isolate);

					v8::Local<v8::Value> result = executeEvent(pevent, 1, args);
					DELETE_ARRAY(args);

					if (result.IsEmpty() == false)
					{
						if ((result->IsNull() == false) && (result->IsUndefined() == false))
						{
							v8::Local<v8::Boolean> change = v8::Local<v8::Boolean>::Cast(result);

							if (change->Value() == false)
								event.Veto();
						}
					}
				}
			#endif

			#ifdef USE_V8
				WebView::WebView(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
				}
			#endif

			WebView::WebView(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void WebView::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					WebViewFrame *object = RJNEW WebViewFrame(wxWebView::New(parentWin, wxID_ANY, _text.towxString(), 
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height)));
					object->webView->Show(_visible);

					_appObj = object;

					setup();
				#endif
			}

			void WebView::setPosition(RJINT x, RJINT y)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						((WebViewFrame *)_appObj)->webView->SetPosition(wxPoint(x, y));
				#endif
			}

			CPP::Vector2 WebView::getPosition()
			{
				CPP::Vector2 newpos;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxPoint pos = ((WebViewFrame *)_appObj)->webView->GetPosition();
						newpos.x = pos.x;
						newpos.y = pos.y;
					}
				#endif

				return (newpos);
			}

			void WebView::setSize(RJINT width, RJINT height)
			{
				_transform->setSize(width, height);

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						((WebViewFrame *)_appObj)->webView->SetSize(width, height);
				#endif
			}

			CPP::Vector2 WebView::getSize()
			{
				CPP::Vector2 size = _transform->getSize();

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxSize wxsize = ((WebViewFrame *)_appObj)->webView->GetSize();
						size.x = wxsize.GetWidth();
						size.y = wxsize.GetHeight();
					}
				#endif

				return (size);
			}

			void WebView::setText(String text)
			{
				_text = text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						((WebViewFrame *)((WebViewFrame *)_appObj)->webView)->SetLabel(text.towxString());
				#endif
			}

			String WebView::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxString wxtext = ((WebViewFrame *)((WebViewFrame *)_appObj)->webView)->GetLabel();
						text = parsewxString(wxtext);
					}
				#endif

				return (_text);
			}

			void WebView::setVisibility(RJBOOL visible)
			{
				_visible = visible;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						if (visible == true)
							((WebViewFrame *)_appObj)->webView->Show();
						else
							((WebViewFrame *)_appObj)->webView->Hide();
					}
				#endif
			}

			RJBOOL WebView::getVisibility()
			{
				RJBOOL visible = _visible;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						visible = ((WebViewFrame *)_appObj)->webView->IsShown();
				#endif

				return (visible);
			}

			void WebView::setEnabled(RJBOOL enabled)
			{
				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						if (enabled == true)
							((WebViewFrame *)((WebViewFrame *)_appObj)->webView)->Enable();
						else
							((WebViewFrame *)((WebViewFrame *)_appObj)->webView)->Disable();
					}
				#endif
			}

			RJBOOL WebView::getEnabled()
			{
				RJBOOL enabled = false;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						enabled = ((WebViewFrame *)((WebViewFrame *)_appObj)->webView)->IsEnabled();
				#endif

				return (enabled);
			}

			#ifdef USE_V8
				void WebView::on(String event, v8::Local<v8::Function> func)
				{
					CPP::GUI::WebViewFrame *object = (CPP::GUI::WebViewFrame *)_appObj;

					object->addNewEvent(event, object, func);

					if (event == "pageLoad")
					{
						v8::Persistent<v8::Value> *pevent = object->createEvent(event, func);
						object->webView->Bind(wxEVT_WEBVIEW_LOADED, WebViewFrame::onPageLoaded, -1, -1, (wxObject *)pevent);
					}

					if (event == "pageChange")
					{
						v8::Persistent<v8::Value> *pevent = object->createEvent(event, func);
						object->webView->Bind(wxEVT_WEBVIEW_NAVIGATING, WebViewFrame::onPageChange, -1, -1, (wxObject *)pevent);
					}
				}
			#endif
		}
	}
}

