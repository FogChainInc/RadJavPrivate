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
#include "cpp/RadJavCPPGUIButton.h"

#include "RadJav.h"
#include "RadJavString.h"

#include <iostream>
namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				ButtonFrame::ButtonFrame(wxWindow *parent, const wxString &text, const wxPoint &pos, const wxSize &size)
					: wxButton(parent, wxID_ANY, text, pos, size), GObjectBase()
				{
				}
		                ButtonFrame::~ButtonFrame()
				{
				  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;

				}
			#endif

			#ifdef USE_V8
				Button::Button(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				        : GObject (jsEngine, args), ObjWrap(jsEngine, args)
				{
				}
			#endif

			Button::Button(String name, String text, CPP::GUI::GObject *parent)
   			        : GObject(name, text, parent)
			{
			}

		        Button::~Button()
			{

			  std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl << std::flush;
			  std::cout << "Button text: " << std::flush;
			  std::cout << getText() << std::endl << std::flush;

			  if (_appObj)
			    {
			      std::cout << "_appObj: " << std::hex << (void*) _appObj << std::dec << std::endl << std::flush;
			      CPP::GUI::ButtonFrame *object = dynamic_cast<CPP::GUI::ButtonFrame *>(_appObj);
			      std::cout << "object: " << std::hex << (void*)object << std::dec << std::endl << std::flush;
			      //DELETEOBJ(_appObj);
			    }
			  //if (_appObj)
			  //DELETEOBJ(_appObj);
			  std::cout << "Frame deleted" << std::endl << std::flush;
								
			    
	    if (myPersistent.IsEmpty())
	      {
		std::cout << "Persistent isEmpty" << std::endl << std::flush;
		return;
	      }
	    if (myPersistent.IsNearDeath())
	      std::cout << "Persistent isNearDeath" << std::endl << std::flush;
			  

			}

			void Button::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					ButtonFrame *object = RJNEW ButtonFrame(parentWin, _text.towxString(), 
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);

					_appObj = object;

					setup();
				#endif
			}

			void Button::setText(String text)
			{
				_text = text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					       dynamic_cast<ButtonFrame *>(_appObj)->SetLabelText(_text.towxString());
				#endif
			}

			String Button::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
					        wxString wxtext = (dynamic_cast<ButtonFrame *>(_appObj))->GetLabelText();
						text = parsewxString(wxtext);
					}
				#endif

				return (text);
			}

			#ifdef USE_V8
				void Button::on(String event, v8::Local<v8::Function> func)
				{
				        CPP::GUI::ButtonFrame *object = dynamic_cast<CPP::GUI::ButtonFrame *>(_appObj);

					object->addNewEvent(event, object, func);
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::ButtonFrame, wxButton)
	wxEND_EVENT_TABLE()
#endif

