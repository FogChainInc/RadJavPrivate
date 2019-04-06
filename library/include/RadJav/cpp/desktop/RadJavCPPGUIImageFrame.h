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
#ifndef _RADJAV_GUI_CPP_IMAGE_FRAME_H_
	#define _RADJAV_GUI_CPP_IMAGE_FRAME_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "cpp/RadJavCPPGUIGObject.h"
	#include "cpp/RadJavCPPGUIImage.h"

	#ifdef GUI_USE_WXWIDGETS
		#include <wx/wx.h>
	#endif

	namespace RadJAV
	{
		namespace CPP
		{
			namespace GUI
			{
				#ifdef GUI_USE_WXWIDGETS
					/// The wxWidgets button to use.
					class RADJAV_EXPORT ImageFrame : public wxStaticBitmap, public GObjectWidget, public ChainedPtr
					{
					public:
						ImageFrame(Image::ScaleMode scaleMode, GObjectWidget *parent, const String &filePath, const Vector2 &pos, const Vector2 &size);

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
						
						void setSize(RJINT width, RJINT height);
						
						wxWindow* getNativeWidget();

					protected:
						void adaptImage();
						
						wxBitmapType getImageType(wxString file);
						void paintEvent(wxPaintEvent &event);

					protected:
						RJBOOL isImageLoaded;
						wxImage image;
						Image::ScaleMode scaleMode;
						wxPoint imagePosition;
						wxSize imageSize;

						wxDECLARE_EVENT_TABLE();
					};
				#endif
			}
		}
	}
#endif
