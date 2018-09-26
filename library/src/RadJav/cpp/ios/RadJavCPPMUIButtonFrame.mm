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
				//Set properties for UIButton here from constructor parameters
				//[widget setTitle:(nullable NSString *) forState:(UIControlState)]
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
                widget.backgroundColor = [UIColor redColor];
                UIWindow * keyWindow = [UIApplication sharedApplication].keyWindow;
                
                UIView * greenView = keyWindow.rootViewController.view ;
                [greenView  addSubview:widget];
                widget.center = greenView.center;
            }
            
            void ViewFrame::setSize(RJINT width, RJINT height)
            {
                widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
            }
            
            
            ViewFrame::~ViewFrame()
            {
                //Release button here
                [widget release];
            }
        }
    }
}


