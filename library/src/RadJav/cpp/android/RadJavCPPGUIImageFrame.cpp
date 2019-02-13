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
#include "cpp/android/RadJavCPPGUIImageFrame.h"
#include <android/Utils.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			using namespace Android;

			jclass ImageFrame::nativeImageViewClass = nullptr;

			jmethodID ImageFrame::nativeConstructor = nullptr;
			jmethodID ImageFrame::nativeSetImageBitmap = nullptr;
			jmethodID ImageFrame::nativeSetScaleType = nullptr;
			jmethodID ImageFrame::nativeGetScaleType = nullptr;

			ImageFrame::ImageFrame(GObjectWidget *parent, const String &imageFile, const Vector2 &pos, const Vector2 &size)
			{
				JNIEnv* env = Jni::getJniEnv();

				if (!nativeImageViewClass)
				{
					Jni& jni = Jni::instance();

					nativeImageViewClass = jni.findClass("android/widget/ImageView");

					nativeConstructor = env->GetMethodID(nativeImageViewClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetImageBitmap = env->GetMethodID(nativeImageViewClass, "setImageBitmap", "(Landroid/graphics/Bitmap;)V");
					nativeSetScaleType = env->GetMethodID(nativeImageViewClass, "setScaleType", "(Landroid/widget/ImageView/ScaleType;)V");
					nativeGetScaleType = env->GetMethodID(nativeImageViewClass, "getScaleType", "()Landroid/widget/ImageView/ScaleType;");
				}

				auto layout = wrap_local(env, env->NewObject(nativeImageViewClass, nativeConstructor, RadJav::getJavaApplication()));
				widget = env->NewGlobalRef(layout);

				if (parent)
					parent->addChild(this);

				setSize(size);
				setPosition(pos);

				if (imageFile.size())
					loadImage(imageFile);
			}
			
			ImageFrame::ImageFrame(GObjectWidget *parent, const Vector2 &pos, const Vector2 &size)
			{
				JNIEnv* env = Jni::getJniEnv();

				if (!nativeImageViewClass)
				{
					Jni& jni = Jni::instance();

					nativeImageViewClass = jni.findClass("android/widget/ImageView");

					nativeConstructor = env->GetMethodID(nativeImageViewClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetImageBitmap = env->GetMethodID(nativeImageViewClass, "setImageBitmap", "(Landroid/graphics/Bitmap;)V");
					nativeSetScaleType = env->GetMethodID(nativeImageViewClass, "setScaleType", "(Landroid/widget/ImageView$ScaleType;)V");
					nativeGetScaleType = env->GetMethodID(nativeImageViewClass, "getScaleType", "()Landroid/widget/ImageView$ScaleType;");
				}

				auto layout = wrap_local(env, env->NewObject(nativeImageViewClass, nativeConstructor, RadJav::getJavaApplication()));
				widget = env->NewGlobalRef(layout);

				if (parent)
					parent->addChild(this);

				setSize(size);
				setPosition(pos);

				setSize(size);
				setPosition(pos);
			}

			ImageFrame::~ImageFrame()
			{
				//TODO: Add implementation
			}
			
			RJBOOL ImageFrame::loadImage(const String& imageFile)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = Jni::getJniEnv();

				jclass bitmapFactoryClass = jni.findClass("android/graphics/BitmapFactory");

				jmethodID decodeFileMethod = env->GetStaticMethodID(bitmapFactoryClass, "decodeFile", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");

				auto imagePathJava = wrap_local(env, env->NewStringUTF(imageFile.c_str()));

				jobject bitmap = env->CallStaticObjectMethod(bitmapFactoryClass, decodeFileMethod, imagePathJava.get());

				if (Utils::IsNull(bitmap))
					return false;

				bitmap = env->NewGlobalRef(bitmap);

				env->CallVoidMethod(widget, nativeSetImageBitmap, bitmap);
				env->DeleteGlobalRef(bitmap);

				return true;
			}
			
			void ImageFrame::setScaleMode(Image::ScaleMode mode)
			{
				Jni& jni = Jni::instance();
				JNIEnv* env = Jni::getJniEnv();

				jclass scaleTypeClass = jni.findClass("android/widget/ImageView$ScaleType");

				switch (mode)
				{
					case Image::ScaleMode::AspectFit:
					{
						jfieldID center_inside = env->GetStaticFieldID(scaleTypeClass,
																	   "CENTER_INSIDE",
																	   "Landroid/widget/ImageView$ScaleType;");

						auto centerInsideValue = wrap_local(env, env->GetStaticObjectField(scaleTypeClass, center_inside));
						env->CallVoidMethod(widget, nativeSetScaleType, centerInsideValue.get());

						break;
					}
					case Image::ScaleMode::AspectFill:
					{
						jfieldID center_crop = env->GetStaticFieldID(scaleTypeClass,
																	 "CENTER_CROP",
																	 "Landroid/widget/ImageView$ScaleType;");

						auto centerCropValue = wrap_local(env, env->GetStaticObjectField(scaleTypeClass, center_crop));
						env->CallVoidMethod(widget, nativeSetScaleType,centerCropValue.get());

						break;
					}
					default:;
				}
			}

			Image::ScaleMode ImageFrame::getScaleMode() const
			{
				Image::ScaleMode scaleMode = Image::ScaleMode::AspectFit;
				JNIEnv* env = Jni::getJniEnv();

				auto scaleModeJava = wrap_local(env, env->CallObjectMethod(widget, nativeGetScaleType));

				String scaleModeStr = Utils::EnumValueToString("android/widget/ImageView$ScaleType", scaleModeJava);

				if (scaleModeStr == "CENTER_CROP")
				{
					scaleMode = Image::ScaleMode::AspectFill;
				}
				else if (scaleModeStr == "CENTER_INSIDE")
				{
					scaleMode = Image::ScaleMode::AspectFit;
				}

				return scaleMode;
			}
			
			bool ImageFrame::bindEvent(const String& eventName, const CPP::Event* event)
			{
				JNIEnv* env = Jni::getJniEnv();
				GUI::EventData* eventData = new GUI::EventData(this, eventName, (void*)event);
				LOGI("%s: %s", __FUNCTION__, eventData->_eventName.c_str());

				jmethodID method = nullptr;

				//setOnClickListener - Callback when the view is clicked
				if (eventData->_eventName.compare("click") == 0) {
					method = env->GetMethodID(nativeImageViewClass, "setOnClickListener", "(Landroid/view/View$OnClickListener;)V");
				}
					//onCreateContextMenu - Callback for pressing and holding a view for a long time
				else if (eventData->_eventName.compare("createContextMenu") == 0) {
					method = env->GetMethodID(nativeImageViewClass, "setOnCreateContextMenuListener", "(Landroid/view/View$OnCreateContextMenuListener;)V");
				}
					//setOnDragListener - Callback when the view is dragged
					//setOnFocusChangeListener - Callback when the view changes focus
				else if (eventData->_eventName.compare("focusChange") == 0) {
					method = env->GetMethodID(nativeImageViewClass, "setOnFocusChangeListener", "(Landroid/view/View$OnFocusChangeListener;)V");
				}
					//setOnGenericMotionListener - Callback for arbitrary gestures
					//setOnHoverListener - Callback for hovering over the view
					//setOnKeyListener - Callback for pressing a hardware key when view has focus
				else if (eventData->_eventName.compare("key") == 0) {
					method = env->GetMethodID(nativeImageViewClass, "setOnKeyListener", "(Landroid/view/View$OnKeyListener;)V");
				}
					//setOnLongClickListener - Callback for pressing and holding a view
				else if (eventData->_eventName.compare("longClick") == 0) {
					method = env->GetMethodID(nativeImageViewClass, "setOnLongClickListener", "(Landroid/view/View$OnLongClickListener;)V");
				}
					//setOnTouchListener - Callback for touching down or up on a view
				else if (eventData->_eventName.compare("touch") == 0) {
					method = env->GetMethodID(nativeImageViewClass, "setOnTouchListener", "(Landroid/view/View$OnTouchListener;)V");
				}
				else {
					LOGE("%s: undefined event handled in imageframe.onBindEvent [ %s ]", __FUNCTION__, eventData->_eventName.c_str());
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

