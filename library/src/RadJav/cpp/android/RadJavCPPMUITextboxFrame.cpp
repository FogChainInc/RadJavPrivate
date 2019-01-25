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
#include "cpp/RadJavCPPMUITextbox.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			using namespace Android;

			jclass TextboxFrame::nativeEditTextClass = nullptr;

			jmethodID TextboxFrame::nativeConstructor = nullptr;
			jmethodID TextboxFrame::nativeSetText = nullptr;
			jmethodID TextboxFrame::nativeGetText = nullptr;
			jmethodID TextboxFrame::nativeSetInputType = nullptr;
			jmethodID TextboxFrame::nativeGetInputType = nullptr;
			jmethodID TextboxFrame::nativeSetLines = nullptr;

			TextboxFrame::TextboxFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
				JNIEnv* env = Jni::getJniEnv();

				if (!nativeEditTextClass)
				{
					Jni& jni = Jni::instance();

					nativeEditTextClass = jni.findClass("android/widget/EditText");

					nativeConstructor = env->GetMethodID(nativeEditTextClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetText = env->GetMethodID(nativeEditTextClass, "setText", "(Ljava/lang/CharSequence;)V");
					nativeGetText = env->GetMethodID(nativeEditTextClass, "getText", "()Ljava/lang/CharSequence;");
					nativeSetInputType = env->GetMethodID(nativeEditTextClass, "setInputType", "(I)V");
					nativeGetInputType = env->GetMethodID(nativeEditTextClass, "getInputType", "()I");
					nativeSetLines = env->GetMethodID(nativeEditTextClass, "setLines", "(I)V");
				}

				auto layout = wrap_local(env, env->NewObject(nativeEditTextClass, nativeConstructor, RadJav::getJavaApplication()));
				widget = env->NewGlobalRef(layout);

				//One line EditText control
				env->CallVoidMethod(widget, nativeSetLines, 1);

				if (parent)
					parent->addChild(this);

				setText(text);
				setSize(size);
				setPosition(pos);
			}
			
			TextboxFrame::~TextboxFrame()
			{
				//TODO: Add implementation
			}
			
			void TextboxFrame::setText(String text)
			{
				JNIEnv* env = Jni::getJniEnv();

				auto jtext = wrap_local(env, text.toJNIString());
				env->CallNonvirtualVoidMethod(widget, nativeEditTextClass, nativeSetText, jtext.get());
			}
			
			String TextboxFrame::getText()
			{
				String text;

				JNIEnv* env = Jni::getJniEnv();

				jobject charSequence = env->CallObjectMethod(widget, nativeGetText);
				text = parseJNICharSequence(charSequence);

				return text;
			}
			
			void TextboxFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
			}
			
			CPP::Font* TextboxFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void TextboxFrame::setInputMode(Textbox::InputMode mode)
			{
				int keyboardType = 0x01; //TYPE_CLASS_TEXT

				switch (mode)
				{
					case Textbox::InputMode::Text:
						keyboardType = 0x01; //TYPE_CLASS_TEXT
						break;
					case Textbox::InputMode::Number:
						keyboardType = 0x02; //TYPE_CLASS_NUMBER
						break;
					case Textbox::InputMode::Decimal:
						keyboardType = 0x02 | 0x2000; //TYPE_CLASS_NUMBER | TYPE_NUMBER_FLAG_DECIMAL
						break;
					case Textbox::InputMode::Phone:
						keyboardType = 0x03; //TYPE_CLASS_PHONE
						break;
					case Textbox::InputMode::Email:
						keyboardType = 0x01 | 0x20; //TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_EMAIL_ADDRESS
						break;
					case Textbox::InputMode::Password:
						keyboardType = 0x01 | 0x80; //TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_PASSWORD
                        break;
					default:
					    break;
				}

				JNIEnv* env = Jni::getJniEnv();

				env->CallVoidMethod(widget, nativeSetInputType, keyboardType);
			}
			
			Textbox::InputMode TextboxFrame::getInputMode() const
			{
				int keyboardType;

				JNIEnv* env = Jni::getJniEnv();

				keyboardType = env->CallIntMethod(widget, nativeGetInputType);

				switch (keyboardType)
				{
					case 0x02: //TYPE_CLASS_NUMBER
						return Textbox::InputMode::Number;
					case 0x02 | 0x2000: //TYPE_CLASS_NUMBER | TYPE_NUMBER_FLAG_DECIMAL
						return Textbox::InputMode::Decimal;
					case 0x03: //TYPE_CLASS_PHONE
						return Textbox::InputMode::Phone;
					case 0x02 | 0x20: //TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_EMAIL_ADDRESS
						return Textbox::InputMode::Email;
					case 0x02 | 0x80: //TYPE_CLASS_TEXT | TYPE_TEXT_VARIATION_PASSWORD
						return Textbox::InputMode::Password;
					case 0x01: //TYPE_CLASS_TEXT
					default:
						return Textbox::InputMode::Text;
				}
			}

			bool TextboxFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				//TODO: Add implementation
			}
		}
	}
}

