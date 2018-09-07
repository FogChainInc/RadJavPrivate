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
#include "cpp/RadJavCPPGUIImage.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				ImageFrame::ImageFrame(wxWindow *parent, const wxString &file, wxSize fileSize, const wxPoint &pos, const wxSize &size)
					: wxPanel(parent, wxID_ANY, pos, size), GObjectBase()
				{
					isImageLoaded = false;
					loadImage(file, fileSize);
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

				void ImageFrame::loadImage(wxString file)
				{
					wxBitmapType type = getImageType(file);
					image.LoadFile(file, type);
				}

				void ImageFrame::loadImage(wxString file, wxSize fileSize)
				{
					wxBitmapType type = getImageType(file);
					RJBOOL hasLoaded = image.LoadFile(file, type);

					if (hasLoaded == false)
					{
						#ifdef USE_V8
							V8_JAVASCRIPT_ENGINE->throwException("Unable to load image " + parsewxString (file));
						#endif

						return;
					}

					isImageLoaded = true;

					//image = image.Scale(fileSize.GetWidth(), fileSize.GetHeight());
					image.Rescale(fileSize.GetWidth(), fileSize.GetHeight(), wxImageResizeQuality::wxIMAGE_QUALITY_HIGH);
					imageSize = fileSize;
				}

				void ImageFrame::paintEvent(wxPaintEvent &evt)
				{
					if (isImageLoaded == false)
						return;

					wxPaintDC dc(this);
					render(dc);
				}

				void ImageFrame::render(wxDC &dc)
				{
					if (isImageLoaded == false)
						return;

					image.Rescale(imageSize.GetWidth(), imageSize.GetHeight(), wxImageResizeQuality::wxIMAGE_QUALITY_HIGH);
					dc.DrawBitmap(image, 0, 0);
				}
			#endif

			#ifdef USE_V8
				Image::Image(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
					_image = V8_JAVASCRIPT_ENGINE->v8GetString(args.This(), "_image");
				}
			#endif

			Image::Image(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
				_image = "";
			}

			void Image::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					ImageFrame *object = RJNEW ImageFrame(parentWin, _image.towxString(), wxSize(_transform->width, _transform->height),
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);

					_appObj = object;
				
					linkWith(object);

					setup();
				#endif
			}

			#ifdef USE_V8
				void Image::on(String event, v8::Local<v8::Function> func)
				{
					#ifdef GUI_USE_WXWIDGETS 
						
						CPP::GUI::ImageFrame *object = (CPP::GUI::ImageFrame *)_appObj;
						object->addNewEvent(event, object, func);
					
					#endif
				}
			#endif

			void Image::setImage(String image)
			{
				#ifdef GUI_USE_WXWIDGETS
					ImageFrame *object = (ImageFrame *)_appObj;

					if (object != NULL) {
						object->loadImage(_image.towxString());
					}
				#endif
			}
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::ImageFrame, wxPanel)
		EVT_PAINT(RadJAV::CPP::GUI::ImageFrame::paintEvent)
	wxEND_EVENT_TABLE()
#endif

