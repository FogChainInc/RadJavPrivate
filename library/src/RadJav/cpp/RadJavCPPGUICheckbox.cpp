/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#include "cpp/RadJavCPPGUICheckbox.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef GUI_USE_WXWIDGETS
	#include "cpp/desktop/RadJavCPPGUICheckboxFrame.h"
#elif defined USE_ANDROID
	#include "cpp/android/RadJavCPPGUICheckboxFrame.h"
#elif defined USE_IOS
	#include "cpp/ios/RadJavCPPGUICheckboxFrame.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef USE_V8
				Checkbox::Checkbox(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
					_checked = V8_JAVASCRIPT_ENGINE->v8GetBool(args.This(), "_checked");
				}
			#elif defined USE_JAVASCRIPTCORE
				Checkbox::Checkbox(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
					: GObject (jsEngine, thisObject, argumentCount, arguments)
				{
					_checked = JSC_JAVASCRIPT_ENGINE->jscGetBool(thisObject, "_checked");
				}
			#endif

			Checkbox::Checkbox(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
				_checked = false;
			}

			void Checkbox::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					CheckboxFrame *object = RJNEW CheckboxFrame(parentWin, _text.towxString(), 
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);
					object->SetValue(_checked);

					_appObj = object;
				
					linkWith(object);

					setup();
				#endif
			}

			void Checkbox::setText(String text)
			{
				_text = text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						((CheckboxFrame *)_appObj)->SetLabelText(_text.towxString());
				#endif
			}

			String Checkbox::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxString wxtext = ((CheckboxFrame *)_appObj)->GetLabelText();
						text = parsewxString(wxtext);
					}
				#endif

				return (text);
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void Checkbox::on(String event, RJ_FUNC_TYPE func)
				{
					#ifdef GUI_USE_WXWIDGETS
						CPP::GUI::CheckboxFrame *object = (CPP::GUI::CheckboxFrame *)_appObj;

						object->addNewEvent(event, object, func);

						if (event == "change")
						{
							object->Connect(wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler(CheckboxFrame::onChanged), object->createEvent(event, func));
						}
					#endif
				}
			#endif

			void Checkbox::setChecked(RJBOOL checked)
			{
				_checked = checked;

				#ifdef GUI_USE_WXWIDGETS
					CheckboxFrame *object = (CheckboxFrame *)_appObj;

					object->SetValue(_checked);
				#endif
			}

			RJBOOL Checkbox::isChecked()
			{
				RJBOOL value = _checked;

				#ifdef GUI_USE_WXWIDGETS
					CheckboxFrame *object = (CheckboxFrame *)_appObj;
					value = object->IsChecked();
				#endif

				return (value);
			}
		}
	}
}
