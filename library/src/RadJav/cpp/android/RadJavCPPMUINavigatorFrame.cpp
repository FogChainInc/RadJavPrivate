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

#include "cpp/RadJavCPPMUINavigator.h"
#include "cpp/RadJavCPPMUIView.h"
#include "android/RadJavAndroid.h"


namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
			using namespace Android;

			jmethodID NavigatorFrame::nativeConstructor = nullptr;
			jmethodID NavigatorFrame::nativeAddView = nullptr;
			jmethodID NavigatorFrame::nativeRemoveView = nullptr;
			jmethodID NavigatorFrame::nativeSetLayoutTransition = nullptr;

			jclass NavigatorFrame::nativeLayoutTransitionClass = nullptr;
			jclass NavigatorFrame::nativeLayoutClass = nullptr;

			NavigatorFrame::NavigatorFrame(ViewFrame *view)
            {
				JNIEnv* env = Jni::getJniEnv();

				if (!nativeLayoutClass)
				{
					Jni& jni = Jni::instance();

					nativeLayoutTransitionClass = jni.findClass("android/animation/LayoutTransition");
					nativeLayoutClass = jni.findClass("com/fogchain/radjavvm/RadJavLayout");

					nativeConstructor = env->GetMethodID(nativeLayoutTransitionClass, "<init>", "()V");

					nativeAddView = env->GetMethodID(nativeLayoutClass, "addView", "(Landroid/view/View;)V");
					nativeRemoveView = env->GetMethodID(nativeLayoutClass, "removeView", "(Landroid/view/View;)V");
					nativeSetLayoutTransition = env->GetMethodID(nativeLayoutClass, "setLayoutTransition", "(Landroid/animation/LayoutTransition;)V");
				}

				if (view)
				{
					rootView = view->getNativeWidget();
				}
				else
				{
					rootView = RadJavAndroid::instance()->getJavaViewGroup();
				}

				auto layoutTransition = wrap_local(env, env->NewObject(nativeLayoutTransitionClass, nativeConstructor));
				env->CallVoidMethod(rootView, nativeSetLayoutTransition, layoutTransition.get());
            }

			NavigatorFrame::~NavigatorFrame()
			{
				//Remove LayoutTransition from rootView
				//TODO: need to add implementation
			}

			void NavigatorFrame::push(ViewFrame* view, bool replace)
			{
				//Do nothing if there is no view provided
				if (!view || !rootView)
					return;

				ViewFrame* viewToRemove = nullptr;

				if (!viewStack.empty())
				{
					viewToRemove = viewStack.top();

					if (replace)
					{
						viewStack.pop();
					}
				}

				viewStack.push(view);

				JNIEnv* env = Jni::getJniEnv();

				env->CallVoidMethod(rootView, nativeAddView, view->getNativeWidget());

				if (viewToRemove)
					env->CallVoidMethod(rootView, nativeRemoveView, viewToRemove->getNativeWidget());
			}

			void NavigatorFrame::pop(ViewFrame* view)
			{
				if (!rootView)
					return;

				//Do nothing if stack is empty or top most view is the same object(view)
				if (viewStack.empty() || viewStack.top() == view)
					return;

				//We always remove top most view
				ViewFrame* viewToRemove = nullptr;

				viewToRemove = viewStack.top();

				//And replace it with new one
				ViewFrame* viewToAdd = nullptr;

				//Inspect stack
				while (!viewStack.empty() &&
					   viewStack.top() != view)
				{
					viewToAdd = viewStack.top();
					viewStack.pop();
				}

				//When view was not found
				if (viewStack.empty())
				{
					//We add view provided into empty view stack
					if (view)
						viewToAdd = view;

					//Or we add an old view from the stack back to it
					viewStack.push(viewToAdd);
				}
				else
				{
					//View to activate was found in the stack
					viewToAdd = viewStack.top();
				}

				JNIEnv* env = Jni::getJniEnv();

				//Activate view
				env->CallVoidMethod(rootView, nativeAddView, viewToAdd->getNativeWidget());

				//Deactivate previous view
				if (viewToRemove)
					env->CallVoidMethod(rootView, nativeRemoveView, viewToRemove->getNativeWidget());
			}

			void NavigatorFrame::pop()
			{
				if (viewStack.size() <= 1)
					return;

				ViewFrame* viewToRemove = viewStack.top();

				viewStack.pop();

				ViewFrame* viewToAdd = viewStack.top();

				JNIEnv* env = Jni::getJniEnv();

				env->CallVoidMethod(rootView, nativeAddView, viewToAdd->getNativeWidget());
				env->CallVoidMethod(rootView, nativeRemoveView, viewToRemove->getNativeWidget());
			}
		}
    }
}
