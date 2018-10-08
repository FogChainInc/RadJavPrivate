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
#import "cpp/RadJavCPPMUITableView.h"

#import <UIKit/UIKit.h>
#import "cpp/RadJavCPPGUIGObject.h"
#import "cpp/RadJavCPPMUIButton.h"


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            TableViewFrame::TableViewFrame(GUI::GObject *parent, const String &text, const Vector2 &pos, const Vector2 &size)
            : widget([[UITableView alloc] init])
            {

            }
            
			TableViewFrame::~TableViewFrame()
			{
				//Release button here
				[widget release];
			}

			void TableViewFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String TableViewFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
			bool TableViewFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				return false;
			}
			
			#ifdef USE_IOS
				UIView* TableViewFrame::getNativeWidget()
				{
					return (UIView*)widget;
				}
			#elif defined USE_ANDROID
				void* TableViewFrame::getNativeWidget()
				{
					return widget;
				}
			#endif
        }
    }
}
