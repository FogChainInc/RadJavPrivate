/*
	MIT-LICENSE
	Copyright (c) 2017 Higher Edge Software, LLC

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
#include "v8/RadJavV8GUIGObject.h"

#include "RadJav.h"

#ifdef USE_V8
#include "v8/RadJavV8JavascriptEngine.h"

#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace V8B
	{
		namespace GUI
		{
			void GObject::createV8Callbacks(v8::Isolate *isolate, v8::Local<v8::Object> object)
			{
				V8_CALLBACK(object, "_createAppObj", GObject::_createAppObj);
			}

			void GObject::_createAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::create(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::setFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getFont(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::setPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getPosition(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getX(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getY(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::setSize(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getSize(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getWidth(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getHeight(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::setText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getText(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getParent(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getAppObj(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::setVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getVisibility(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::setEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::getEnabled(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}

			void GObject::on(const v8::FunctionCallbackInfo<v8::Value> &args)
			{
			}
		}
	}
}
#endif
