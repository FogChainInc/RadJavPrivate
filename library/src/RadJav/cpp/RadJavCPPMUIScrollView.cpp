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
#include "cpp/RadJavCPPMUIScrollView.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef GUI_USE_WXWIDGETS
	#include "cpp/desktop/RadJavCPPMUIScrollViewFrame.h"
#elif defined USE_ANDROID
	#include "cpp/android/RadJavCPPMUIScrollViewFrame.h"
#elif defined USE_IOS
	#include "cpp/ios/RadJavCPPMUIScrollViewFrame.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			#ifdef USE_V8
				ScrollView::ScrollView(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args)
				{
				}
			#elif USE_JAVASCRIPTCORE
				ScrollView::ScrollView(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
				: GObject (jsEngine, thisObj, numArgs, args)
				{
				}
			#endif
			
			ScrollView::ScrollView(String name, String text, CPP::GUI::GObject *parent)
			: GObject(name, text, parent)
			{
			}
			
			void ScrollView::create()
			{
				#ifdef RADJAV_MOBILE
					GUI::GObjectWidget* parentWin = nullptr;
				
					if (_parent != nullptr)
						parentWin = _parent->_appObj;
				
					ScrollViewFrame* object = RJNEW ScrollViewFrame(
															parentWin,
															Vector2(_transform->x, _transform->y),
															Vector2(_transform->width, _transform->height));
				#endif

				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					ScrollViewFrame *object = RJNEW ScrollViewFrame(parentWin,
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->AlwaysShowScrollbars(false, false);
					//object->SetScrollRate(10, 10);

					onChildCreated = [object](GUI::GObject *obj)
						{
							wxPoint pos = obj->_appObj->GetPosition();
							wxSize size = obj->_appObj->GetSize();
							RJINT xsize = object->GetScrollPageSize(wxHORIZONTAL) + pos.x + size.GetWidth ();
							RJINT ysize = object->GetScrollPageSize (wxVERTICAL) + pos.y + size.GetHeight ();

							object->SetScrollbars (1, 1, xsize, ysize);
						};
				#endif
				
				setVisibility(_visible);
				_appObj = object;
				linkWith(object);
				setup();

				if (_parent != NULL)
					_parent->_callChildCreated(this);
			}
			
			void ScrollView::setContentSize(const CPP::Vector2& size)
			{
//				if (_appObj)
//				{
//					ScrollViewFrame* scrollView = static_cast<ScrollViewFrame*>(_appObj);
//					scrollView->setContentSize(size);
//				}
			}

//			void ScrollView::addChild(RadJAV::CPP::GUI::GObject *child)
//			{
//				if (_appObj)
//				{
//					ScrollViewFrame* scrollView = static_cast<ScrollViewFrame*>(_appObj);
//					scrollView->addChild(child);
//				}
//			}

			CPP::Vector2 ScrollView::getContentSize() const
			{
//				if (_appObj)
//				{
//					ScrollViewFrame* scrollView = static_cast<ScrollViewFrame*>(_appObj);
//					return scrollView->getContentSize();
//				}
//
				return Vector2();
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void ScrollView::on(String event, RJ_FUNC_TYPE func)
				{
					#ifdef RADJAV_MOBILE
						if (_appObj)
						{
							_appObj->addNewEvent(event, func);
						}
					#endif

					#ifdef GUI_USE_WXWIDGETS
						ScrollViewFrame *object = (ScrollViewFrame *)_appObj;

						object->addNewEvent(event, object, func);

						if (event == "click")
						{
							object->Connect(wxEVT_LEFT_UP, wxMouseEventHandler(ScrollViewFrame::onClick), object->createEvent(event, func));
						}
					#endif
				}
			#endif
		}
	}
}
