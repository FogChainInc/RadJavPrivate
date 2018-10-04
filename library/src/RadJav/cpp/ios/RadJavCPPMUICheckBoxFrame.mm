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
#include "cpp/RadJavCPPMUICheckBox.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			CheckBoxFrame::CheckBoxFrame(GUI::GObject *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			: widget([[UISwitch alloc] init])
			{
				widgetDelegate = [[SwitchDelegate alloc] init];
				widgetDelegate.widget = this;

				[parent->_appObj->getNativeWidget() addSubview:widget];
				
				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			CheckBoxFrame::~CheckBoxFrame()
			{
				[widget release];
			}
			
			#ifdef USE_IOS
				UIView* CheckBoxFrame::getNativeWidget()
				{
					return widget;
				}
			#elif defined USE_ANDROID
				void* CheckBoxFrame::getNativeWidget()
				{
					return widget;
				}
			#endif
			
			void CheckBoxFrame::setChecked(RJBOOL checked)
			{
				[widget setOn:checked];
			}
			
			RJBOOL CheckBoxFrame::getChecked() const
			{
				return widget.on;
			}

			void CheckBoxFrame::setSize(RJINT width, RJINT height)
			{
				widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
			}
			void CheckBoxFrame::setPosition(RJINT x, RJINT y)
			{
				widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
			}
			
			void CheckBoxFrame::setText(String text)
			{
				//No need
			}
			
			String CheckBoxFrame::getText()
			{
				return String();
			}
			
			void CheckBoxFrame::setVisibility(RJBOOL visible)
			{
				[widget setHidden:!visible];
			}
			
			RJBOOL CheckBoxFrame::getVisibility()
			{
				return !widget.isHidden;
			}
			
			void CheckBoxFrame::addChild(GUI::GObject *child)
			{
				UIView* childObj = child->_appObj->getNativeWidget();
				[widget addSubview:childObj];
			}
			
			void CheckBoxFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
				//[widget setFont:(UIFont * _Nullable)];
			}
			
			CPP::Font* CheckBoxFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void CheckBoxFrame::setPosition(CPP::Vector2 pos)
			{
				setPosition(pos.x, pos.y);
			}
			
			CPP::Vector2 CheckBoxFrame::getPosition()
			{
				return Vector2(getX(), getY());
			}
			
			RJINT CheckBoxFrame::getX()
			{
				return widget.frame.origin.x;
			}
			
			RJINT CheckBoxFrame::getY()
			{
				return widget.frame.origin.y;
			}
			
			void CheckBoxFrame::setSize(CPP::Vector2 size)
			{
				setSize(size.x, size.y);
			}
			
			CPP::Vector2 CheckBoxFrame::getSize()
			{
				return Vector2(getWidth(), getHeight());
			}
			
			RJINT CheckBoxFrame::getWidth()
			{
				return widget.frame.size.width;
			}
			
			RJINT CheckBoxFrame::getHeight()
			{
				return widget.frame.size.height;
			}
			
			GUI::GObject* CheckBoxFrame::getParent()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void CheckBoxFrame::setEnabled(RJBOOL enabled)
			{
				[widget setEnabled:enabled];
			}
			
			RJBOOL CheckBoxFrame::getEnabled()
			{
				return widget.isEnabled;
			}
			
			bool CheckBoxFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				return [widgetDelegate bindEvent:widget eventName:eventName];
			}
		}
	}
}

