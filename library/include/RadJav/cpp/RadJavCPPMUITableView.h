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
#ifndef _RADJAV_MUI_CPP_TABLEVIEW_H_
#define _RADJAV_MUI_CPP_TABLEVIEW_H_

#include "cpp/RadJavCPPGUIGObject.h"
#include "cpp/RadJavCPPMUITableViewModel.h"
#include "cpp/RadJavCPPMUIView.h"
#include "cpp/RadJavCPPMUITableViewCellCreator.h"

namespace RadJAV
{
	namespace CPP
	{
		class Persistent;
		
		namespace MUI
		{
			class View;

			/**
			 * @ingroup group_mui_cpp
			 * @brief TableView class.
			 */
			class RADJAV_EXPORT TableView : public CPP::GUI::GObject,
											public TableViewCellCreator
			{
				public:
					#ifdef USE_V8
						TableView(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
					#endif
					#ifdef USE_JAVASCRIPTCORE
						TableView(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
					#endif
					TableView(String name, String text = "", CPP::GUI::GObject *parent = NULL);

					~TableView();

					void create();
				
					#ifdef USE_V8
						void setModel(v8::Local<v8::Object> model);
					#elif defined USE_JAVASCRIPTCORE
						void setModel(JSObjectRef model);
					#endif
				
					View* createViewForItem(unsigned int itemIndex);

					#if defined USE_V8 || defined USE_JAVASCRIPTCORE
						/// Set TableView item delegate
						void setDelegate(RJ_FUNC_TYPE delegateFunction);

						/// Execute when an event is triggered.
						void on(String event, RJ_FUNC_TYPE func);
					#endif

				protected:
					Persistent* delegateJs;
					Persistent* modelJs;
			};
		}
	}
}
#endif
