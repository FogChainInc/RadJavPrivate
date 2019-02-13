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
#include "cpp/ios/RadJavCPPGUITextareaFrame.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			TextareaFrame::TextareaFrame(GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			: widget([[UITextView alloc] init])
			{
				//TODO: add delegate
				//widgetDelegate = [[TextFieldDelegate alloc] init];
				//widgetDelegate.widget = this;
				//widget.delegate = widgetDelegate;
				
				if (parent)
					parent->addChild(this);
				
				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			TextareaFrame::~TextareaFrame()
			{
				[widget release];
				
				//TODO: add delegate
				//[widgetDelegate release];
			}
			
			void TextareaFrame::setText(String text)
			{
				NSString *objcString = [NSString stringWithUTF8String:text.c_str()];
				[widget setText:objcString];
			}
			
			String TextareaFrame::getText()
			{
				return [widget.text UTF8String];
			}
			
			void TextareaFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
				//[widget setFont:(UIFont * _Nullable)];
			}
			
			CPP::Font* TextareaFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void TextareaFrame::setEnabled(RJBOOL enabled)
			{
				[widget setUserInteractionEnabled:enabled];
			}
			
			RJBOOL TextareaFrame::getEnabled()
			{
				return widget.isUserInteractionEnabled;
			}
			
			bool TextareaFrame::bindEvent(const String& eventName, const CPP::Event* /*event*/)
			{
				//return [widgetDelegate bindEvent:widget eventName:eventName];
				return false;
			}
			
			UIView* TextareaFrame::getNativeWidget()
			{
				return widget;
			}
		}
	}
}

