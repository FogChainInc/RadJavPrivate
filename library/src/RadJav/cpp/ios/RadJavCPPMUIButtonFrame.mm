#include "cpp/RadJavCPPMUIButton.h"

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

