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
#include "cpp/RadJavCPPMUIWebView.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"

#import "RadJavString.h"

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			WebViewFrame::WebViewFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
                //WKWebViewConfiguration *config = [[WKWebViewConfiguration alloc] init];
                //widget = [[WKWebView alloc] initWithFrame: parent->_appObj->getNativeWidget().frame configuration: config];
                widget = [[WKWebView alloc] init];
                widget.navigationDelegate = navDelegate;

				widgetDelegate = [[WebViewDelegate alloc] init];
				widgetDelegate.widget = this;
				widget.UIDelegate = widgetDelegate;

				[parent->getNativeWidget() addSubview: widget];
				
				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			WebViewFrame::~WebViewFrame()
			{
				[widget release];
				[widgetDelegate release];
			}
			
			void WebViewFrame::setText(String text)
			{
                NSString *nsstr = text.toNSString();
                NSURL *url = [NSURL URLWithString: nsstr];
                NSURLRequest *request = [NSURLRequest requestWithURL: url];
                [widget loadRequest: request];

                [nsstr release];
			}
			
			String WebViewFrame::getText()
			{
                NSURL *url = [widget URL];
                NSString *strurl = [[NSString alloc] initWithString: [url path]];
                String urlRJStr = parseNSString (strurl);
                
                [strurl release];

				return (urlRJStr);
			}
			
			void WebViewFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
				//[widget setFont:(UIFont * _Nullable)];
			}
			
			CPP::Font* WebViewFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void WebViewFrame::setEnabled(RJBOOL enabled)
			{
			}
			
			RJBOOL WebViewFrame::getEnabled()
			{
				return true;
			}

			bool WebViewFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				return [widgetDelegate bindEvent:widget eventName:eventName];
			}
			
			#ifdef USE_IOS
				UIView* WebViewFrame::getNativeWidget()
				{
					return widget;
				}
			#elif defined USE_ANDROID
				void* WebViewFrame::getNativeWidget()
				{
					return widget;
				}
			#endif
		}
	}
}

