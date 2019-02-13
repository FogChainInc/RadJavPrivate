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
#include "cpp/android/RadJavCPPGUITextareaFrame.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			using namespace Android;

			jclass TextareaFrame::nativeEditTextClass = nullptr;

			jmethodID TextareaFrame::nativeConstructor = nullptr;
			jmethodID TextareaFrame::nativeSetText = nullptr;
			jmethodID TextareaFrame::nativeGetText = nullptr;

			TextareaFrame::TextareaFrame(GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
				JNIEnv* env = Jni::getJniEnv();

				if (!nativeEditTextClass)
				{
					Jni& jni = Jni::instance();

					nativeEditTextClass = jni.findClass("android/widget/EditText");

					nativeConstructor = env->GetMethodID(nativeEditTextClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetText = env->GetMethodID(nativeEditTextClass, "setText", "(Ljava/lang/CharSequence;)V");
					nativeGetText = env->GetMethodID(nativeEditTextClass, "getText", "()Ljava/lang/CharSequence;");
				}

				auto layout = wrap_local(env, env->NewObject(nativeEditTextClass, nativeConstructor, RadJav::getJavaApplication()));
				widget = env->NewGlobalRef(layout);

				if (parent)
					parent->addChild(this);

				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			TextareaFrame::~TextareaFrame()
			{
				//TODO: Add implementation
			}
			
			void TextareaFrame::setText(String text)
			{
				JNIEnv* env = Jni::getJniEnv();

				auto jtext = wrap_local(env, text.toJNIString());
				env->CallNonvirtualVoidMethod(widget, nativeEditTextClass, nativeSetText, jtext.get());
			}
			
			String TextareaFrame::getText()
			{
				String text;

				JNIEnv* env = Jni::getJniEnv();

				jobject charSequence = env->CallObjectMethod(widget, nativeGetText);
				text = parseJNICharSequence(charSequence);

				return text;
			}
			
			void TextareaFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* TextareaFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			bool TextareaFrame::bindEvent(const String& eventName, const CPP::Event* /*event*/)
			{
				return false;
			}
		}
	}
}

