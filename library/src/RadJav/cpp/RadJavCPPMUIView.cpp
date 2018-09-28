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
#include "cpp/RadJavCPPMUIView.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			#ifdef USE_V8
			View::View(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args)
			{
			}
			#endif
            #ifdef USE_JAVASCRIPTCORE
                View::View(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
                    : GObject (jsEngine, thisObj, numArgs, args)
                {
                }
            #endif

			View::View(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

            void View::addChild(GUI::GObject *child){
                _appObject->addChild(child);
            }
            
			void View::create()
			{

                //TODO: add correct parent object
                ViewFrame* object = RJNEW ViewFrame(/*parentWin*/ nullptr, _text,
                                                        Vector2(_transform->x, _transform->y),
                                                        Vector2(_transform->width, _transform->height));
                
                _appObject = object;
                linkWith(object);
                setup();
			}

			void View::setPosition(RJINT x, RJINT y)
			{
                _appObject->setPosition(x, y);
			}

			CPP::Vector2 View::getPosition()
			{
				CPP::Vector2 newpos;

                /// Do stuff here.

				return (newpos);
			}

			void View::setSize(RJINT width, RJINT height)
			{
				_transform->setSize(width, height);

                _appObject->setSize(width, height);
			}

			CPP::Vector2 View::getSize()
			{
				CPP::Vector2 size = _transform->getSize();

                /// Do stuff here.

				return (size);
			}

			void View::setText(String text)
			{
				_text = text;

                /// Do stuff here.
			}

			String View::getText()
			{
				String text = _text;

                /// Do stuff here.

				return (text);
			}

			void View::setVisibility(RJBOOL visible)
			{
				_visible = visible;

                /// Do stuff here.
			}

			RJBOOL View::getVisibility()
			{
				RJBOOL visible = _visible;

                /// Do stuff here.

				return (visible);
			}

			void View::setEnabled(RJBOOL enabled)
			{
                /// Do stuff here.
			}

			RJBOOL View::getEnabled()
			{
				RJBOOL enabled = false;

				/// Do stuff here.

				return (enabled);
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
            	void View::on(String event, RJ_FUNC_TYPE func)
				{
                    /// Do stuff here.
				}
			#endif
		}
	}
}

