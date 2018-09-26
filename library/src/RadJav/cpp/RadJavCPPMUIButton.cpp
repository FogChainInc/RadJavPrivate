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
#include "cpp/RadJavCPPMUIButton.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			#ifdef USE_V8
			Button::Button(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args)
			{
			}
			#endif
            #ifdef USE_JAVASCRIPTCORE
                Button::Button(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
                : GObject (jsEngine, thisObj, numArgs, args)
                {
                }
            #endif

			Button::Button(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void Button::create()
			{
//				SomeBaseContainerType* parentWin = nullptr;
//
//				if (_parent != nullptr)
//					parentWin = (SomeBaseContainerType*)_parent->_appObj;

				
				//TODO: add correct parent object
				ButtonFrame* object = RJNEW ButtonFrame(/*parentWin*/ nullptr, _text,
														Vector2(_transform->x, _transform->y),
														Vector2(_transform->width, _transform->height));
				
//				_appObj = object;
//
//				linkWith(object);
				
				setup();

//				object->Show(_visible);
			}

			void Button::setPosition(RJINT x, RJINT y)
			{
                /// Do stuff here.
			}

			CPP::Vector2 Button::getPosition()
			{
				CPP::Vector2 newpos;

                /// Do stuff here.

				return (newpos);
			}

			void Button::setSize(RJINT width, RJINT height)
			{
				_transform->setSize(width, height);

                /// Do stuff here.
			}

			CPP::Vector2 Button::getSize()
			{
				CPP::Vector2 size = _transform->getSize();

                /// Do stuff here.

				return (size);
			}

			void Button::setText(String text)
			{
				_text = text;

                /// Do stuff here.
			}

			String Button::getText()
			{
				String text = _text;

                /// Do stuff here.

				return (text);
			}

			void Button::setVisibility(RJBOOL visible)
			{
				_visible = visible;

                /// Do stuff here.
			}

			RJBOOL Button::getVisibility()
			{
				RJBOOL visible = _visible;

                /// Do stuff here.

				return (visible);
			}

			void Button::setEnabled(RJBOOL enabled)
			{
                /// Do stuff here.
			}

			RJBOOL Button::getEnabled()
			{
				RJBOOL enabled = false;

                /// Do stuff here.

				return (enabled);
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
            	void Button::on(String event, RJ_FUNC_TYPE func)
				{
                    /// Do stuff here.
				}
			#endif
		}
	}
}

