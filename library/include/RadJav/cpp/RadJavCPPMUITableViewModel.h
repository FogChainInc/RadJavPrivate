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
#ifndef _RADJAV_MUI_CPP_TABLEVIEWMODEL_H_
#define _RADJAV_MUI_CPP_TABLEVIEWMODEL_H_

#include "RadJavPreprocessor.h"
#include "cpp/RadJavCPPChainedPtr.h"

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#elif defined USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCJavascriptEngine.h"
#endif


	namespace RadJAV
	{
		namespace CPP
		{
			namespace MUI
			{
				class RADJAV_EXPORT TableViewModel : public ChainedPtr
				{
					public:
						#ifdef USE_V8
							TableViewModel(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
						#ifdef USE_JAVASCRIPTCORE
							TableViewModel(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
						#endif
						TableViewModel();

						void itemAdded(unsigned int index);
						void itemRemoved(unsigned int index);
						void itemsCleared();

						unsigned int size() const;
					
						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
							/// Execute when an event is triggered.
							void on(String event, RJ_FUNC_TYPE func);
						#endif
					protected:
						unsigned int itemsCount;
				};
			}
		}
	}
#endif

