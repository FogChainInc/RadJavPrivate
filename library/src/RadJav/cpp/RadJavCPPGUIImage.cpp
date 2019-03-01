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
#include "cpp/RadJavCPPGUIImage.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef GUI_USE_WXWIDGETS
	#include "cpp/desktop/RadJavCPPGUIImageFrame.h"
#elif defined USE_ANDROID
	#include "cpp/android/RadJavCPPGUIImageFrame.h"
#elif defined USE_IOS
	#include "cpp/ios/RadJavCPPGUIImageFrame.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef USE_V8
				Image::Image(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
					_image = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "_image");
				}
			#elif defined USE_JAVASCRIPTCORE
				Image::Image(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
					: GObject (jsEngine, thisObject, argumentCount, arguments)
				{
					_image = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "_image");
				}
			#endif

			Image::Image(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
				_image = "";
			}

			void Image::create()
			{
				GObjectWidget* parentWin = nullptr;
				
				if (_parent != nullptr)
					parentWin = _parent->_appObj;
				
				ImageFrame* object = RJNEW ImageFrame(parentWin, _image,
													  Vector2(_transform->x, _transform->y),
													  Vector2(_transform->width, _transform->height));
				
				object->setVisibility(_visible);
				_appObj = object;
				linkWith(object);
				setup();

				if (_parent != NULL)
					_parent->_callChildCreated(this);
			}

			RJBOOL Image::setImage(const String& imageFile)
			{
				if (imageFile == "")
					return false;

				if (_appObj)
				{
					_image = imageFile;

					ImageFrame* image = static_cast<ImageFrame*>(_appObj);
					return image->loadImage(imageFile);
				}
				
				return false;
			}
			
			void Image::setScaleMode(Image::ScaleMode mode)
			{
				if (_appObj)
				{
					ImageFrame* image = static_cast<ImageFrame*>(_appObj);
					return image->setScaleMode(mode);
				}
			}

			Image::ScaleMode Image::getScaleMode() const
			{
				if (_appObj)
				{
					ImageFrame* image = static_cast<ImageFrame*>(_appObj);
					return image->getScaleMode();
				}
				
				return ScaleMode::AspectFit;
			}
			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void Image::on(String event, RJ_FUNC_TYPE func)
				{
					if (_appObj)
					{
						#ifdef GUI_USE_WXWIDGETS 
							
							CPP::GUI::ImageFrame *object = (CPP::GUI::ImageFrame *)_appObj;
							object->addNewEvent(event, object, func);
							
						#else
							_appObj->addNewEvent(event, func);
						#endif
					}
				}
			#endif
		}
	}
}

