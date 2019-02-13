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
#ifndef _RADJAV_GUI_CPP_IMAGE_FRAME_H_
#define _RADJAV_GUI_CPP_IMAGE_FRAME_H_

#include "RadJavPreprocessor.h"
#include "RadJavString.h"

#include "cpp/RadJavCPPGUIGObject.h"
#include "cpp/RadJavCPPGUIImage.h"
#include "cpp/RadJavCPPEvent.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			class RADJAV_EXPORT ImageFrame : public GObjectWidget
											,public ChainedPtr
			{
			public:
				/**
				 * Constructor.
				 * @param GObjectWidget parent. Constructed object will be added to view hierarchy of parent.
				 * @param String imageFile. Initial state
				 * @param Vector2 pos Initial position.
				 * @param Vector2 size Initial size.
				 */
				ImageFrame(GObjectWidget *parent, const String &imageFile, const Vector2 &pos, const Vector2 &size);
				/**
				 * Constructor.
				 * @param GObjectWidget parent. Constructed object will be added to view hierarchy of parent.
				 * @param Vector2 pos Initial position.
				 * @param Vector2 size Initial size.
				 */
				ImageFrame(GObjectWidget *parent, const Vector2 &pos, const Vector2 &size);
				~ImageFrame();
				
				/** @method loadImage
				 * Loads image from supplied source.
				 * @param imageFile Source to load from.
				 * @return BOOL whether image was loaded.
				 */
				RJBOOL loadImage(const String& imageFile);
				/** @method setScaleMode
				 * Setter for scale mode
				 * @param ScaleMode Only aspect fit and aspect fill are supported now.
				 */
				void setScaleMode(Image::ScaleMode mode);
				/** @method getScaleMode
				 * Getter for scale mode
				 * @return ScaleMode Current scale mode.
				 */
				Image::ScaleMode getScaleMode() const;
				
				bool bindEvent(const String& eventName, const CPP::Event* event);
				
			private:
				static jclass nativeImageViewClass;

				static jmethodID nativeConstructor;
				static jmethodID nativeSetImageBitmap;
				static jmethodID nativeSetScaleType;
				static jmethodID nativeGetScaleType;
			};
		}
	}
}
#endif
