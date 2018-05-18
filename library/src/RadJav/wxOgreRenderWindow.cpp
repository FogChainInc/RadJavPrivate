#include "wxOgreRenderWindow.h"

#if defined C3D_USE_OGRE && defined GUI_USE_WXWIDGETS

const wxWindowID ID_RENDERTIMER = wxNewId ();
 
wxIMPLEMENT_CLASS (wxOgreRenderWindow, wxControl);
 
BEGIN_EVENT_TABLE (wxOgreRenderWindow, wxControl)
#ifndef __WXMSW__
    EVT_PAINT (wxOgreRenderWindow::OnPaint)
#endif
    EVT_TIMER (ID_RENDERTIMER, wxOgreRenderWindow::OnRenderTimer)
    EVT_SIZE (wxOgreRenderWindow::OnSize)
    EVT_MOUSE_EVENTS (wxOgreRenderWindow::OnMouseEvents)
END_EVENT_TABLE ()
 
Ogre::Root *wxOgreRenderWindow::msOgreRoot = 0;

wxOgreRenderWindow::wxOgreRenderWindow (Ogre::Root *root, wxWindow *parent, wxWindowID id,
                const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator) {
	msOgreRoot = root;
    Init ();

	SetBackgroundStyle(wxBG_STYLE_PAINT);
	
    Create (parent, id, pos, size, style, validator);
}

//------------------------------------------------------------------------------
wxOgreRenderWindow::wxOgreRenderWindow () {
    Init ();
}

//------------------------------------------------------------------------------
bool wxOgreRenderWindow::Create (wxWindow *parent, wxWindowID id,
                const wxPoint &pos, const wxSize &size, long style, const wxValidator &validator) {
    // Error creating the base class
    if (!wxControl::Create (parent, id, pos, size, style, validator))
        return false;
 
    // Use the told size or let the sizers pick one.
    SetInitialSize (size);
 
    if (msOgreRoot)
	{
        if(!createRenderWindow( this, size, false))
			return false;
		
		renderWindow->setActive(true);
	}
 
    return true;
}
//------------------------------------------------------------------------------
wxOgreRenderWindow::~wxOgreRenderWindow () {
    if (mRenderTimer)
        delete mRenderTimer;
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::Init () {
    // Callbacks
    mMouseEventsCallback = 0;
 
    mRenderTimer = new wxTimer (this, ID_RENDERTIMER);
    mRenderTimer->Start (10);
}
//------------------------------------------------------------------------------
inline wxSize wxOgreRenderWindow::DoGetBestSize () const {
    return wxSize (320, 240);
}
//------------------------------------------------------------------------------
Ogre::Root *wxOgreRenderWindow::GetOgreRoot () {
    return msOgreRoot;
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::SetOgreRoot (Ogre::Root *root) {
    msOgreRoot = root;
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::SetRenderTimerPeriod (int period) {
    if (!mRenderTimer)
        return;
 
    if (period <= 0)
        mRenderTimer->Stop ();
    else
        mRenderTimer->Start (period);
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::Update () {
    if (msOgreRoot)
        msOgreRoot->renderOneFrame ();
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::SetMouseEventsCallback (MouseEventsCallback callback) {
    mMouseEventsCallback = callback;
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::OnPaint (wxPaintEvent &evt) {
    Update ();
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::OnRenderTimer (wxTimerEvent &evt) {
	Refresh();
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::OnSize (wxSizeEvent &evt)
{
    if (renderWindow)
    {
        // Setting new size;
        int width;
        int height;
        wxSize size = evt.GetSize ();
        width = size.GetWidth ();
        height = size.GetHeight ();
 
        // Cocoa will break the window here if the panel docks using an Advanced UI Manager
        // because wxWidgets actually destroys the floating panel.
        // wxWidgets handles the width and height of the window anyway
        // and Ogre recognises that in windowMovedOrResized().
        // I am not sure if windows requires this, but it works fine with it.
//#if defined(__WXGTK__) || defined(__WXMSW__)
        renderWindow->resize (width, height);
//#endif
 
        // Letting Ogre know the window has been resized;
        renderWindow->windowMovedOrResized ();
    }
 
    Update ();
}
//------------------------------------------------------------------------------
void wxOgreRenderWindow::OnMouseEvents (wxMouseEvent &evt)
{
    if (mMouseEventsCallback)
        (*mMouseEventsCallback)(evt);
}

#endif //C3D_USE_OGRE && GUI_USE_WXWIDGETS
