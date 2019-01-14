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
#include "cpp/RadJavCPPMUIScrollView.h"
#include "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			jmethodID ScrollViewFrame::nativeConstructor = nullptr;
            jmethodID ScrollViewFrame::nativeAddView = nullptr;
            jmethodID ScrollViewFrame::nativeRemoveView = nullptr;

            jclass ScrollViewFrame::nativeLayoutClass = nullptr;
            void ScrollViewFrame::initNatives()
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
			ScrollViewFrame::ScrollViewFrame(GUI::GObjectWidget *parent, const Vector2 &pos, const Vector2 &size)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = jni.getJniEnv();

				if (!nativeLayoutClass) {
					//best option so far, but under apache 2.0 license
                    //nativeLayoutClass = jni.findClass("com/fogchain/radjavvm/TwoDScrollView");

					//option #2, simpler but gives worse look&feel, still this can be enough for our needs
					//nativeLayoutClass = jni.findClass("com/fogchain/radjavvm/BiDirectionScrollView");

					//debug option #3 - simple view instead of scrollview
                    nativeLayoutClass = jni.findClass("com/fogchain/radjavvm/RadJavLayout");

                    nativeConstructor = env->GetMethodID(nativeLayoutClass, "<init>", "(Landroid/content/Context;)V");
                    nativeAddView = env->GetMethodID(nativeLayoutClass, "addView", "(Landroid/view/View;)V");
					//nativeSetContentSize;
					//nativeGetContentSize;
				}
                    RadJav::runOnUiThreadAsync([&, parent](JNIEnv* env, void* data) {
					auto layout = wrap_local(env, env->NewObject(nativeLayoutClass, nativeConstructor, RadJav::getJavaApplication()));

					widget = env->NewGlobalRef(layout);
            	});

				if (parent)
					parent->addChild(this);


			}


			ScrollViewFrame::~ScrollViewFrame()
			{
				//TODO: add implementation
			}


            void ScrollViewFrame::setEnabled(RJBOOL enabled)
            {
                GObjectWidget::setEnabled(enabled);
            }

            RJBOOL ScrollViewFrame::getEnabled()
            {
                return GObjectWidget::getEnabled();
            }

            void ScrollViewFrame::addChild(GUI::GObjectWidget *child)
            {
				RadJav::runOnUiThreadAsync([&, child](JNIEnv* env, void* data) {
					env->CallVoidMethod(widget, nativeAddView, child->getNativeWidget());
				});
            }

//            void ScrollViewFrame::setContentSize(const CPP::Vector2& size)
//            {
//                //TODO: implement
//            }
//
//
//			CPP::Vector2 ScrollViewFrame::getContentSize() const
//			{
//				//TODO: implement
//			}

/**
 * binding events will probably override the fallback bindings. TODO: verify this
 */
			bool ScrollViewFrame::bindEvent(const String& eventName, const GUI::Event* event)
			{
				RadJav::runOnUiThreadAsync([&, eventName, event](JNIEnv* env, void* data) {
					GUI::EventData* eventData = new GUI::EventData(this, eventName, (void*)event);
					LOGI("%s: %s", __FUNCTION__, eventData->_eventName.c_str());

					jmethodID method = nullptr;

					{
						LOGE("%s: undefined event handled in button.onBindEvent [ %s ]", __FUNCTION__, eventData->_eventName.c_str());
					}

				});

				return false;
			}
		}
	}
}
