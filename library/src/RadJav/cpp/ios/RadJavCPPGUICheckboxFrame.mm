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
#include "cpp/ios/RadJavCPPGUICheckboxFrame.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			CheckboxFrame::CheckboxFrame(GObjectWidget *parent, RJBOOL checked, String text, const Vector2 &pos, const Vector2 &size)
			: widget([[UISwitch alloc] init])
			{
				widgetDelegate = [[SwitchDelegate alloc] init];
				widgetDelegate.widget = this;

				if (parent)
					parent->addChild(this);
				
				setChecked(checked);
				setSize(size);
				setPosition(pos);
			}
			
			CheckboxFrame::~CheckboxFrame()
			{
				[widget release];
				[widgetDelegate release];
			}
			
			void CheckboxFrame::setChecked(RJBOOL checked)
			{
				[widget setOn:checked];
			}
			
			RJBOOL CheckboxFrame::isChecked() const
			{
				return widget.on;
			}

			void CheckboxFrame::setEnabled(RJBOOL enabled)
			{
				[widget setEnabled:enabled];
			}
			
			RJBOOL CheckboxFrame::getEnabled()
			{
				return widget.isEnabled;
			}
			
			bool CheckboxFrame::bindEvent(const String& eventName, const CPP::Event* /*event*/)
			{
				return [widgetDelegate bindEvent:widget eventName:eventName];
			}
			
			UIView* CheckboxFrame::getNativeWidget()
			{
				return widget;
			}
		}
	}
}

