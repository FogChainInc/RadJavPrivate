#import "cpp/RadJavCPPMUIView.h"

#import <UIKit/UIKit.h>

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
			ViewFrame::ViewFrame()
			{
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
			
			ViewFrame::~ViewFrame()
			{
				//Release button here
				[widget release];
			}

//			void ViewFrame::addChild(GObject *child)
//			{
//				//TODO: Add implementation
//			}

			void ViewFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* ViewFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void ViewFrame::setPosition(RJINT x, RJINT y)
			{
				//TODO: Add implementation
			}
			
			void ViewFrame::setPosition(CPP::Vector2 pos)
			{
				//TODO: Add implementation
			}
			
			CPP::Vector2 ViewFrame::getPosition()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ViewFrame::getX()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ViewFrame::getY()
			{
				//TODO: Add implementation
				return 0;
			}
			
			void ViewFrame::setSize(RJINT width, RJINT height)
			{
				widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
			}
			
			void ViewFrame::setSize(CPP::Vector2 size)
			{
				widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, size.x, size.y);
			}
			
			CPP::Vector2 ViewFrame::getSize()
			{
				//TODO: Add implementation
				return Vector2();
			}
			
			RJINT ViewFrame::getWidth()
			{
				//TODO: Add implementation
				return 0;
			}
			
			RJINT ViewFrame::getHeight()
			{
				//TODO: Add implementation
				return 0;
			}
			
			void ViewFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String ViewFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
//			GObject* ViewFrame::getParent()
//			{
//				//TODO: Add implementation
//				return nullptr;
//			}

			void ViewFrame::setVisibility(RJBOOL visible)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ViewFrame::getVisibility()
			{
				//TODO: Add implementation
				return false;
			}
			
			void ViewFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ViewFrame::getEnabled()
			{
				//TODO: Add implementation
				return false;
			}
        }
    }
}

