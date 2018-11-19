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
#include "cpp/RadJavCPPGUIGObject.h"

#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
            GObjectWidget::GObjectWidget ()
            {
                _parent = NULL;
            }

			void GObjectWidget::addChild(GObject *child)
			{
				UIView* widget = getNativeWidget();
				UIView* childObj = child->_appObj->getNativeWidget();
				[widget addSubview:childObj];
			}
			
			void GObjectWidget::setFont(CPP::Font *font)
			{
				//Do nothing here
			}
			
			CPP::Font* GObjectWidget::getFont()
			{
				//Do nothing here
				return nullptr;
			}
			
			void GObjectWidget::setPosition(RJINT x, RJINT y)
			{
				UIView* widget = getNativeWidget();
				widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
			}
			
			void GObjectWidget::setPosition(CPP::Vector2 pos)
			{
				setPosition(pos.x, pos.y);
			}
			
			CPP::Vector2 GObjectWidget::getPosition()
			{
				return Vector2(getX(), getY());
			}
			
			RJINT GObjectWidget::getX()
			{
				UIView* widget = getNativeWidget();
				return widget.frame.origin.x;
			}
			
			RJINT GObjectWidget::getY()
			{
				UIView* widget = getNativeWidget();
				return widget.frame.origin.y;
			}
			
			void GObjectWidget::setSize(RJINT width, RJINT height)
			{
				UIView* widget = getNativeWidget();
				widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
			}
			
			void GObjectWidget::setSize(CPP::Vector2 size)
			{
				setSize(size.x, size.y);
			}
			
			CPP::Vector2 GObjectWidget::getSize()
			{
				return Vector2(getWidth(), getHeight());
			}
			
			RJINT GObjectWidget::getWidth()
			{
				UIView* widget = getNativeWidget();
				return widget.frame.size.width;
			}
			
			RJINT GObjectWidget::getHeight()
			{
				UIView* widget = getNativeWidget();
				return widget.frame.size.height;
			}
			
			void GObjectWidget::setText(String text)
			{
				//Do nothing here
			}
			
			String GObjectWidget::getText()
			{
				//Do nothing here
				return String();
			}
			
			GObject* GObjectWidget::getParent()
			{
                return (_parent);
			}
			
			void GObjectWidget::setVisibility(RJBOOL visible)
			{
				UIView* widget = getNativeWidget();
				[widget setHidden:!visible];
			}
			
			RJBOOL GObjectWidget::getVisibility()
			{
				UIView* widget = getNativeWidget();
				return !widget.isHidden;
			}
			
			void GObjectWidget::setEnabled(RJBOOL enabled)
			{
                UIView* widget = getNativeWidget();
                widget.userInteractionEnabled = enabled;
			}
			
			RJBOOL GObjectWidget::getEnabled()
			{
                UIView* widget = getNativeWidget();
                return (widget.userInteractionEnabled);
			}
		}
	}
}
