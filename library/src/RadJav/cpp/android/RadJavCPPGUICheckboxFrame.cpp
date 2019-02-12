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
#include "cpp/android/RadJavCPPMUICheckboxFrame.h"
#include <android/Utils.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			using namespace Android;

			jclass CheckboxFrame::nativeSwitchClass = nullptr;

			jmethodID CheckboxFrame::nativeConstructor = nullptr;
			jmethodID CheckboxFrame::nativeSetChecked = nullptr;
			jmethodID CheckboxFrame::nativeIsChecked = nullptr;

			CheckboxFrame::CheckboxFrame(GUI::GObjectWidget *parent, RJBOOL checked, const Vector2 &pos, const Vector2 &size)
			{
				JNIEnv* env = Jni::getJniEnv();

				if (!nativeSwitchClass)
				{
					Jni& jni = Jni::instance();

					nativeSwitchClass = jni.findClass("android/widget/Switch");

					nativeConstructor = env->GetMethodID(nativeSwitchClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetChecked = env->GetMethodID(nativeSwitchClass, "setChecked", "(Z)V");
					nativeIsChecked = env->GetMethodID(nativeSwitchClass, "isChecked", "()Z");
				}

				auto layout = wrap_local(env, env->NewObject(nativeSwitchClass, nativeConstructor, RadJav::getJavaApplication()));
				widget = env->NewGlobalRef(layout);

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
				JNIEnv* env = Jni::getJniEnv();

				env->CallVoidMethod(widget, nativeSetChecked, checked);
			}
			
			RJBOOL CheckboxFrame::getChecked() const
			{
				RJBOOL checked;

				JNIEnv* env = Jni::getJniEnv();

				checked = env->CallBooleanMethod(widget, nativeIsChecked);

				return checked;
			}

			bool CheckboxFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				JNIEnv* env = Jni::getJniEnv();

				GUI::EventData* eventData = new GUI::EventData(this, eventName, (void*)event);
				LOGI("%s: %s", __FUNCTION__, eventData->_eventName.c_str());

				jmethodID method = nullptr;

				//setOnClickListener - Callback when the view is clicked
				if (eventData->_eventName.compare("click") == 0) {
					method = env->GetMethodID(nativeSwitchClass, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
				}
					//setOnCreateContextMenu - Callback for pressing and holding a view for a long time
				else if (eventData->_eventName.compare("createContextMenu") == 0) {
					method = env->GetMethodID(nativeSwitchClass, "setOnCreateContextMenuListener", "(Landroid/view/View$OnCreateContextMenuListener;)V");
				}
					//setOnDragListener - Callback when the view is dragged
					//setOnFocusChangeListener - Callback when the view changes focus
				else if (eventData->_eventName.compare("focusChange") == 0) {
					method = env->GetMethodID(nativeSwitchClass, "setOnFocusChangeListener", "(Landroid/view/View$OnFocusChangeListener;)V");
				}
					//setOnGenericMotionListener - Callback for arbitrary gestures
					//setOnHoverListener - Callback for hovering over the view
					//setOnKeyListener - Callback for pressing a hardware key when view has focus
				else if (eventData->_eventName.compare("key") == 0) {
					method = env->GetMethodID(nativeSwitchClass, "setOnKeyListener", "(Landroid/view/View$OnKeyListener;)V");
				}
					//setOnLongClickListener - Callback for pressing and holding a view
				else if (eventData->_eventName.compare("longClick") == 0) {
					method = env->GetMethodID(nativeSwitchClass, "setOnLongClickListener", "(Landroid/view/View$OnLongClickListener;)V");
				}
					//setOnTouchListener - Callback for touching down or up on a view
				else if (eventData->_eventName.compare("touch") == 0) {
					method = env->GetMethodID(nativeSwitchClass, "setOnTouchListener", "(Landroid/view/View$OnTouchListener;)V");
				}
					//setOnCheckedChangeListener - Callback for changing state of control
				else if (eventData->_eventName.compare("changed") == 0) {
					method = env->GetMethodID(nativeSwitchClass, "setOnCheckedChangeListener", "(Landroid/widget/CompoundButton$OnCheckedChangeListener;)V");
				}
				else {
					LOGE("%s: undefined event handled in checkboxframe.onBindEvent [ %s ]", __FUNCTION__, eventData->_eventName.c_str());
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

