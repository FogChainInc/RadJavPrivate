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
#include "cpp/RadJavCPPMUITextarea.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			TextareaFrame::TextareaFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
				//TODO: Add implementation

				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			TextareaFrame::~TextareaFrame()
			{
				//TODO: Add implementation
			}
			
			void TextareaFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String TextareaFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
			void TextareaFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* TextareaFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void TextareaFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation
			}
			
			RJBOOL TextareaFrame::getEnabled()
			{
				//TODO: Add implementation
				return true;
			}
			
			bool TextareaFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				return false;
			}

			jobject TextareaFrame::getNativeWidget()
			{
				return widget;
			}
		}
	}
}

