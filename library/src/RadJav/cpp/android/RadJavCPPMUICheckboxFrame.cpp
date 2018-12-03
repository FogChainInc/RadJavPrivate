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
#include <android/Utils.h>
#include "cpp/RadJavCPPMUICheckbox.h"
#include "cpp/RadJavCPPMUIView.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			jclass CheckboxFrame::nativeSwitchClass = nullptr;

			jmethodID CheckboxFrame::nativeConstructor = nullptr;
			jmethodID CheckboxFrame::nativeSetChecked = nullptr;
			jmethodID CheckboxFrame::nativeIsChecked = nullptr;

			CheckboxFrame::CheckboxFrame(GUI::GObjectWidget *parent, RJBOOL checked, const Vector2 &pos, const Vector2 &size)
			{
				if (!nativeSwitchClass)
				{
					Jni& jni = Jni::instance();
					JNIEnv* env = jni.getJniEnv();

					nativeSwitchClass = jni.findClass("android/widget/Switch");

					nativeConstructor = env->GetMethodID(nativeSwitchClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetChecked = env->GetMethodID(nativeSwitchClass, "setChecked", "(Z)V");
					nativeIsChecked = env->GetMethodID(nativeSwitchClass, "isChecked", "()Z");
				}

				RadJav::runOnUiThreadAsync([&, parent](JNIEnv* env, void* data) {
					auto layout = wrap_local(env, env->NewObject(nativeSwitchClass, nativeConstructor, RadJav::getJavaApplication()));

					widget = env->NewGlobalRef(layout);
				});

				if (parent)
					parent->addChild(this);

				setChecked(checked);
				setSize(size);
				setPosition(pos);
			}
			
			CheckboxFrame::~CheckboxFrame()
			{
				//TODO: Add implementation
			}
			
			void CheckboxFrame::setChecked(RJBOOL checked)
			{
				RadJav::runOnUiThreadAsync([&, checked](JNIEnv* env, void* data) {
					env->CallVoidMethod(widget, nativeSetChecked, checked);
				});
			}
			
			RJBOOL CheckboxFrame::getChecked() const
			{
				RJBOOL checked;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					checked = env->CallBooleanMethod(widget, nativeIsChecked);
				});

				return checked;
			}

			bool CheckboxFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				RadJav::runOnUiThreadAsync([&, eventName, event](JNIEnv* env, void* data) {
					Jni& jni = Jni::instance();
					GUI::EventData* eventData = new GUI::EventData(this, eventName, (void*)event);
					 //CheckboxFrame* checkbox = static_cast<CheckboxFrame*> (eventData->_widget);
                    jclass _eventListenerClass = jni.findClass("com/fogchain/radjavvm/UiEventListener");
                    jmethodID _eventListenerConstructor = env->GetMethodID(_eventListenerClass, "<init>", "(Ljava/nio/ByteBuffer;)V");
                    LOGI("%s: %s", __FUNCTION__, eventData->_eventName.c_str());

                    if (eventData->_eventName.compare("click") == 0) {
                        jmethodID setOnClickListener = env->GetMethodID(nativeSwitchClass, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");

                        auto eventBuffer = jni.wrapLocalRef(env->NewDirectByteBuffer(eventData, sizeof(eventData)));
                        auto listenerInstance = jni.wrapLocalRef(env->NewObject(_eventListenerClass, _eventListenerConstructor, eventBuffer.get()));

                        env->CallVoidMethod(widget, setOnClickListener, listenerInstance.get());
                    }
                    else if (eventData->_eventName.compare("longClick") == 0) {
                        jmethodID setOnLongClickListener = env->GetMethodID(nativeSwitchClass, "setOnLongClickListener", "(Landroid/view/View$OnLongClickListener;)V");

                        auto eventBuffer = jni.wrapLocalRef(env->NewDirectByteBuffer(eventData, sizeof(eventData)));
                        auto listenerInstance = jni.wrapLocalRef(env->NewObject(_eventListenerClass, _eventListenerConstructor, eventBuffer.get()));

                        env->CallVoidMethod(widget, setOnLongClickListener, listenerInstance.get());
                    }
                    else if (eventData->_eventName.compare("changed") == 0) {
                        jclass _class = jni.findClass("android/widget/CompoundButton");

                        jmethodID setOnCheckedChangeListener = env->GetMethodID(_class, "setOnCheckedChangeListener", "(Landroid/widget/CompoundButton$OnCheckedChangeListener;)V");

                        auto eventBuffer = jni.wrapLocalRef(env->NewDirectByteBuffer(eventData, sizeof(eventData)));
                        auto listenerInstance = jni.wrapLocalRef(env->NewObject(_eventListenerClass, _eventListenerConstructor, eventBuffer.get()));

                        env->CallVoidMethod(widget, setOnCheckedChangeListener, listenerInstance.get());
                    }
                    else {
                        LOGE("%s: undefined event handled in button.onBindEvent [ %s ]", __FUNCTION__, eventData->_eventName.c_str());
                    }
                });

				return true;
			}
		}
	}
}

