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
#ifndef _RADJAV_MUI_CPP_WEBVIEW_FRAME_H_
#define _RADJAV_MUI_CPP_WEBVIEW_FRAME_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIGObject.h"
#include "cpp/RadJavCPPEvent.h"

OBJC_CLASS(WKWebView);
OBJC_CLASS(WebViewDelegate);
OBJC_CLASS(WebViewNavigationDelegate);

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			class RADJAV_EXPORT WebViewFrame : public GUI::GObjectWidget
												,public ChainedPtr
			{
			public:
				WebViewFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size);
				~WebViewFrame();
				
				void setText(String text);
				String getText();
				void setFont(CPP::Font *font);
				CPP::Font *getFont();
				void setEnabled(RJBOOL enabled);
				RJBOOL getEnabled();

				bool bindEvent(const String& eventName, const CPP::Event* event);
				
				UIView* getNativeWidget();
				
			private:
				WKWebView* widget;
				WebViewDelegate* widgetDelegate;
				WebViewNavigationDelegate* navDelegate;
			};
		}
	}
}
#endif
