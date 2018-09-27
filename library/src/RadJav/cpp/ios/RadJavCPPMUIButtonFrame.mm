#include "cpp/RadJavCPPMUIButton.h"
#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			ButtonFrame::ButtonFrame(void *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			: widget([[UIButton alloc] init])
			{

                UIWindow * keyWindow = [UIApplication sharedApplication].keyWindow;
                UIView * greenView = keyWindow.rootViewController.view ;
                [greenView  addSubview:widget];
                
                [widget setTitle:@"Button" forState:UIControlStateNormal];
                [widget setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];

			}
			
            
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
                //widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
            }
            
            String ButtonFrame::getText(){
                
                
            }
            
            void ButtonFrame::setVisibility(RJBOOL visible){
                
                
            }
            RJBOOL ButtonFrame::getVisibility(){
                
            }
            
            
            
			ButtonFrame::~ButtonFrame()
			{
				//Release button here
				[widget release];
			}
		}
	}
}

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            
            ViewFrame::ViewFrame(){
                
                
            }
            
            ViewFrame::ViewFrame(void *parent, const String &text, const Vector2 &pos, const Vector2 &size)
            : widget([[UIView alloc] init])
            {
                UIWindow * keyWindow = [UIApplication sharedApplication].keyWindow;
                
                UIView * greenView = keyWindow.rootViewController.view ;
                [greenView  addSubview:widget];
                widget.backgroundColor = [UIColor redColor];
            }
            
            void ViewFrame::setSize(RJINT width, RJINT height)
            {
                widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
            }
            void ViewFrame::setPosition(RJINT x, RJINT y)
            {
                widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
            }
            
            ViewFrame::~ViewFrame()
            {
                //Release button here
                [widget release];
            }
        }
    }
}


