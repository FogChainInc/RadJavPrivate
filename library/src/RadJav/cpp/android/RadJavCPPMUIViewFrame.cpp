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
#include "cpp/RadJavCPPMUIView.h"

#include "cpp/RadJavCPPGUIGObject.h"


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
        	using namespace Android;

			jmethodID ViewFrame::nativeConstructor = nullptr;
			jmethodID ViewFrame::nativeAddView = nullptr;
			jmethodID ViewFrame::nativeRemoveView = nullptr;

			jclass ViewFrame::nativeLayoutClass = nullptr;

			void ViewFrame::initNatives()
			{
				if (!nativeLayoutClass)
				{
					Jni& jni = Jni::instance();
					JNIEnv* env = jni.getJniEnv();

					nativeLayoutClass = jni.findClass("com/fogchain/radjavvm/RadJavLayout");

					nativeConstructor = env->GetMethodID(nativeLayoutClass, "<init>", "(Landroid/content/Context;)V");
					nativeAddView = env->GetMethodID(nativeLayoutClass, "addView", "(Landroid/view/View;)V");
					nativeRemoveView = env->GetMethodID(nativeLayoutClass, "removeView", "(Landroid/view/View;)V");
				}
			}

			ViewFrame::ViewFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
            {
            	initNatives();

            	RadJav::runOnUiThreadAsync([&, parent](JNIEnv* env, void* data) {
					auto layout = wrap_local(env, env->NewObject(nativeLayoutClass, nativeConstructor, RadJav::getJavaApplication()));

					widget = env->NewGlobalRef(layout);
            	});

            	if (parent)
            		parent->addChild(this);

				setText(text);
            }

			ViewFrame::ViewFrame(const String &text, const Vector2 &pos, const Vector2 &size)
			{
				//Constructor to make this View point to RootView created at app startup

				initNatives();

				Jni& jni = Jni::instance();
				widget = jni.getJniEnv()->NewGlobalRef(RadJav::getJavaViewGroup());

				setText(text);
			}

			ViewFrame::~ViewFrame()
			{
				//Release native widget here
				//TODO: need to add implementation
			}

			void ViewFrame::addChild(GUI::GObjectWidget *child)
			{
				RadJav::runOnUiThreadAsync([&, child](JNIEnv* env, void* data) {
					env->CallVoidMethod(widget, nativeAddView, child->getNativeWidget());
				});
			}

			void ViewFrame::setText(String text)
			{
				//TODO: Add implementation
			}
			
			String ViewFrame::getText()
			{
				//TODO: Add implementation
				return String();
			}
			
			bool ViewFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				return false;
			}
        }
    }
}
