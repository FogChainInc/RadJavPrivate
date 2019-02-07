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
#include "cpp/desktop/RadJavCPPGUIWebViewFrame.h"

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
				{
					this->webView = webView;
				}

				void WebViewFrame::onPageLoaded(wxWebViewEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}

				void WebViewFrame::onPageChange(wxWebViewEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					String url = parsewxString(event.GetURL());
					
					#ifdef USE_V8
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
					#endif
					
					#ifdef USE_JAVASCRIPTCORE
					/// @todo Fix this.
					#endif
				}

				void WebViewFrame::onPageNavigated(wxWebViewEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void WebViewFrame::onPageNavigationError(wxWebViewEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void WebViewFrame::onNewWindow(wxWebViewEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void WebViewFrame::onTitleChanged(wxWebViewEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
			#endif
		}
	}
}
