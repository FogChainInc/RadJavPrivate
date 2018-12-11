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
#import "cpp/RadJavCPPMUIView.h"

#import <UIKit/UIKit.h>
#import "cpp/RadJavCPPGUIGObject.h"


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            ViewFrame::ViewFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
            : widget([[UIView alloc] init])
            {
				if (parent)
				{
					parent->addChild(this);
				}
				
				setText(text);
            }
            
			ViewFrame::ViewFrame(const String &text, const Vector2 &pos, const Vector2 &size)
			: widget([[UIView alloc] init])
			{
				UIWindow * keyWindow = [UIApplication sharedApplication].keyWindow;
				
				UIView * rootView = keyWindow.rootViewController.view;
				[rootView  addSubview:widget];

				setText(text);
			}

			ViewFrame::~ViewFrame()
			{
				//Release native widget here
				[widget release];
			}

			void ViewFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String ViewFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
			bool ViewFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				return false;
			}
			
			UIView* ViewFrame::getNativeWidget()
			{
				return widget;
			}
        }
    }
}
