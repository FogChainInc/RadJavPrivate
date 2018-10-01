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
#include "cpp/RadJavCPPMUIButton.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			ButtonFrame::ButtonFrame(GUI::GObject *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			: widget([[UIButton alloc] init])
			{
                widgetDelegate = [[ButtonDelegate alloc] init];
                widgetDelegate.widget = this;
				
				[parent->_appObj->getNativeWidget() addSubview:widget];

				[widget setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
				
				setSize(size);
				setPosition(pos);
			}
			
			ButtonFrame::~ButtonFrame()
			{
				//Release button here
				[widget release];
			}

			#ifdef USE_IOS
				UIView* ButtonFrame::getNativeWidget()
				{
					return widget;
				}
			#elif defined USE_ANDROID
				void* ButtonFrame::getNativeWidget()
				{
					return widget;
				}
			#endif

            void ButtonFrame::setSize(RJINT width, RJINT height)
            {
                widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
            }
            void ButtonFrame::setPosition(RJINT x, RJINT y)
            {
                widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
            }
            
            void ButtonFrame::setText(String text)
            {
                const char * cString = text.c_str();
                NSString *objcString = [NSString stringWithUTF8String:cString];
                [widget setTitle:objcString forState:UIControlStateNormal];
            }
            
            String ButtonFrame::getText(){
                
                
            }
            
            void ButtonFrame::setVisibility(RJBOOL visible){
                
                
            }
            RJBOOL ButtonFrame::getVisibility(){
                
            }
            
            
            
			
			void ButtonFrame::addChild(GUI::GObject *child)
			{
				UIView* childObj = child->_appObj->getNativeWidget();
				[widget addSubview:childObj];
			}



			void ButtonFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* ButtonFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}


			

			void ButtonFrame::setPosition(CPP::Vector2 pos)
			{
				setPosition(pos.x, pos.y);
			}
			
			CPP::Vector2 ButtonFrame::getPosition()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ButtonFrame::getX()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ButtonFrame::getY()
			{
				//TODO: Add implementation
				return 0;
			}
	
			void ButtonFrame::setSize(CPP::Vector2 size)
			{
				setSize(size.x, size.y);
			}
			
			CPP::Vector2 ButtonFrame::getSize()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ButtonFrame::getWidth()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ButtonFrame::getHeight()
			{
				//TODO: Add implementation
				return 0;
			}
	
			
			GUI::GObject* ButtonFrame::getParent()
			{
				//TODO: Add implementation
				return nullptr;
			}


			
			void ButtonFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ButtonFrame::getEnabled()
			{
				//TODO: Add implementation
				return false;
			}
			
			bool ButtonFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				return [widgetDelegate bindEvent:widget eventName:eventName];
			}
		}
	}
}

