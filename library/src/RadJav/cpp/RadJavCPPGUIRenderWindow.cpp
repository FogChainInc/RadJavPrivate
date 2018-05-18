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
#include "cpp/RadJavCPPGUIRenderWindow.h"

#if defined C3D_USE_OGRE && defined GUI_USE_WXWIDGETS

#ifdef __WXGTK__
	#include <gdk/gdk.h>
	#include <gtk/gtk.h>
	#include <gdk/gdkx.h>
	#include <wx/gtk/win_gtk.h>
	#include <GL/glx.h>
#endif


namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			RenderWindow::RenderWindow()
			: renderWindow(nullptr)
			{}
			
			RenderWindow::~RenderWindow()
			{
				if(renderWindow)
				{
					Ogre::Root* root = Ogre::Root::getSingletonPtr();
					if(root)
					{
						try {
							root->detachRenderTarget(renderWindow);
							root->destroyRenderTarget(renderWindow);
						}
						catch(...){}
					}
				}
			}
			
			bool RenderWindow::createRenderWindow( wxWindow* window, const wxSize& size, bool fullscreen)
			{
				Ogre::Root* root = Ogre::Root::getSingletonPtr();
				if(renderWindow || !root)
					return false;
				
				Ogre::String handle = getWindowHandle(window);

				Ogre::NameValuePairList params;
				params["externalWindowHandle"] = handle;
				
				// Need to tell Ogre3D that we are using a cocoa window
				// if wx is using it.
				// and that wxWidgets uses an NSView* as the handle
				#ifdef __WXOSX_COCOA__
					params["macAPI"] = "cocoa";
					params["macAPICocoaUseNSView"] = "true";
				
					//Determine scaling of the window (retina support)
					Ogre::StringStream scaleFactor;
					scaleFactor << Ogre::Math::Clamp( window->GetContentScaleFactor(), 1.0, 2.0);
				
					params["contentScalingFactor"] = scaleFactor.str();
				#endif
				
				Ogre::StringStream str;
				str << "wxWindow" << window->GetId();

				// Create the render window
				renderWindow = root->createRenderWindow( str.str(), size.GetWidth(), size.GetHeight(), fullscreen, &params);
				
				//renderWindow->setActive (true);
				return true;
			}
			
			Ogre::String RenderWindow::getWindowHandle(wxWindow* window) const
			{
				Ogre::String handle;
				WXWidget win = window->GetHandle();
				
				#ifdef __WXMSW__
					// Handle for Windows systems
					HWND hwnd = (HWND)win;
					handle = Ogre::StringConverter::toString((size_t)hwnd);
				#elif defined(__WXOSX_COCOA__)
					handle = Ogre::StringConverter::toString((size_t)((NSView*)(win)));
				#elif defined(__WXGTK__)
					// Handle for GTK-based systems
				
					GtkWidget *widget = win;
					gtk_widget_set_double_buffered (widget, FALSE);
					gtk_widget_realize( widget );
				
					// Grab the window object
					GdkWindow *gdkWin = GTK_PIZZA (widget)->bin_window;
					Display* display = GDK_WINDOW_XDISPLAY(gdkWin);
					Window wid = GDK_WINDOW_XWINDOW(gdkWin);
				
					std::stringstream str;
				
					// Display
					str << (unsigned long)display << ':';
				
					// Screen (returns "display.screen")
					std::string screenStr = DisplayString(display);
					std::string::size_type dotPos = screenStr.find(".");
					screenStr = screenStr.substr(dotPos+1, screenStr.size());
					str << screenStr << ':';
				
					// XID
					str << wid << ':';
				
					// Retrieve XVisualInfo
					int attrlist[] = { GLX_RGBA, GLX_DOUBLEBUFFER, GLX_DEPTH_SIZE, 16, GLX_STENCIL_SIZE, 8, None };
					XVisualInfo* vi = glXChooseVisual(display, DefaultScreen(display), attrlist);
					str << (unsigned long)vi;
				
					handle = str.str();
				#else
					// Any other unsupported system
					#error "No support for RenderWindow on this platform"
				#endif
				
				return handle;
			}

			Ogre::RenderWindow* RenderWindow::getRenderWindow() const
			{
				return renderWindow;
			}
		}
	}
}

#endif //C3D_USE_OGRE and GUI_USE_WXWIDGETS
