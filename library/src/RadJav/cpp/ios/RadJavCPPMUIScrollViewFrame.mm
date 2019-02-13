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
#include "cpp/ios/RadJavCPPMUIScrollViewFrame.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			ScrollViewFrame::ScrollViewFrame(GUI::GObjectWidget *parent, const Vector2 &pos, const Vector2 &size)
			: widget([[UIScrollView alloc] init])
			{
				//TODO: add delegate
				
				if (parent)
					parent->addChild(this);
				
				setSize(size);
				setPosition(pos);
			}
			
			ScrollViewFrame::~ScrollViewFrame()
			{
				[widget release];
				
				//TODO: add delegate
				//[widgetDelegate release];
			}
			
			void ScrollViewFrame::setEnabled(RJBOOL enabled)
			{
				[widget setUserInteractionEnabled:enabled];
			}
			
			RJBOOL ScrollViewFrame::getEnabled()
			{
				return widget.isUserInteractionEnabled;
			}

			void ScrollViewFrame::addChild(GUI::GObjectWidget *child)
			{
				GObjectWidget::addChild(child);
				CPP::Vector2 size = child->getSize();
				
				[widget setContentSize: CGSizeMake(size.x, size.y)];
			}

			//TODO: add ScrollView implementation for iOS. Android implementation is available
//			void ScrollViewFrame::setContentSize(const CPP::Vector2& size)
//			{
//				[widget setContentSize: CGSizeMake(size.x, size.y)];
//			}
//
//			CPP::Vector2 ScrollViewFrame::getContentSize() const
//			{
//				return Vector2(widget.contentSize.width, widget.contentSize.height);
//			}

			bool ScrollViewFrame::bindEvent(const String& eventName, const CPP::Event* /*event*/)
			{
				//return [widgetDelegate bindEvent:widget eventName:eventName];
				return false;
			}
			
			#ifdef USE_IOS
				UIView* ScrollViewFrame::getNativeWidget()
				{
					return widget;
				}
			#elif defined USE_ANDROID
				void* ScrollViewFrame::getNativeWidget()
				{
					return widget;
				}
			#endif
		}
	}
}

