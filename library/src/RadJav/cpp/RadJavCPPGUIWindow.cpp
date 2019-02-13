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
#include "cpp/RadJavCPPGUIWindow.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef GUI_USE_WXWIDGETS
	#include "cpp/desktop/RadJavCPPGUIWindowFrame.h"
#elif defined USE_ANDROID
	#include "cpp/android/RadJavCPPGUIWindowFrame.h"
#elif defined USE_IOS
	#include "cpp/ios/RadJavCPPGUIWindowFrame.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef USE_V8
			Window::Window(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: GObject (jsEngine, args)
			{
				icon = jsEngine->v8GetString(args.This(), "_icon");
			}
			#endif
            #ifdef USE_JAVASCRIPTCORE
                Window::Window(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
                : GObject (jsEngine, thisObj, numArgs, args)
                {
                    icon = jsEngine->jscGetString(thisObj, "_icon");
                }
            #endif

			Window::Window(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void Window::create()
			{
				#ifdef GUI_USE_WXWIDGETS
                    #ifdef __WXOSX__
                        if (_transform->y == 0)
                        {
                            /// @todo Fix this later on OSX.
                            /*NSScreen *screen = [NSScreen mainScreen];
                            RJINT appleMenuBarHeight = screen.frame.size.height;
                            _transform->y = appleMenuBarHeight;*/
                            _transform->y = 22;
                        }
                    #endif

					WindowFrame *object = RJNEW WindowFrame(_text, 
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
				
					linkWith(object);
				
					RadJav::app->SetTopWindow(object);
					object->setVisibility(_visible);
					RadJav::app->SetActive(_visible, object);

					_appObj = object;

					if (icon != "")
						setIcon(icon);

					setup();
				#endif
			}

			void Window::setIcon(String newIcon)
			{
				icon = newIcon;

				#ifdef GUI_USE_WXWIDGETS
                    #ifdef __WINDOWS__
                        wxIcon wxicon = wxICON(newIcon.c_str ());
                
                        if (wxicon.IsOk() == false)
                            throw RadJAV::Exception("Invalid icon");
                
                        ((WindowFrame *)((WindowFrame *)_appObj)->GetParent())->SetIcon(wxicon);
                    #else
                        /// @todo Add Window Icon support for non Windows platforms.
                        #warning "TODO: Add Window Icon support for non Windows platforms"
                    #endif
				#endif
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
            	void Window::on(String event, RJ_FUNC_TYPE func)
				{
					#ifdef GUI_USE_WXWIDGETS
						CPP::GUI::WindowFrame *object = (CPP::GUI::WindowFrame *)_appObj;
						CPP::GUI::WindowFrame *obj = (CPP::GUI::WindowFrame *)object->GetParent();

						obj->addNewEvent(event, obj, func);

						if (event == "close")
						{
							obj->Connect(wxEVT_CLOSE_WINDOW, wxCloseEventHandler(WindowFrame::onJSClose), obj->createEvent(event, func));
						}

						if (event == "minimize")
						{
							obj->Connect(wxEVT_ICONIZE, wxIconizeEventHandler(WindowFrame::onJSMinimized), obj->createEvent(event, func));
						}
						if (event == "maximize")
						{
							obj->Connect(wxEVT_MAXIMIZE, wxMaximizeEventHandler(WindowFrame::onJSMaximized), obj->createEvent(event, func));
						}
				
						if (event == "menuselected")
						{
							obj->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(WindowFrame::onMenuSelected), obj->createEvent(event, func));
						}
					#endif
				}
			#endif
		}
	}
}
