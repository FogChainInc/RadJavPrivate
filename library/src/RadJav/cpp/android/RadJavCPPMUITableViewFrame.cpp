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
#include "cpp/RadJavCPPMUITableView.h"
#include "cpp/RadJavCPPGUIGObject.h"
#include "android/Utils.h"
#include "android/NativeCallbackFunction.h"
#include "cpp/android/RadJavCPPMUIListAdapter.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			using namespace Android;

			jclass TableViewFrame::nativeListViewClass = nullptr;

			jmethodID TableViewFrame::nativeConstructor = nullptr;
			jmethodID TableViewFrame::nativeSetAdapter = nullptr;

			TableViewFrame::TableViewFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
				if (!nativeListViewClass)
				{
					Jni& jni = Jni::instance();
					JNIEnv* env = jni.getJniEnv();

					nativeListViewClass = jni.findClass("android/widget/ListView");

					nativeConstructor = env->GetMethodID(nativeListViewClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetAdapter = env->GetMethodID(nativeListViewClass, "setAdapter", "(Landroid/widget/ListAdapter;)V");
				}

				listAdapter = RJNEW ListAdapter();

				RadJav::runOnUiThreadAsync([&, parent](JNIEnv* env, void* data) {
					auto nativeWidget = wrap_local(env, env->NewObject(nativeListViewClass, nativeConstructor, RadJav::getJavaApplication()));

					widget = env->NewGlobalRef(nativeWidget);

					env->CallVoidMethod(widget, nativeSetAdapter, listAdapter->getNativeObject());
				});

				if (parent)
					parent->addChild(this);

				setText(text);
				setSize(size);
				setPosition(pos);
			}

			void TableViewFrame::setModel(MUI::TableViewModel *model)
			{
				if (listAdapter)
				{
					//TODO: Add model interface into Adapter
					//listAdapter->setModel();
				}

				this->model = model;

				reload();
			}

			void TableViewFrame::reload()
			{
				//TODO: add implementation
			}


			TableViewFrame::~TableViewFrame()
			{
				//TODO: add implementation
				if (listAdapter)
				{
					RJDELETE listAdapter;
					listAdapter = nullptr;
				}
			}

			bool TableViewFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				return false;
			}
		}
	}
}
