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
#ifndef _RADJAV_GUI_CPP_CANVAS3D_FRAME_H_
	#define _RADJAV_GUI_CPP_CANVAS3D_FRAME_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "cpp/RadJavCPPGUIGObject.h"
	#include "cpp/desktop/RadJavCPPGUIRenderWindow.h"
	#include "cpp/RadJavCPPColor.h"

	#ifdef GUI_USE_WXWIDGETS
		#include <wx/wx.h>
		#include "wxOgreRenderWindow.h"
	#endif

	namespace RadJAV
	{
		namespace CPP
		{
			namespace C3D
			{
				class Transform;
			}
			
			namespace GUI
			{
				#ifdef C3D_USE_OGRE
					class RADJAV_EXPORT CanvasResource
					{
						public:
							inline CanvasResource()
							{
								resourceDir = "";
								name = "";
								type = "";
								group = "";
								loadAtStart = true;
							}

							String resourceDir;
							String name;
							String type;
							String group;
							RJBOOL loadAtStart;
					};

					#ifdef GUI_USE_WXWIDGETS
						/// The wxWidgets frame to use.
						class RADJAV_EXPORT Canvas3DFrame : public wxFrame, public GObjectWidget, public RenderWindow, public ChainedPtr
						{
							public:
								Canvas3DFrame(const wxString &text, const wxPoint &pos, const wxSize &size, Array<CanvasResource *> resources);

								void addChild(const C3D::Transform& child);
								void removeChild(const C3D::Transform& child);

								void onClose(wxCloseEvent &evt);
								void onJSClose(wxCloseEvent &evt);
								void onJSMinimized(wxIconizeEvent &evt);
								void onClick(wxMouseEvent &evt);

							public:
								wxOgreRenderWindow *renderingWidget;
								Ogre::SceneManager* sceneManager;
								wxDECLARE_EVENT_TABLE();
						};
					#endif
				#endif
			}
		}
	}
#endif
