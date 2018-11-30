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
#include "cpp/RadJavCPPMUIImage.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			jclass ImageFrame::nativeImageViewClass = nullptr;

			jmethodID ImageFrame::nativeConstructor = nullptr;
			jmethodID ImageFrame::nativeSetImageBitmap = nullptr;
			jmethodID ImageFrame::nativeSetScaleType = nullptr;
			jmethodID ImageFrame::nativeGetScaleType = nullptr;

			ImageFrame::ImageFrame(GUI::GObjectWidget *parent, const String &imageFile, const Vector2 &pos, const Vector2 &size)
			{
				if (!nativeImageViewClass)
				{
					Jni& jni = Jni::instance();
					JNIEnv* env = jni.getJniEnv();

					nativeImageViewClass = jni.findClass("android/widget/ImageView");

					nativeConstructor = env->GetMethodID(nativeImageViewClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetImageBitmap = env->GetMethodID(nativeImageViewClass, "setImageBitmap", "(Landroid/graphics/Bitmap;)V");
					nativeSetScaleType = env->GetMethodID(nativeImageViewClass, "setScaleType", "(Landroid/widget/ImageView/ScaleType;)V");
					nativeGetScaleType = env->GetMethodID(nativeImageViewClass, "getScaleType", "()Landroid/widget/ImageView/ScaleType;");
				}

				RadJav::runOnUiThreadAsync([&, parent](JNIEnv* env, void* data) {
					auto layout = wrap_local(env, env->NewObject(nativeImageViewClass, nativeConstructor, RadJav::getJavaApplication()));

					widget = env->NewGlobalRef(layout);
				});

				if (parent)
					parent->addChild(this);

				setSize(size);
				setPosition(pos);

				if (imageFile.size())
					loadImage(imageFile);
			}
			
			ImageFrame::ImageFrame(GUI::GObjectWidget *parent, const Vector2 &pos, const Vector2 &size)
			{
				if (!nativeImageViewClass)
				{
					Jni& jni = Jni::instance();
					JNIEnv* env = jni.getJniEnv();

					nativeImageViewClass = jni.findClass("android/widget/ImageView");

					nativeConstructor = env->GetMethodID(nativeImageViewClass, "<init>", "(Landroid/content/Context;)V");
					nativeSetImageBitmap = env->GetMethodID(nativeImageViewClass, "setImageBitmap", "(Landroid/graphics/Bitmap;)V");
					nativeSetScaleType = env->GetMethodID(nativeImageViewClass, "setScaleType", "(Landroid/widget/ImageView$ScaleType;)V");
					nativeGetScaleType = env->GetMethodID(nativeImageViewClass, "getScaleType", "()Landroid/widget/ImageView$ScaleType;");
				}

				RadJav::runOnUiThreadAsync([&, parent](JNIEnv* env, void* data) {
					auto layout = wrap_local(env, env->NewObject(nativeImageViewClass, nativeConstructor, RadJav::getJavaApplication()));

					widget = env->NewGlobalRef(layout);
				});

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
				JNIEnv* env = jni.getJniEnv();

				jclass bitmapFactoryClass = jni.findClass("android/graphics/BitmapFactory");

				jmethodID decodeFileMethod = env->GetStaticMethodID(bitmapFactoryClass, "decodeFile", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");

				auto imagePathJava = wrap_local(env, env->NewStringUTF(imageFile.c_str()));

				jobject bitmap = env->CallStaticObjectMethod(bitmapFactoryClass, decodeFileMethod, imagePathJava.get());

				if (AndroidUtils::IsNull(bitmap))
					return false;

				bitmap = env->NewGlobalRef(bitmap);

				RadJav::runOnUiThreadAsync([&, bitmap](JNIEnv* env, void* data) {
					env->CallVoidMethod(widget, nativeSetImageBitmap, bitmap);
					env->DeleteGlobalRef(bitmap);
				});

				return true;
			}
			
			void ImageFrame::setScaleMode(Image::ScaleMode mode)
			{
				RadJav::runOnUiThreadAsync([&, mode](JNIEnv* env, void*) {
					Jni& jni = Jni::instance();

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
				});
			}

			Image::ScaleMode ImageFrame::getScaleMode() const
			{
				Image::ScaleMode scaleMode = Image::ScaleMode::AspectFit;

				RadJav::runOnUiThread([&](JNIEnv* env, void* data) {
					auto scaleModeJava = wrap_local(env, env->CallObjectMethod(widget, nativeGetScaleType));

					String scaleModeStr = AndroidUtils::EnumValueToString("android/widget/ImageView$ScaleType", scaleModeJava);

					if (scaleModeStr == "CENTER_CROP")
					{
						scaleMode = Image::ScaleMode::AspectFill;
					}
					else if (scaleModeStr == "CENTER_INSIDE")
					{
						scaleMode = Image::ScaleMode::AspectFit;
					}
				});

				return scaleMode;
			}
			
			bool ImageFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				//TODO: do we need to handle UIImageView events?
				//return [widgetDelegate bindEvent:widget eventName:eventName];
				return false;
			}
		}
	}
}

