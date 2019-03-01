/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#include "cpp/desktop/RadJavCPPGUIImageFrame.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				ImageFrame::ImageFrame(GObjectWidget *parent, const String &filePath, const Vector2 &pos, const Vector2 &size)
					: wxStaticBitmap(parent ? parent->getNativeWidget() : nullptr, wxID_ANY, wxBitmap(), wxPoint(pos.x, pos.y), wxSize(size.x, size.y), wxTRANSPARENT_WINDOW | wxBORDER_NONE)
				{
					isImageLoaded = false;
					scaleMode = Image::ScaleMode::AspectFit;
					
					loadImage(filePath);
				}

				wxBitmapType ImageFrame::getImageType(wxString file)
				{
					String temp = parsewxString(file);
					String ext = "";
					wxBitmapType type;

					temp = temp.toLowerCase();
					size_t posext = temp.rfind(".bmp");

					if (posext != String::npos)
						type = wxBitmapType::wxBITMAP_TYPE_BMP;

					posext = temp.rfind(".jpg");

					if (posext != String::npos)
						type = wxBitmapType::wxBITMAP_TYPE_JPEG;

					posext = temp.rfind(".jpeg");

					if (posext != String::npos)
						type = wxBitmapType::wxBITMAP_TYPE_JPEG;

					posext = temp.rfind(".png");

					if (posext != String::npos)
						type = wxBitmapType::wxBITMAP_TYPE_PNG;

					posext = temp.rfind(".gif");

					if (posext != String::npos)
						type = wxBitmapType::wxBITMAP_TYPE_GIF;

					posext = temp.rfind(".tga");

					if (posext != String::npos)
						type = wxBitmapType::wxBITMAP_TYPE_TGA;

					posext = temp.rfind(".tiff");

					if (posext != String::npos)
						type = wxBitmapType::wxBITMAP_TYPE_TIFF;

					posext = temp.rfind(".pcx");

					if (posext != String::npos)
						type = wxBitmapType::wxBITMAP_TYPE_PCX;

					return (type);
				}

				RJBOOL ImageFrame::loadImage(const String& imageFile)
				{
					if (imageFile == "")
						return false;
					
					wxString file = imageFile.towxString();
					
					wxBitmapType type = getImageType(file);
					if (!image.LoadFile(file, type))
						return false;
					
					isImageLoaded = true;
					
					adaptImage();
					
					return true;
				}
			
				void ImageFrame::setScaleMode(Image::ScaleMode mode)
				{
					scaleMode = mode;
					adaptImage();
				}
			
				Image::ScaleMode ImageFrame::getScaleMode() const
				{
					return scaleMode;
				}

				void ImageFrame::setSize(RJINT width, RJINT height)
				{
					GObjectWidget::setSize(width, height);
					
					adaptImage();
				}

				wxWindow* ImageFrame::getNativeWidget()
				{
					return this;
				}

				void ImageFrame::adaptImage()
				{
					if (!isImageLoaded)
						return;
					
					int controlWidth;
					int controlHeight;
					GetSize(&controlWidth, &controlHeight);
					
					int imageWidth = image.GetWidth();
					int imageHeight = image.GetHeight();

					switch (scaleMode)
					{
						case Image::ScaleMode::AspectFit:
						{
							RDECIMAL xScale = controlWidth/imageWidth;
							RDECIMAL yScale = controlHeight/imageHeight;
							RDECIMAL scale = std::min(xScale, yScale);
							imageSize = wxSize(imageWidth*scale, imageHeight*scale);
							imagePosition = wxPoint((controlWidth-imageWidth)/2.0, (controlHeight-imageHeight)/2.0);
							image.Rescale(imageSize.GetWidth(), imageSize.GetHeight(), wxImageResizeQuality::wxIMAGE_QUALITY_HIGH);
							break;
						}
						case Image::ScaleMode::AspectFill:
						{
							imageSize = wxSize(controlWidth, controlHeight);
							imagePosition = wxPoint(0, 0);
							image.Rescale(controlWidth, controlHeight, wxImageResizeQuality::wxIMAGE_QUALITY_HIGH);
							break;
						}
						default:;
					}
				}

				void ImageFrame::paintEvent(wxPaintEvent &evt)
				{
					if (isImageLoaded == false)
						return;

					wxPaintDC dc(this);
					dc.DrawBitmap(image, imagePosition);
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::ImageFrame, wxStaticBitmap)
		EVT_PAINT(RadJAV::CPP::GUI::ImageFrame::paintEvent)
	wxEND_EVENT_TABLE()
#endif
