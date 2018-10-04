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
#include "cpp/RadJavCPPMUICheckBox.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			#ifdef USE_V8
				CheckBox::CheckBox(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args)
				{
				}
			#elif USE_JAVASCRIPTCORE
				CheckBox::CheckBox(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
				: GObject (jsEngine, thisObj, numArgs, args)
				{
				}
			#endif
			
			CheckBox::CheckBox(String name, String text, CPP::GUI::GObject *parent)
			: GObject(name, text, parent)
			{
			}
			
			void CheckBox::create()
			{
				GUI::GObjectWidget* parentWin = nullptr;
				
				if (_parent != nullptr)
					parentWin = _parent->_appObj;
				
				CheckBoxFrame* object = RJNEW CheckBoxFrame(_parent, _text,
													  Vector2(_transform->x, _transform->y),
													  Vector2(_transform->width, _transform->height));
				
				object->setVisibility(_visible);
				_appObj = object;
				linkWith(object);
				setup();
			}
			
			void CheckBox::setChecked(RJBOOL checked)
			{
				if (_appObj)
				{
					CheckBoxFrame* box = static_cast<CheckBoxFrame*>(_appObj);
					box->setChecked(checked);
				}
			}
			
			RJBOOL CheckBox::getChecked() const
			{
				if (_appObj)
				{
					CheckBoxFrame* box = static_cast<CheckBoxFrame*>(_appObj);
					return box->getChecked();
				}
				
				return false;
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void CheckBox::on(String event, RJ_FUNC_TYPE func)
				{
					if (_appObj)
					{
						_appObj->addNewEvent(event, func);
					}
				}
			#endif
		}
	}
}

