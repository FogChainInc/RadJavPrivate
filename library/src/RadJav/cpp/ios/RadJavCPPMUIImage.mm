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
			ImageFrame::ImageFrame(GUI::GObject *parent, const String &imageFile, const Vector2 &pos, const Vector2 &size)
			: widget([[UIImageView alloc] init]),
			  image(nullptr)
			{
				widget.contentMode = UIViewContentModeScaleAspectFit;

				[parent->_appObj->getNativeWidget() addSubview:widget];
				
				setSize(size);
				setPosition(pos);
				
				loadImage(imageFile);
			}
			
			ImageFrame::ImageFrame(GUI::GObject *parent, const Vector2 &pos, const Vector2 &size)
			: widget([[UIImageView alloc] init]),
			  image(nullptr)
			{
				widget.contentMode = UIViewContentModeScaleAspectFit;
				
				[parent->_appObj->getNativeWidget() addSubview:widget];

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
			
			void ImageFrame::setSize(RJINT width, RJINT height)
			{
				widget.frame = CGRectMake(widget.frame.origin.x, widget.frame.origin.y, width, height);
				[widget setNeedsDisplay];
			}
			void ImageFrame::setPosition(RJINT x, RJINT y)
			{
				widget.frame = CGRectMake(x, y, widget.frame.size.width, widget.frame.size.height);
				[widget setNeedsDisplay];
			}
			
			void ImageFrame::setText(String text)
			{
				//No need
			}
			
			String ImageFrame::getText()
			{
				return String();
			}
			
			void ImageFrame::setVisibility(RJBOOL visible)
			{
				[widget setHidden:!visible];
			}
			
			RJBOOL ImageFrame::getVisibility()
			{
				return !widget.isHidden;
			}
			
			void ImageFrame::addChild(GUI::GObject *child)
			{
				UIView* childObj = child->_appObj->getNativeWidget();
				[widget addSubview:childObj];
			}
			
			void ImageFrame::setFont(CPP::Font *font)
			{
				//TODO: Add implementation
				//[widget setFont:(UIFont * _Nullable)];
			}
			
			CPP::Font* ImageFrame::getFont()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void ImageFrame::setPosition(CPP::Vector2 pos)
			{
				setPosition(pos.x, pos.y);
			}
			
			CPP::Vector2 ImageFrame::getPosition()
			{
				return Vector2(getX(), getY());
			}
			
			RJINT ImageFrame::getX()
			{
				return widget.frame.origin.x;
			}
			
			RJINT ImageFrame::getY()
			{
				return widget.frame.origin.y;
			}
			
			void ImageFrame::setSize(CPP::Vector2 size)
			{
				setSize(size.x, size.y);
			}
			
			CPP::Vector2 ImageFrame::getSize()
			{
				return Vector2(getWidth(), getHeight());
			}
			
			RJINT ImageFrame::getWidth()
			{
				return widget.frame.size.width;
			}
			
			RJINT ImageFrame::getHeight()
			{
				return widget.frame.size.height;
			}
			
			GUI::GObject* ImageFrame::getParent()
			{
				//TODO: Add implementation
				return nullptr;
			}
			
			void ImageFrame::setEnabled(RJBOOL enabled)
			{
				//UIImageView seems does not have such functionality
			}
			
			RJBOOL ImageFrame::getEnabled()
			{
				//UIImageView seems does not have such functionality
				return true;
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

