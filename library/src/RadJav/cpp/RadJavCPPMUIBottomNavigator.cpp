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
#include "cpp/RadJavCPPMUIBottomNavigator.h"
#include "cpp/RadJavCPPMUIView.h"

#ifdef GUI_USE_WXWIDGETS
	#include "cpp/desktop/RadJavCPPMUIBottomNavigatorFrame.h"
	#include "cpp/desktop/RadJavCPPMUIViewFrame.h"
#elif defined USE_ANDROID
	#include "cpp/android/RadJavCPPMUIBottomNavigatorFrame.h"
	#include "cpp/android/RadJavCPPMUIViewFrame.h"
#elif defined USE_IOS
	#include "cpp/ios/RadJavCPPMUIBottomNavigatorFrame.h"
	#include "cpp/ios/RadJavCPPMUIViewFrame.h"
#endif

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			#ifdef USE_V8
				BottomNavigator::BottomNavigator(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
				: impl(nullptr),
				  rootView(nullptr)
				{
					v8::Local<v8::Object> rootWinJs = jsEngine->v8GetObject(args.This(), "rootWin");
					if(!jsEngine->v8IsNull(rootWinJs))
					{
						rootView = (View*)jsEngine->v8GetExternal(rootWinJs, "_appObj");
					}
				}
			#elif defined USE_JAVASCRIPTCORE
				BottomNavigator::BottomNavigator(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[])
				: impl(nullptr),
				  rootView(nullptr)
            	{
					JSObjectRef rootWinJs = jsEngine->jscGetObject(thisObj, "rootWin");
					if(!jsEngine->jscIsNull(rootWinJs))
					{
						rootView = (View*)jsEngine->jscGetExternal(jsEngine->globalContext, rootWinJs, "_appObj");
					}
            	}
            #endif

			BottomNavigator::BottomNavigator(View* view)
			{
				impl = nullptr;
				rootView = view;
			}

			void BottomNavigator::create()
			{
				ViewFrame* root = nullptr;

				if(rootView)
					root = static_cast<ViewFrame*>(rootView->_appObj);

				impl = RJNEW BottomNavigatorFrame(root);

				linkWith(impl);
			}
			int BottomNavigator::getSelectedTab(){
				if (impl)
					return impl->getSelectedTab();
				
				return 0;
			}
			void BottomNavigator::addTab(View* view, String icon)
			{
				//We can't deal with parented Views
				if (view->getParent())
					return;

				if (impl)
					impl->addTab(view, icon);
			}
		}
	}
}

