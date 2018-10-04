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
#include "cpp/RadJavCPPMUILabel.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			LabelFrame::LabelFrame(GUI::GObject *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			: widget([[UILabel alloc] init])
			{
				[parent->_appObj->getNativeWidget() addSubview:widget];

				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			LabelFrame::~LabelFrame()
			{
				[widget release];
			}
			
			void LabelFrame::setText(String text)
			{
				NSString *objcString = [NSString stringWithUTF8String:text.c_str()];
				[widget setText:objcString];
			}
			
			String LabelFrame::getText()
			{
				return [widget.text UTF8String];
			}
			
			void LabelFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
				//[widget setFont:(UIFont * _Nullable)];
			}
			
			CPP::Font* LabelFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void LabelFrame::setEnabled(RJBOOL enabled)
			{
				[widget setEnabled:enabled];
			}
			
			RJBOOL LabelFrame::getEnabled()
			{
				return widget.isEnabled;
			}
			
			bool LabelFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				//TODO: do we need to handle UILabel events?
				//return [widgetDelegate bindEvent:widget eventName:eventName];
				return false;
			}
			
			#ifdef USE_IOS
				UIView* LabelFrame::getNativeWidget()
				{
					return widget;
				}
			#elif defined USE_ANDROID
				void* LabelFrame::getNativeWidget()
				{
					return widget;
				}
			#endif
		}
	}
}

