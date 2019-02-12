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
#ifndef _RADJAV_GUI_CPP_WEBVIEW_FRAME_H_
	#define _RADJAV_GUI_CPP_WEBVIEW_FRAME_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "cpp/RadJavCPPGUIGObject.h"

	#ifdef GUI_USE_WXWIDGETS
		#include <wx/wx.h>
		#include <wx/webview.h>
	#endif

	namespace RadJAV
	{
		namespace CPP
		{
			namespace GUI
			{
				#ifdef WXWIDGETS_HAS_WEBVIEW
					#ifdef GUI_USE_WXWIDGETS
						/// The wxWidgets button to use.
						class RADJAV_EXPORT WebViewFrame : public wxWindow, public GObjectWidget, public ChainedPtr
						{
							public:
								WebViewFrame(wxWebView *webView);

								static void onPageLoaded(wxWebViewEvent &event);
								static void onPageChange(wxWebViewEvent &event);
								static void onPageNavigated(wxWebViewEvent &event);
								static void onPageNavigationError(wxWebViewEvent &event);
								static void onNewWindow(wxWebViewEvent &event);
								static void onTitleChanged(wxWebViewEvent &event);

								wxWindow* getNativeWidget();

								wxWebView *webView;
						};
					#endif
				#endif
			}
		}
	}
#endif
