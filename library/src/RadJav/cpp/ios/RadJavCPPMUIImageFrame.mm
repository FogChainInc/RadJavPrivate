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
#include "cpp/RadJavCPPMUIImage.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"

#import "cpp/RadJavCPPMUIView.h"
#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			ImageFrame::ImageFrame(GUI::GObjectWidget *parent, const String &imageFile, const Vector2 &pos, const Vector2 &size)
			: widget([[UIImageView alloc] init]),
			  image(nullptr)
			{
				widget.contentMode = UIViewContentModeScaleAspectFit;

				[parent->getNativeWidget() addSubview:widget];
				
				setSize(size);
				setPosition(pos);
				
				loadImage(imageFile);
			}
			
			ImageFrame::ImageFrame(GUI::GObjectWidget *parent, const Vector2 &pos, const Vector2 &size)
			: widget([[UIImageView alloc] init]),
			  image(nullptr)
			{
				widget.contentMode = UIViewContentModeScaleAspectFit;
				
				[parent->getNativeWidget() addSubview:widget];

				setSize(size);
				setPosition(pos);
			}

			ImageFrame::~ImageFrame()
			{
				[widget release];
				[image release];
			}
			
			RJBOOL ImageFrame::loadImage(const String& imageFile)
			{
				if (image)
				{
					//Do we need to release old one?
					[widget setImage:nil];
					image = nullptr;
				}

				NSString *path = [NSString stringWithUTF8String:imageFile.c_str()];

				/* Temporarily, for testing
				NSString *docDirPath = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES)objectAtIndex:0];
				NSMutableString *path = [NSMutableString stringWithString:docDirPath];
				[path appendString:@"/christmas-tree.png"];
				 */

				image = [UIImage imageWithContentsOfFile:path];
				
				[widget setImage:image];

				return image;
			}
			
			void ImageFrame::setScaleMode(Image::ScaleMode mode)
			{
				switch (mode)
				{
					case Image::ScaleMode::AspectFit:
						widget.contentMode = UIViewContentModeScaleAspectFit;
						[widget setNeedsDisplay];
						break;
					case Image::ScaleMode::AspectFill:
						widget.contentMode = UIViewContentModeScaleAspectFill;
						[widget setNeedsDisplay];
						break;
					default:;
				}
			}

			Image::ScaleMode ImageFrame::getScaleMode() const
			{
				switch (widget.contentMode)
				{
					case UIViewContentModeScaleAspectFill:
						return Image::ScaleMode::AspectFill;
					case UIViewContentModeScaleAspectFit:
					default:
						return Image::ScaleMode::AspectFit;
				}
			}
			
			bool ImageFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
			{
				//TODO: do we need to handle UIImageView events?
				//return [widgetDelegate bindEvent:widget eventName:eventName];
				return false;
			}
			
			#ifdef USE_IOS
				UIView* ImageFrame::getNativeWidget()
				{
					return widget;
				}
			#elif defined USE_ANDROID
				void* ImageFrame::getNativeWidget()
				{
					return widget;
				}
			#endif
		}
	}
}

