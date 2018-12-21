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
#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			jmethodID GObjectWidget::nativeSetLeft = nullptr;
			jmethodID GObjectWidget::nativeSetTop = nullptr;
			jmethodID GObjectWidget::nativeSetRight = nullptr;
			jmethodID GObjectWidget::nativeSetBottom = nullptr;
			jmethodID GObjectWidget::nativeSetVisibility = nullptr;
			jmethodID GObjectWidget::nativeSetEnabled = nullptr;

			jmethodID GObjectWidget::nativeGetLeft = nullptr;
			jmethodID GObjectWidget::nativeGetTop = nullptr;
			jmethodID GObjectWidget::nativeGetRight = nullptr;
			jmethodID GObjectWidget::nativeGetBottom = nullptr;
			jmethodID GObjectWidget::nativeGetWidth = nullptr;
			jmethodID GObjectWidget::nativeGetHeight = nullptr;
			jmethodID GObjectWidget::nativeGetVisibility = nullptr;
			jmethodID GObjectWidget::nativeIsEnabled = nullptr;

			jclass GObjectWidget::nativeViewClass = nullptr;

			GObjectWidget::GObjectWidget()
			{
				widget = nullptr;
				parent = nullptr;

				//Store native pointers
				if (nativeViewClass)
					return;

				//Init statics for faster access
				Jni& jni = Jni::instance();
				JNIEnv* env = jni.getJniEnv();

				nativeViewClass = jni.findClass("android/view/View");

				nativeSetLeft = env->GetMethodID(nativeViewClass, "setLeft", "(I)V");
				nativeSetTop = env->GetMethodID(nativeViewClass, "setTop", "(I)V");
				nativeSetRight = env->GetMethodID(nativeViewClass, "setRight", "(I)V");
				nativeSetBottom = env->GetMethodID(nativeViewClass, "setBottom", "(I)V");
				nativeSetVisibility = env->GetMethodID(nativeViewClass, "setVisibility", "(I)V");
				nativeSetEnabled = env->GetMethodID(nativeViewClass, "setEnabled", "(Z)V");

				nativeGetLeft = env->GetMethodID(nativeViewClass, "getLeft", "()I");
				nativeGetTop = env->GetMethodID(nativeViewClass, "getTop", "()I");
				nativeGetRight = env->GetMethodID(nativeViewClass, "getRight", "()I");
				nativeGetBottom = env->GetMethodID(nativeViewClass, "getBottom", "()I");
				nativeGetWidth = env->GetMethodID(nativeViewClass, "getWidth", "()I");
				nativeGetHeight = env->GetMethodID(nativeViewClass, "getHeight", "()I");
				nativeGetVisibility = env->GetMethodID(nativeViewClass, "getVisibility", "()I");
				nativeIsEnabled = env->GetMethodID(nativeViewClass, "isEnabled", "()Z");
			}

			void GObjectWidget::addChild(GObjectWidget *child)
			{
				//This functionality is only available for ViewGroup derived Containers/Windows
				//Controls lack this functionality since they derived from View
				//jobject widget = getNativeWidget();
				//jobject childObj = child->_appObj->getNativeWidget();


				//Call addView on Java side to add Control to ViewGroup derived Window
			}
			
			void GObjectWidget::setFont(CPP::Font *font)
			{
				//Do nothing here
			}
			
			CPP::Font* GObjectWidget::getFont()
			{
				//Do nothing here
				return nullptr;
			}
			
			void GObjectWidget::setPosition(RJINT x, RJINT y)
			{
				RadJav::runOnUiThreadAsync([& ,x, y](JNIEnv* env, void* data) {
					env->CallNonvirtualVoidMethod(widget, nativeViewClass, nativeSetLeft, x);
					env->CallNonvirtualVoidMethod(widget, nativeViewClass, nativeSetTop, y);
				});
			}
			
			void GObjectWidget::setPosition(CPP::Vector2 pos)
			{
				setPosition(pos.x, pos.y);
			}
			
			CPP::Vector2 GObjectWidget::getPosition()
			{
				return Vector2(getX(), getY());
			}
			
			RJINT GObjectWidget::getX()
			{
				RJINT x = 0;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					x = env->CallNonvirtualIntMethod(widget, nativeViewClass, nativeGetLeft);
				});

				return x;
			}
			
			RJINT GObjectWidget::getY()
			{
				RJINT y = 0;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					y = env->CallNonvirtualIntMethod(widget, nativeViewClass, nativeGetLeft);
				});

				return y;
			}
			
			void GObjectWidget::setSize(RJINT width, RJINT height)
			{
				RadJav::runOnUiThreadAsync([&, width, height](JNIEnv* env, void* data) {
					jint right = env->CallNonvirtualIntMethod(widget, nativeViewClass, nativeGetLeft);
					jint bottom = env->CallNonvirtualIntMethod(widget, nativeViewClass, nativeGetTop);

					right += width;
					bottom += height;

					env->CallNonvirtualVoidMethod(widget, nativeViewClass, nativeSetRight, right);
					env->CallNonvirtualVoidMethod(widget, nativeViewClass, nativeSetBottom, bottom);
				});
			}
			
			void GObjectWidget::setSize(CPP::Vector2 size)
			{
				setSize(size.x, size.y);
			}
			
			CPP::Vector2 GObjectWidget::getSize()
			{
				return Vector2(getWidth(), getHeight());
			}
			
			RJINT GObjectWidget::getWidth()
			{
				RJINT width = 0;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					width = env->CallNonvirtualIntMethod(widget, nativeViewClass, nativeGetWidth);
				});

				return width;
			}
			
			RJINT GObjectWidget::getHeight()
			{
				RJINT height = 0;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					height = env->CallNonvirtualIntMethod(widget, nativeViewClass, nativeGetHeight);
				});

				return height;
			}
			
			void GObjectWidget::setText(String text)
			{
				//Do nothing here
			}
			
			String GObjectWidget::getText()
			{
				//Do nothing here
				return String();
			}
			
			GObjectWidget* GObjectWidget::getParent()
			{
				return parent;
			}
			
			void GObjectWidget::setVisibility(RJBOOL visible)
			{
				RadJav::runOnUiThreadAsync([&, visible](JNIEnv* env, void* data) {
					env->CallNonvirtualVoidMethod(widget, nativeViewClass, nativeSetVisibility, visible ? 0x0 : 0x4);
				});
			}
			
			RJBOOL GObjectWidget::getVisibility()
			{
				bool visible = true;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					visible = env->CallIntMethod(widget, nativeGetVisibility) == 0x0;
				});

				return visible;
			}
			
			void GObjectWidget::setEnabled(RJBOOL enabled)
			{
				RadJav::runOnUiThreadAsync([&, enabled](JNIEnv* env, void* data) {
					env->CallVoidMethod(widget, nativeSetEnabled, enabled);
				});
			}
			
			RJBOOL GObjectWidget::getEnabled()
			{
				RJBOOL enabled = true;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					enabled = env->CallBooleanMethod(widget, nativeIsEnabled);
				});

				return enabled;
			}

			jobject GObjectWidget::getNativeWidget()
			{
				return widget;
			}
		}
	}
}
