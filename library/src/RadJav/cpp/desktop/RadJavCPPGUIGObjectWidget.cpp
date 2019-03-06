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

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			GObjectWidget::GObjectWidget()
			{
				parent = nullptr;
			}

			void GObjectWidget::addChild(GObjectWidget *child)
			{
				if (child)
				{
					getNativeWidget()->AddChild(child->getNativeWidget());
					child->parent = this;
				}
			}
			
			void GObjectWidget::setFont(CPP::Font *font)
			{
				wxFont wxfont;
				
				if (font->fontFamily != "")
				{
					if (!wxfont.SetFaceName(font->fontFamily.towxString()))
						return;
				}
				else
				{
					wxfont = *wxSMALL_FONT;
				}
				
				wxfont.SetPixelSize(wxSize(0, font->size));
				
				if (font->underline == true)
					wxfont.MakeUnderlined();
				
				if (font->bold == true)
					wxfont.MakeBold();
				
				if (font->italic == true)
					wxfont.MakeItalic();
				
				wxWindow* widget = getNativeWidget();
				if (widget)
				{
					widget->SetFont(wxfont);
					widget->SetForegroundColour(wxColor(font->color.r * 255,
														font->color.g * 255, font->color.b * 255, font->color.a * 255));
				}
			}
			
			CPP::Font* GObjectWidget::getFont()
			{
				//TODO: convert native Font to CPP::Font here
				return nullptr;
			}
			
			void GObjectWidget::setPosition(RJINT x, RJINT y)
			{
				getNativeWidget()->SetPosition(wxPoint(x, y));
			}
			
			void GObjectWidget::setPosition(CPP::Vector2 pos)
			{
				setPosition(pos.x, pos.y);
			}
			
			CPP::Vector2 GObjectWidget::getPosition()
			{
				wxPoint pos = getNativeWidget()->GetPosition();
				return Vector2(pos.x, pos.y);
			}
			
			RJINT GObjectWidget::getX()
			{
				return getNativeWidget()->GetPosition().x;
			}
			
			RJINT GObjectWidget::getY()
			{
				return getNativeWidget()->GetPosition().y;
			}
			
			void GObjectWidget::setSize(RJINT width, RJINT height)
			{
				getNativeWidget()->SetSize(width, height);
			}
			
			void GObjectWidget::setSize(CPP::Vector2 size)
			{
				setSize(size.x, size.y);
			}
			
			CPP::Vector2 GObjectWidget::getSize()
			{
				wxSize size = getNativeWidget()->GetSize();
				return Vector2(size.GetWidth(), size.GetHeight());
			}
			
			RJINT GObjectWidget::getWidth()
			{
				return getNativeWidget()->GetSize().GetWidth();
			}
			
			RJINT GObjectWidget::getHeight()
			{
				return getNativeWidget()->GetSize().GetHeight();
			}
			
			void GObjectWidget::setText(String text)
			{
				getNativeWidget()->SetLabel(text.towxString());
			}
			
			String GObjectWidget::getText()
			{
				return parsewxString(getNativeWidget()->GetLabel());
			}
			
			GObjectWidget* GObjectWidget::getParent()
			{
				return parent;
			}
			
			void GObjectWidget::setVisibility(RJBOOL visible)
			{
				if (visible)
					getNativeWidget()->Show();
				else
					getNativeWidget()->Hide();
			}
			
			RJBOOL GObjectWidget::getVisibility()
			{
				return getNativeWidget()->IsShown();
			}
			
			void GObjectWidget::setEnabled(RJBOOL enabled)
			{
				if (enabled)
					getNativeWidget()->Enable();
				else
					getNativeWidget()->Disable();
			}
			
			RJBOOL GObjectWidget::getEnabled()
			{
				return getNativeWidget()->IsEnabled();
			}
		}
	}
}
