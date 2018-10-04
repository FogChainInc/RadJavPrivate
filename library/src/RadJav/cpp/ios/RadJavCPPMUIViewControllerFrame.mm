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
#import "cpp/RadJavCPPMUIViewController.h"

#import <UIKit/UIKit.h>
#import "cpp/RadJavCPPGUIGObject.h"
#import "cpp/RadJavCPPMUIButton.h"


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            
            ViewControllerFrame::ViewControllerFrame(){
                
                
            }
            UIView* ViewControllerFrame::getNativeWidget(){
                
                return (UIView*)widget;
            }
            ViewControllerFrame::ViewControllerFrame(void *parent, const String &text, const Vector2 &pos, const Vector2 &size)
            : widget([[UIViewController alloc] init])
            {
     
            }
            
            void ViewControllerFrame::setPosition(RJINT x, RJINT y)
            {
                //widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
            }
        
			
			ViewControllerFrame::~ViewControllerFrame()
			{
				//Release button here
				[widget release];
			}

            void ViewControllerFrame::addChild(GUI::GObject *child)
            {
				UIView* childObj = child->_appObj->getNativeWidget();
                [widget.view addSubview:childObj];
            }
            
            void ViewControllerFrame::presentViewControllerAnimated(CPP::GUI::GObject *presentedController){
                
                [widget presentViewController:(UIViewController*)presentedController->_appObj->getNativeWidget() animated:YES completion:^{}];
            }
            
            void ViewControllerFrame::dismissViewControllerAnimated(){
                
                [widget dismissViewControllerAnimated:YES completion:^{}];
            }
            
            void ViewControllerFrame::makeRootViewController(){
                UIWindow * keyWindow = [UIApplication sharedApplication].keyWindow;
                [keyWindow setRootViewController:widget];
            }
            
            
			void ViewControllerFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* ViewControllerFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
						
			void ViewControllerFrame::setPosition(CPP::Vector2 pos)
			{
				//TODO: Add implementation
			}
			
			CPP::Vector2 ViewControllerFrame::getPosition()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ViewControllerFrame::getX()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ViewControllerFrame::getY()
			{
				//TODO: Add implementation
				return 0;
			}
			
			void ViewControllerFrame::setSize(RJINT width, RJINT height)
			{
				//widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
			}
			
			void ViewControllerFrame::setSize(CPP::Vector2 size)
			{
				//widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, size.x, size.y);
			}
			
			CPP::Vector2 ViewControllerFrame::getSize()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ViewControllerFrame::getWidth()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ViewControllerFrame::getHeight()
			{
				//TODO: Add implementation
				return 0;
			}
			
			void ViewControllerFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String ViewControllerFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
			GUI::GObject* ViewControllerFrame::getParent()
			{
				//TODO: Add implementation
				return nullptr;
			}

			void ViewControllerFrame::setVisibility(RJBOOL visible)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ViewControllerFrame::getVisibility()
			{
				//TODO: Add implementation
				return false;
			}
			
			void ViewControllerFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ViewControllerFrame::getEnabled()
			{
				//TODO: Add implementation
				return false;
			}
			
			bool ViewControllerFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				
				return false;
			}
        }
    }
}

