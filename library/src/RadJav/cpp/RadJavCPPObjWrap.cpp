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

#include "cpp/RadJavCPPObjWrap.h"

#include "v8/RadJavV8GUIButton.h"

#include "RadJavString.h"
#include "v8/RadJavV8JavascriptEngine.h"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>

#include <iostream>
#include <sstream>
#include <iomanip>

namespace RadJAV
{
	namespace CPP
	{

	  ObjWrap::ObjWrap() {}
	  ObjWrap::ObjWrap(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
	  {
	    //std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
	    std::cout << "Args len: " << args.Length() << std::endl;

	    this -> jsEngine = jsEngine;
	    v8::Isolate *isolate = args.GetIsolate();
			  
	  }

	  ObjWrap::~ObjWrap()
	  {
	    std::cout << __PRETTY_FUNCTION__ << ": begin " << std::hex << (void*)this << std::dec << std::endl << std::flush;
	    if (myPersistent.IsEmpty())
	      {
		std::cout << "Persistent isEmpty" << std::endl << std::flush;
		return;
	      }
	    if (myPersistent.IsNearDeath())
	      std::cout << "Persistent isNearDeath" << std::endl << std::flush;
	    myPersistent.ClearWeak();
	    myPersistent.Reset();
	    std::cout << __PRETTY_FUNCTION__ << ": end " << std::hex << (void*)this << std::dec << std::endl;
	  }

	  void ObjWrap::wrap(v8::Local<v8::Object> handle)
	  {
	    std::cout << __PRETTY_FUNCTION__ << ": begin " << std::hex << (void*)this << std::dec << std::endl;

	    myPersistent.Reset(handle -> GetIsolate(), handle);
	    myPersistent.SetWeak(this, weakCallback, v8::WeakCallbackType::kParameter);
			  
	  }
		  
	  void ObjWrap::weakCallback(const v8::WeakCallbackInfo<ObjWrap>& data)
	  {
	    std::cout << __PRETTY_FUNCTION__ << ": begin" << std::endl;
	    ObjWrap *wrap = data.GetParameter();
	    std::cout << "AA" << std::endl << std::flush;

	    RadJAV::V8B::GUI::Button *button = dynamic_cast<RadJAV::V8B::GUI::Button*>(button);
	    
	    if (button)
	      std::cout << "This is a button" << std::endl << std::flush;
	    else
	      std::cout << "Dynamic casts came empty" << std::endl << std::flush;
	      


	    std::cout << "BB" << std::endl << std::flush;
	    DELETEOBJ(wrap);
	    std::cout << "CC" << std::endl << std::flush;
	  }

	}
}

