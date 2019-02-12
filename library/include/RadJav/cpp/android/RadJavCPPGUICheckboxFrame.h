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
#ifndef _RADJAV_GUI_CPP_CHECKBOX_FRAME_H_
#define _RADJAV_GUI_CPP_CHECKBOX_FRAME_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			class RADJAV_EXPORT CheckboxFrame : public GObjectWidget
												,public ChainedPtr
			{
			public:
				/**
				 * Constructor. Frame will be added as child to provided parent, state and size are preset from parameters.
				 * @param GObjectWidget parent. Constructed object will be added to view hierarchy of parent.
				 * @param BOOL checked. Initial state
				 * @param String text. For compatibility with desktop Checkbox
				 * @param Vector2 pos Initial position.
				 * @param Vector2 size Initial size.
				 */
				CheckboxFrame(GObjectWidget *parent, RJBOOL checked, String text, const Vector2 &pos, const Vector2 &size);
				~CheckboxFrame();
				
				/** @method setChecked
				 * Setter for control state.
				 * @param BOOL checked
				 */
				void setChecked(RJBOOL checked);
				
				/** @method isChecked
				 * Getter for control state.
				 * @return BOOL checked
				 */
				RJBOOL isChecked() const;

				bool bindEvent(const String& eventName, const GUI::Event* event);

			private:
				static jclass nativeSwitchClass;

				static jmethodID nativeConstructor;
				static jmethodID nativeSetChecked;
				static jmethodID nativeIsChecked;
			};
		}
	}
}
#endif
