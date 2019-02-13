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
#ifndef _RADJAV_GUI_CPP_LABEL_FRAME_H_
#define _RADJAV_GUI_CPP_LABEL_FRAME_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			class RADJAV_EXPORT LabelFrame : public GObjectWidget
											,public ChainedPtr
			{
			public:
				/**
				 * Constructor.
				 * @param GObjectWidget parent. Constructed object will be added to view hierarchy of parent
				 * @param String text. Display text, has default value of empty string.
				 * @param Vector2 pos Initial position.
				 * @param Vector2 size Initial size.
				 */
				LabelFrame(GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size);
				~LabelFrame();

				/** @method setText
				 * Setter for button text.
				 * @param String text
				 */
				void setText(String text);
				
				/** @method getText
				 * Getter for buttonText.
				 * @return String text
				 */
				String getText();
				
				/** @method setFont
				 * Setter for button font.
				 * @param Font font
				 */
				void setFont(CPP::Font *font);
				/** @method getFont
				 * Getter for button font.
				 * @return Font
				 */
				CPP::Font *getFont();

				bool bindEvent(const String& eventName, const GUI::Event* event);

			private:
				static jclass nativeTextViewClass;

				static jmethodID nativeConstructor;
				static jmethodID nativeSetText;
				static jmethodID nativeGetText;
			};
		}
	}
}
#endif
