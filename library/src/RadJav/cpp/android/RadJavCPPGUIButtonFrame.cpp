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
#include "cpp/android/RadJavCPPMUIButtonFrame.h"

#include "RadJavString.h"
#include <android/Utils.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			using namespace Android;

			jclass ButtonFrame::nativeButtonClass = nullptr;

			jmethodID ButtonFrame::nativeConstructor = nullptr;
			jmethodID ButtonFrame::nativeSetText = nullptr;
			jmethodID ButtonFrame::nativeGetText = nullptr;

			ButtonFrame::ButtonFrame(GUI::GObjectWidget *parent, const String &text, const Vector2 &pos, const Vector2 &size)
			{
				/*^:AndroidJNI.outputStart ("jniCreate");*/
				/*^:AndroidJNI.outputEnd ("jniCreate");*/

				JNIEnv* env = Jni::getJniEnv();

				if (!nativeButtonClass)
				{
					Jni& jni = Jni::instance();

					nativeButtonClass = jni.findClass("android/widget/Button");

					nativeConstructor = env->GetMethodID(nativeButtonClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetText = env->GetMethodID(nativeButtonClass, "setText", "(Ljava/lang/CharSequence;)V");
					nativeGetText = env->GetMethodID(nativeButtonClass, "getText", "()Ljava/lang/CharSequence;");
				}

				auto layout = wrap_local(env, env->NewObject(nativeButtonClass, nativeConstructor, RadJav::getJavaApplication()));
				widget = env->NewGlobalRef(layout);

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
				/*^:
					RadJav.Android.runOnUiThreadAsync (["&", "text"], function (defCap, text)
						{
							let jniText = text.toWrappedJNIString ();
							let output = `
							auto jniText = ${jniText};
							${jni.Button.setText ("text")}`;

							return (output);
						});
				*/

				JNIEnv* env = Jni::getJniEnv();

				auto jtext = wrap_local(env, text.toJNIString());
				env->CallNonvirtualVoidMethod(widget, nativeButtonClass, nativeSetText, jtext.get());
            }
            
            String ButtonFrame::getText()
			{
				String text;

				JNIEnv* env = Jni::getJniEnv();

				jobject charSequence = env->CallObjectMethod(widget, nativeGetText);
				text = parseJNICharSequence(charSequence);

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
			
			bool ButtonFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				JNIEnv* env = Jni::getJniEnv();

				GUI::EventData* eventData = new GUI::EventData(this, eventName, (void*)event);
				LOGI("%s: %s", __FUNCTION__, eventData->_eventName.c_str());

				jmethodID method = nullptr;

				//setOnClickListener - Callback when the view is clickedOnItemClick
				if (eventData->_eventName.compare("click") == 0)
				{
					method = env->GetMethodID(nativeButtonClass, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
				}
					//onCreateContextMenu - Callback for pressing and holding a view for a long time
				else if (eventData->_eventName.compare("createContextMenu") == 0)
				{
					method = env->GetMethodID(nativeButtonClass, "setOnCreateContextMenuListener", "(Landroid/view/View$OnCreateContextMenuListener;)V");
				}
					//setOnDragListener - Callback when the view is dragged
					//setOnFocusChangeListener - Callback when the view changes focus
				else if (eventData->_eventName.compare("focusChange") == 0)
				{
					method = env->GetMethodID(nativeButtonClass, "setOnFocusChangeListener", "(Landroid/view/View$OnFocusChangeListener;)V");
				}
					//setOnGenericMotionListener - Callback for arbitrary gestures
					//setOnHoverListener - Callback for hovering over the view
					//setOnKeyListener - Callback for pressing a hardware key when view has focus
				else if (eventData->_eventName.compare("key") == 0)
				{
					method = env->GetMethodID(nativeButtonClass, "setOnKeyListener", "(Landroid/view/View$OnKeyListener;)V");
				}
					//setOnLongClickListener - Callback for pressing and holding a view
				else if (eventData->_eventName.compare("longClick") == 0)
				{
					method = env->GetMethodID(nativeButtonClass, "setOnLongClickListener", "(Landroid/view/View$OnLongClickListener;)V");
				}
					//setOnTouchListener - Callback for touching down or up on a view
				else if (eventData->_eventName.compare("touch") == 0)
				{
					method = env->GetMethodID(nativeButtonClass, "setOnTouchListener", "(Landroid/view/View$OnTouchListener;)V");
				}
				else
				{
					LOGE("%s: undefined event handled in button.onBindEvent [ %s ]", __FUNCTION__, eventData->_eventName.c_str());
				}

				if (method != nullptr) {
					Jni& jni = Jni::instance();

					jclass _eventListenerClass = jni.findClass("com/fogchain/radjavvm/UiEventListener");
					jmethodID _eventListenerConstructor = env->GetMethodID(_eventListenerClass, "<init>", "(Ljava/nio/ByteBuffer;)V");

					auto eventBuffer = jni.wrapLocalRef(env->NewDirectByteBuffer(eventData, sizeof(eventData)));
					auto listenerInstance = jni.wrapLocalRef(env->NewObject(_eventListenerClass, _eventListenerConstructor, eventBuffer.get()));

					env->CallVoidMethod(widget, method, listenerInstance.get());
				}

				return true;
			}
		}
	}
}

