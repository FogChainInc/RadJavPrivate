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
#include "cpp/ios/RadJavCPPMUIButtonFrame.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			ButtonFrame::ButtonFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			: widget([[UIButton alloc] init])
			{
                widgetDelegate = [[ButtonDelegate alloc] init];
                widgetDelegate.widget = this;
				
                id parentWidget = parent->getNativeWidget();
                
                if ([parentWidget isKindOfClass:[UIView class]]){
                    [parentWidget addSubview:widget];
                }
                else {
                    [[parentWidget view] addSubview:widget];
                }
				

				[widget setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
				
				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			ButtonFrame::~ButtonFrame()
			{
				[widget release];
				[widgetDelegate release];
			}

            void ButtonFrame::setText(String text)
            {
                const char * cString = text.c_str();
                NSString *objcString = [NSString stringWithUTF8String:cString];
                [widget setTitle:objcString forState:UIControlStateNormal];
            }
            
            String ButtonFrame::getText()
			{
				NSString* title = [widget currentTitle];
				return String([title UTF8String]);
            }
            
			void ButtonFrame::setFont(CPP::Font *font)
			{
                widget.titleLabel.font = font->toUIFont ();

                if (font->underline == true)
                {
                    NSString* title = [widget currentTitle];
                    NSMutableAttributedString *attrText = [[NSMutableAttributedString alloc] initWithString: title];
                    [attrText addAttribute: NSUnderlineStyleAttributeName value: [NSNumber numberWithInt: 1] range: (NSRange){0, [attrText length]}];
                    widget.titleLabel.attributedText = attrText;
                }
			}
			
			CPP::Font* ButtonFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}

			void ButtonFrame::setEnabled(RJBOOL enabled)
			{
				[widget setEnabled:enabled];
			}
			
			RJBOOL ButtonFrame::getEnabled()
			{
				return widget.isEnabled;
			}
			
			bool ButtonFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				return [widgetDelegate bindEvent:widget eventName:eventName];
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
		}
	}
}

