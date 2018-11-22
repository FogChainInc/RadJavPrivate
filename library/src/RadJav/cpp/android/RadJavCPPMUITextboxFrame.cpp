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
#include "cpp/RadJavCPPMUITextbox.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			TextboxFrame::TextboxFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
				//TODO: Add implementation

				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			TextboxFrame::~TextboxFrame()
			{
				//TODO: Add implementation
			}
			
			void TextboxFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String TextboxFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
			void TextboxFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* TextboxFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void TextboxFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation
			}
			
			RJBOOL TextboxFrame::getEnabled()
			{
				//TODO: Add implementation
				return true;
			}
			
			void TextboxFrame::setInputMode(Textbox::InputMode mode)
			{
				//TODO: Add implementation
				/*
				UIKeyboardType keyboardType = UIKeyboardTypeDefault;
				
				switch (mode)
				{
					case Textbox::InputMode::Text:
						keyboardType = UIKeyboardTypeDefault;
						break;
					case Textbox::InputMode::Number:
						keyboardType = UIKeyboardTypeNumberPad;
						break;
					case Textbox::InputMode::Decimal:
						keyboardType = UIKeyboardTypeDecimalPad;
						break;
					case Textbox::InputMode::Phone:
						keyboardType = UIKeyboardTypePhonePad;
						break;
					case Textbox::InputMode::Email:
						keyboardType = UIKeyboardTypeEmailAddress;
						break;
					case Textbox::InputMode::Password:
						[widget setSecureTextEntry:true];
						return;
					default:;
				}

				if (widget.keyboardType != keyboardType)
					widget.keyboardType = keyboardType;
				 */
			}
			
			Textbox::InputMode TextboxFrame::getInputMode() const
			{
				//TODO: Add implementation
				/*
				if ([widget isSecureTextEntry])
					return Textbox::InputMode::Password;
				
				switch (widget.keyboardType)
				{
					case UIKeyboardTypeNumberPad:
					case UIKeyboardTypeNumbersAndPunctuation:
						return Textbox::InputMode::Number;
					case UIKeyboardTypeDecimalPad:
						return Textbox::InputMode::Decimal;
					case UIKeyboardTypePhonePad:
						return Textbox::InputMode::Phone;
					case UIKeyboardTypeEmailAddress:
						return Textbox::InputMode::Email;
					case UIKeyboardTypeDefault:
					case UIKeyboardTypeASCIICapable:
					case UIKeyboardTypeURL:
					case UIKeyboardTypeNamePhonePad:
					case UIKeyboardTypeASCIICapableNumberPad:
					case UIKeyboardTypeWebSearch:
					case UIKeyboardTypeTwitter:
					default:
						return Textbox::InputMode::Text;
				}
				 */

				return Textbox::InputMode::Text;
			}

			bool TextboxFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				//TODO: Add implementation
			}

			jobject TextboxFrame::getNativeWidget()
			{
				return widget;
			}
		}
	}
}

