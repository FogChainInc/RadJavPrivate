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
#include "cpp/RadJavCPPMUICheckbox.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			CheckboxFrame::CheckboxFrame(GUI::GObjectWidget *parent, RJBOOL checked, const Vector2 &pos, const Vector2 &size)
			{
				//TODO: Add implementation

				setChecked(checked);
				setSize(size);
				setPosition(pos);
			}
			
			CheckboxFrame::~CheckboxFrame()
			{
				//TODO: Add implementation
			}
			
			void CheckboxFrame::setChecked(RJBOOL checked)
			{
				//TODO: Add implementation

			}
			
			RJBOOL CheckboxFrame::getChecked() const
			{
				//TODO: Add implementation

			}

			void CheckboxFrame::setEnabled(RJBOOL enabled)
			{
				//TODO: Add implementation

			}
			
			RJBOOL CheckboxFrame::getEnabled()
			{
				//TODO: Add implementation

			}
			
			bool CheckboxFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				//TODO: Add implementation

			}

			jobject CheckboxFrame::getNativeWidget()
			{
				return widget;
			}
		}
	}
}

