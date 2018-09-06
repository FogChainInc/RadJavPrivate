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
#ifndef _RADJAV_GUI_CPP_GOBJECT_H_
#define _RADJAV_GUI_CPP_GOBJECT_H_

#include "RadJavPreprocessor.h"

#include "RadJavString.h"
#include "RadJavHashMap.h"

#include "cpp/RadJavCPPRectangle.h"
#include "cpp/RadJavCPPFont.h"
#include "cpp/RadJavCPPVector2.h"
#include "cpp/RadJavCPPGUIEvent.h"
#include "cpp/RadJavCPPChainedPtr.h"

#include "RadJavXML.h"

#ifdef GUI_USE_WXWIDGETS
	#include <wx/wx.h>
#endif

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#endif

#ifdef USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCJavascriptEngine.h"
#endif

namespace RadJAV
{
	namespace CPP
	{
		/// Contains classes for the OS GUI.
		namespace GUI
		{
			class RADJAV_EXPORT GObject : public ChainedPtr
			{
				public:
					#ifdef USE_V8
						GObject(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
					#endif
                    #ifdef USE_JAVASCRIPTCORE
                        GObject(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                    #endif
					GObject(String name, String text = "", GObject *parent = NULL);
					virtual ~GObject();

					inline void show ()
					{
						setVisibility(true);
					}

					inline void hide()
					{
						setVisibility(false);
					}

					/// Using the existing parameters in this object, create it.
					virtual void create();

					/// Using the existing parameters in this object, create it.
					virtual void addChild(GObject *child);

					/** Set this object's font.
					* Theme Event: setFont
					* Is Theme Event Asynchronous: No
					* @param {RadJav.Font} font The font to set.
					*/
					virtual void setFont(CPP::Font *font);

					/** Get this object's font.
					* Theme Event: getFont
					* Is Theme Event Asynchronous: No
					* @return {RadJav.Font} The font.
					*/
					virtual CPP::Font *getFont();

					/** Set the position of this object.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @param {Number/RadJav.Vector2} x The new position, or the new x coordinate of the new position.
					* @param {Number} [y=null] The new y coordinate.
					*/
					virtual void setPosition(RJINT x, RJINT y);
					virtual void setPosition(CPP::Vector2 pos);

					/** @method getPosition
					* Get the position of this object.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @return {RadJav.Vector2} The position of this object.
					*/
					virtual CPP::Vector2 getPosition();

					/** Get the X position of this object.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @return {RadJav.Vector2} The position of this object.
					*/
					virtual RJINT getX();

					/** Get the Y position of this object.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @return {RadJav.Vector2} The position of this object.
					*/
					virtual RJINT getY();

					/** Set the size of this object.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @param {Number/RadJav.Vector2} width The object's new size, or new width.
					* @param {Number} [height=null] The object's new height.
					*/
					virtual void setSize(RJINT width, RJINT height);
					virtual void setSize(CPP::Vector2 size);

					/** Get the size of this object.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @return {RadJav.Vector2} The size of this object.
					*/
					virtual CPP::Vector2 getSize();

					/** Get the width of this object.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @return {Number} The width of this object.
					*/
					virtual RJINT getWidth();

					/** Get the height of this object.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @return {Number} The height of this object.
					*/
					virtual RJINT getHeight();

					/** Set the object's text.
					* Theme Event: setText
					* Is Theme Event Asynchronous: Yes
					* @param {String} text The text to set.
					* @return {String} The text associated with this object.
					*/
					virtual void setText(String text);

					/** Get the object's text.
					* Theme Event: getText
					* Is Theme Event Asynchronous: No
					* @return {String} The text associated with this object.
					*/
					virtual String getText();

					/** Get the parent.
					* Theme Event: None
					* Is Theme Event Asynchronous: No
					* @return {RadJav.GUI.GObject} The parent of this object.
					*/
					virtual GObject *getParent();

					/** Set the visibility of this object.
					* Theme Event: setVisibility
					* Is Theme Event Asynchronous: Yes
					* Parameters Passed to Theme Event: RadJav.GUI.GObject, Boolean
					* @param {Boolean} visible The visibility of the object
					*/
					virtual void setVisibility(RJBOOL visible);

					/** Get the visibility of this object.
					* Theme Event: setVisibility
					* Is Theme Event Asynchronous: No
					* Parameters Passed to Theme Event: RadJav.GUI.GObject
					* @return {Boolean} The visibility of this object
					*/
					virtual RJBOOL getVisibility();

					/** Enable or disable this object.
					* Theme Event: setEnabled
					* Is Theme Event Asynchronous: Yes
					* Parameters Passed to Theme Event: RadJav.GUI.GObject, Boolean
					*/
					virtual void setEnabled(RJBOOL enabled);

					/** Get whether or not this object is enabled.
					* Theme Event: getEnabled
					* Is Theme Event Asynchronous: No
					* Parameters Passed to Theme Event: RadJav.GUI.GObject
					* @return {Boolean} The enabled status of this object
					*/
					virtual RJBOOL getEnabled();

					#ifdef USE_V8
						/// Execute when an event is triggered.
						virtual void on(String event, v8::Local<v8::Function> func) = 0;
					#endif

					virtual void setup();
					void setupCursor();

					/// The name of this object.
					String name;
					/// The type of object.
					String type;
					/// The transform of this object.
					Rectangle *_transform;
					/// The visibility of the object.
					RJBOOL _visible;
					/// The initial z-index of this object. The higher the value the more "on top" the
					/// object will be compared to other objects.
					RJINT _zIndex;
					/// The text associated with this object.
					String _text;
					/// The font associated with this object.
					Font *_font;
					/// The cursor to use.
					String _cursor;
					/// The parent of this object.
					GObject *_parent;
					/// This object's children.
					Array<GObject *> _children;
					/// The native os gui object associated with this object.
					#ifdef GUI_USE_WXWIDGETS
						wxWindow *_appObj;
					#endif
			};

			class RADJAV_EXPORT GObjectBase
			{
				public:
					GObjectBase();
					virtual ~GObjectBase();

					#ifdef GUI_USE_WXWIDGETS
                        #ifdef USE_V8
                            Event* createEvent(String event, v8::Local<v8::Function> function);
                            void addNewEvent(String event, wxWindow *object, v8::Local<v8::Function> func);
                        #endif

                        #ifdef USE_JAVASCRIPTCORE
                            Event* createEvent(String event, JSValueRef function);
                            void addNewEvent(String event, wxWindow *object, JSValueRef func);
                        #endif

						static void onClick(wxMouseEvent &event);
						static void onDoubleClick(wxMouseEvent &event);
						static void onRightClick(wxMouseEvent &event);
						static void onRightDoubleClick(wxMouseEvent &event);
						static void onRightDown(wxMouseEvent &event);
						static void onMiddleClick(wxMouseEvent &event);
						static void onMiddleDoubleClick(wxMouseEvent &event);
						static void onMiddleDown(wxMouseEvent &event);

						static void onMouseAux1Down(wxMouseEvent &event);
						static void onMouseAux1Up(wxMouseEvent &event);
						static void onMouseEnterWindow(wxMouseEvent &event);
						static void onMouseLeaveWindow(wxMouseEvent &event);
						static void onMouseMotion(wxMouseEvent &event);
						static void onMouseWheel(wxMouseEvent &event);
						
						static void onKeyUp(wxKeyEvent &event);
						static void onKeyDown(wxKeyEvent &event);

						static void onFocusSet(wxFocusEvent &event);
						static void onFocusOut(wxFocusEvent &event);

                        #ifdef USE_V8
                            static v8::Local<v8::Value> executeEvent(Event *pevent, RJINT numArgs = 0, v8::Local<v8::Value> *args = NULL);
                        #endif
                
                        #ifdef USE_JAVASCRIPTCORE
                            static JSValueRef executeEvent(Event *pevent, RJINT numArgs = 0, JSValueRef *args = NULL);
                        #endif

						HashMap<std::string, Event* > *events;

					#endif
			};
		}
	}
}
#endif

