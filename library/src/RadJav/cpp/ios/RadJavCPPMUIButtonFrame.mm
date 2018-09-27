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
			
//			void ButtonFrame::addChild(GObject *child)
//			{
//				//TODO: Add implementation
//			}

			void ButtonFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* ButtonFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}

			void ButtonFrame::setPosition(RJINT x, RJINT y)
			{
				//TODO: Add implementation
			}

			void ButtonFrame::setPosition(CPP::Vector2 pos)
			{
				//TODO: Add implementation
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
			
			void ButtonFrame::setSize(RJINT width, RJINT height)
			{
				//TODO: Add implementation
			}
			
			void ButtonFrame::setSize(CPP::Vector2 size)
			{
				//TODO: Add implementation
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
			
			void ButtonFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String ButtonFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
//			GObject* ButtonFrame::getParent()
//			{
//				//TODO: Add implementation
//				return nullptr;
//			}
			
			void ButtonFrame::setVisibility(RJBOOL visible)
			{
				//TODO: Add implementation
			}
			
			RJBOOL ButtonFrame::getVisibility()
			{
				//TODO: Add implementation
				return false;
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
		}
	}
}
