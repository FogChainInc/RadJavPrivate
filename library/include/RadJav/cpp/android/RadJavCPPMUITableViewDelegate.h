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

#ifndef _RADJAV_MUI_CPP_TABLEVIEWDELEGATE_H_
#define _RADJAV_MUI_CPP_TABLEVIEWDELEGATE_H_

#include "android/Jni.h"
#include "cpp/RadJavCPPMUITableViewCellCreator.h"
#include "cpp/RadJavCPPEvent.h"

#include <vector>

namespace RadJAV
{
	namespace Android
	{
		class NativeCallbackFunction;
	}

	namespace CPP
	{
		namespace MUI
		{
			class TableViewModel;

			class TableViewDelegate
			{
			public:
				TableViewDelegate(TableViewCellCreator& cellCreator);
				virtual ~TableViewDelegate();

				void setModel(TableViewModel* listModel);

				jobject getNativeObject() const;

			protected:
				virtual void registerCallbacks();
				virtual void registerCallback(JNIEnv* env, const char* callbackType, Android::NativeCallbackFunction* function);

			protected:
				TableViewModel* model;
				TableViewCellCreator& viewCreator;

				jobject object;
				std::vector<Android::NativeCallbackFunction*> callbacks;

				static jclass listViewClass;
				static jclass listAdapterClass;
				static jclass nativeCallbackClass;

				static jmethodID listAdapterConstructor;
				static jmethodID nativeCallbackConstructor;
				static jmethodID setCallback;
				static jmethodID notifyChanged;
				static jmethodID notifyInvalidated;
			};
		}
	}
}

#endif
