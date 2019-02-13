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
#import "cpp/ios/RadJavCPPMUITableViewFrame.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"
#import <UIKit/UIKit.h>
#import "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            TableViewFrame::TableViewFrame(GUI::GObjectWidget *parent,
										   TableViewCellCreator& cellCreator,
										   const String &text,
										   const Vector2 &pos,
										   const Vector2 &size)
            : widget([[[UITableView alloc] init]retain])
            {
                [widget registerClass:[UITableViewCell class] forCellReuseIdentifier:@"identifier"];
                widgetDelegate = [[TableViewDelegate alloc] init];
                widgetDelegate.widget = this;
				//TODO: pass it by reference to Objective-C code
				widgetDelegate.cellCreator = &cellCreator;
                widget.dataSource = widgetDelegate;
                widget.delegate = widgetDelegate;
				
				if (parent)
					parent->addChild(this);
            }
            
            void TableViewFrame::setModel(MUI::TableViewModel *model)
			{
                this->model = model;
                widgetDelegate.model = model;
                reload();
            }
            
            void TableViewFrame::reload()
			{
                [widget reloadData];
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
				 return [widgetDelegate bindEvent:widget eventName:eventName];
			}
			
			View* TableViewFrame::viewForCellModel(int index)
			{
				return tableView->createViewForItem(index);
			}
            
			UIView* TableViewFrame::getNativeWidget()
			{
				return (UIView*)widget;
			}
        }
    }
}
