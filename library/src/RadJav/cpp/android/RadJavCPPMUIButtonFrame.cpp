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
#include "cpp/RadJavCPPMUIButton.h"

#include "cpp/RadJavCPPMUIView.h"
#include "android/Utils.h"


namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			jclass ButtonFrame::nativeButtonClass = nullptr;

			jmethodID ButtonFrame::nativeConstructor = nullptr;
			jmethodID ButtonFrame::nativeSetText = nullptr;
			jmethodID ButtonFrame::nativeGetText = nullptr;

			ButtonFrame::ButtonFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
				if (!nativeButtonClass)
				{
					Jni& jni = Jni::instance();
					JNIEnv* env = jni.getJniEnv();

					nativeButtonClass = jni.findClass("android/widget/Button");

					nativeConstructor = env->GetMethodID(nativeButtonClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetText = env->GetMethodID(nativeButtonClass, "setText", "(Ljava/lang/CharSequence;)V");
					nativeGetText = env->GetMethodID(nativeButtonClass, "getText", "()Ljava/lang/CharSequence;");
				}

				RadJav::runOnUiThreadAsync([&, parent](JNIEnv* env, void* data) {
					auto layout = wrap_local(env, env->NewObject(nativeButtonClass, nativeConstructor, RadJav::getJavaApplication()));

					widget = env->NewGlobalRef(layout);
				});

				if (parent)
					parent->addChild(this);

				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			ButtonFrame::~ButtonFrame()
			{
				//TODO: add implementation
			}

            void ButtonFrame::setText(String text)
            {
            	RadJav::runOnUiThreadAsync([&, text](JNIEnv* env, void* data) {
					auto jtext = wrap_local(env, env->NewStringUTF(text.c_str()));

					env->CallNonvirtualVoidMethod(widget, nativeButtonClass, nativeSetText, jtext.get());
            	});
            }
            
            String ButtonFrame::getText()
			{
				String text;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					jobject charSequence = env->CallObjectMethod(widget, nativeGetText);
					text = utils::CharSequenceToString(charSequence);
				});

				return text;
            }
            
			void ButtonFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* ButtonFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}

			void ButtonFrame::setEnabled(RJBOOL enabled)
			{
				GObjectWidget::setEnabled(enabled);
			}
			
			RJBOOL ButtonFrame::getEnabled()
			{
				return GObjectWidget::getEnabled();
			}
			
			bool ButtonFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				//TODO: add events handling

				return false;
			}
		}
	}
}

