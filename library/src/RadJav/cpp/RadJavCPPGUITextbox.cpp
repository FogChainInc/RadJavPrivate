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
#include "cpp/RadJavCPPGUITextbox.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef GUI_USE_WXWIDGETS
	#include "cpp/desktop/RadJavCPPGUITextboxFrame.h"
#elif defined USE_ANDROID
	#include "cpp/android/RadJavCPPGUITextboxFrame.h"
#elif defined USE_IOS
	#include "cpp/ios/RadJavCPPGUITextboxFrame.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef USE_V8
				Textbox::Textbox(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
				}
			#elif defined USE_JAVASCRIPTCORE
				Textbox::Textbox(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
					: GObject (jsEngine, thisObject, argumentCount, arguments)
				{
				}
			#endif

			Textbox::Textbox(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void Textbox::create()
			{
				GObjectWidget* parentWin = nullptr;
				
				if (_parent != nullptr)
					parentWin = _parent->_appObj;
				
				TextboxFrame* object = RJNEW TextboxFrame(parentWin, _text,
													  Vector2(_transform->x, _transform->y),
													  Vector2(_transform->width, _transform->height));
				
				object->setVisibility(_visible);
				_appObj = object;
				linkWith(object);
				setup();
			}

			void Textbox::setInputMode(Textbox::InputMode mode)
			{
				if (_appObj)
				{
					TextboxFrame* textbox = static_cast<TextboxFrame*>(_appObj);
					return textbox->setInputMode(mode);
				}
			}

			Textbox::InputMode Textbox::getInputMode() const
			{
				if (_appObj)
				{
					TextboxFrame* textbox = static_cast<TextboxFrame*>(_appObj);
					return textbox->getInputMode();
				}
				
				return InputMode::Text;
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void Textbox::on(String event, RJ_FUNC_TYPE func)
				{
					if (_appObj)
					{
						#ifdef GUI_USE_WXWIDGETS
							CPP::GUI::TextboxFrame *obj = (CPP::GUI::TextboxFrame *)_appObj;

							obj->addNewEvent(event, obj, func);
							
							if (event == "onText")
							{
								obj->Connect(wxEVT_TEXT, wxCommandEventHandler(TextboxFrame::onText), obj->createEvent(event, func));
							}

							if (event == "onTextEnter")
							{
								obj->Connect(wxEVT_TEXT_ENTER, wxCommandEventHandler(TextboxFrame::onTextEnter), obj->createEvent(event, func));
							}
						#else
							_appObj->addNewEvent(event, func);
						#endif
					}
				}
			#endif
		}
	}
}
