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
#include "cpp/RadJavCPPMUILabel.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			jclass LabelFrame::nativeTextViewClass = nullptr;

			jmethodID LabelFrame::nativeConstructor = nullptr;
			jmethodID LabelFrame::nativeSetText = nullptr;
			jmethodID LabelFrame::nativeGetText = nullptr;

			LabelFrame::LabelFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
				if (!nativeTextViewClass)
				{
					Jni& jni = Jni::instance();
					JNIEnv* env = jni.getJniEnv();

					nativeTextViewClass = jni.findClass("android/widget/TextView");

					nativeConstructor = env->GetMethodID(nativeTextViewClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetText = env->GetMethodID(nativeTextViewClass, "setText", "(Ljava/lang/CharSequence;)V");
					nativeGetText = env->GetMethodID(nativeTextViewClass, "getText", "()Ljava/lang/CharSequence;");
				}

				RadJav::runOnUiThreadAsync([&, parent](JNIEnv* env, void* data) {
					auto layout = wrap_local(env, env->NewObject(nativeTextViewClass, nativeConstructor, RadJav::getJavaApplication()));

					widget = env->NewGlobalRef(layout);
				});

				if (parent)
					parent->addChild(this);

				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			LabelFrame::~LabelFrame()
			{
				//TODO: add implementation
			}
			
			void LabelFrame::setText(String text)
			{
				RadJav::runOnUiThreadAsync([&, text](JNIEnv* env, void* data) {
					auto jtext = wrap_local(env, text.toJNIString());

					env->CallNonvirtualVoidMethod(widget, nativeTextViewClass, nativeSetText, jtext.get());
				});
			}
			
			String LabelFrame::getText()
			{
				String text;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					jobject charSequence = env->CallObjectMethod(widget, nativeGetText);
					text = parseJNICharSequence(charSequence);
				});

				return text;
			}
			
			void LabelFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* LabelFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			bool LabelFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				//TODO: do we need to handle UILabel events?
				return false;
			}
		}
	}
}

