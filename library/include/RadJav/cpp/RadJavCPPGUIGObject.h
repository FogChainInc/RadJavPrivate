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
#include "cpp/RadJavCPPChainedPtr.h"
#include "cpp/RadJavCPPEvent.h"

#include "RadJavXML.h"

#ifdef GUI_USE_WXWIDGETS
	#include <wx/wx.h>
#endif

#ifdef USE_V8
	#include "v8/RadJavV8JavascriptEngine.h"
#elif defined USE_JAVASCRIPTCORE
    #include "jscore/RadJavJSCJavascriptEngine.h"
#endif

#if defined (USE_IOS)
	#ifdef __OBJC__
		#define OBJC_CLASS(name) @class name
	#else
		#define OBJC_CLASS(name) typedef struct objc_object name
	#endif

	OBJC_CLASS(UIView);
#elif defined (USE_ANDROID)
	#define JNI_CLASS(name) typedef class _jobject* name

	JNI_CLASS(jobject);
#endif


namespace RadJAV
{
	namespace CPP
	{
		/// Contains classes for the OS GUI.
		namespace GUI
		{
			class GObject;
			class GObjectWidget;
			
			/**
			 * @ingroup group_gui_cpp
			 * @brief Base class for UI controls(common interface)
			 */
			class RADJAV_EXPORT GObjectInterface
			{
			public:
				virtual ~GObjectInterface() {};
				
				/** Set this object's font.
				 * Theme Event: setFont
				 * Is Theme Event Asynchronous: No
				 * @param {RadJav.Font} font The font to set.
				 */
				virtual void setFont(CPP::Font *font) = 0;
				
				/** Get this object's font.
				 * Theme Event: getFont
				 * Is Theme Event Asynchronous: No
				 * @return {RadJav.Font} The font.
				 */
				virtual CPP::Font *getFont() = 0;
				
				/** Set the position of this object.
				 * Theme Event: None
				 * Is Theme Event Asynchronous: No
				 * @param {Number/RadJav.Vector2} x The new position, or the new x coordinate of the new position.
				 * @param {Number} [y=null] The new y coordinate.
				 */
				virtual void setPosition(RJINT x, RJINT y) = 0;
				virtual void setPosition(CPP::Vector2 pos) = 0;
				
				/** @method getPosition
				 * Get the position of this object.
				 * Theme Event: None
				 * Is Theme Event Asynchronous: No
				 * @return {RadJav.Vector2} The position of this object.
				 */
				virtual CPP::Vector2 getPosition() = 0;
				
				/** Get the X position of this object.
				 * Theme Event: None
				 * Is Theme Event Asynchronous: No
				 * @return {RadJav.Vector2} The position of this object.
				 */
				virtual RJINT getX() = 0;
				
				/** Get the Y position of this object.
				 * Theme Event: None
				 * Is Theme Event Asynchronous: No
				 * @return {RadJav.Vector2} The position of this object.
				 */
				virtual RJINT getY() = 0;
				
				/** Set the size of this object.
				 * Theme Event: None
				 * Is Theme Event Asynchronous: No
				 * @param {Number/RadJav.Vector2} width The object's new size, or new width.
				 * @param {Number} [height=null] The object's new height.
				 */
				virtual void setSize(RJINT width, RJINT height) = 0;
				virtual void setSize(CPP::Vector2 size) = 0;
				
				/** Get the size of this object.
				 * Theme Event: None
				 * Is Theme Event Asynchronous: No
				 * @return {RadJav.Vector2} The size of this object.
				 */
				virtual CPP::Vector2 getSize() = 0;
				
				/** Get the width of this object.
				 * Theme Event: None
				 * Is Theme Event Asynchronous: No
				 * @return {Number} The width of this object.
				 */
				virtual RJINT getWidth() = 0;
				
				/** Get the height of this object.
				 * Theme Event: None
				 * Is Theme Event Asynchronous: No
				 * @return {Number} The height of this object.
				 */
				virtual RJINT getHeight() = 0;
				
				/** Set the object's text.
				 * Theme Event: setText
				 * Is Theme Event Asynchronous: Yes
				 * @param {String} text The text to set.
				 * @return {String} The text associated with this object.
				 */
				virtual void setText(String text) = 0;
				
				/** Get the object's text.
				 * Theme Event: getText
				 * Is Theme Event Asynchronous: No
				 * @return {String} The text associated with this object.
				 */
				virtual String getText() = 0;
				
				/** Set the visibility of this object.
				 * Theme Event: setVisibility
				 * Is Theme Event Asynchronous: Yes
				 * Parameters Passed to Theme Event: RadJav.GUI.GObject, Boolean
				 * @param {Boolean} visible The visibility of the object
				 */
				virtual void setVisibility(RJBOOL visible) = 0;
				
				/** Get the visibility of this object.
				 * Theme Event: setVisibility
				 * Is Theme Event Asynchronous: No
				 * Parameters Passed to Theme Event: RadJav.GUI.GObject
				 * @return {Boolean} The visibility of this object
				 */
				virtual RJBOOL getVisibility() = 0;
				
				/** Enable or disable this object.
				 * Theme Event: setEnabled
				 * Is Theme Event Asynchronous: Yes
				 * Parameters Passed to Theme Event: RadJav.GUI.GObject, Boolean
				 */
				virtual void setEnabled(RJBOOL enabled) = 0;
				
				/** Get whether or not this object is enabled.
				 * Theme Event: getEnabled
				 * Is Theme Event Asynchronous: No
				 * Parameters Passed to Theme Event: RadJav.GUI.GObject
				 * @return {Boolean} The enabled status of this object
				 */
				virtual RJBOOL getEnabled() = 0;
			};
			
			/// Base class to glue JS and C++ GUI object
			class RADJAV_EXPORT GObject : public GObjectInterface, public ChainedPtr
			{
				public:
					#ifdef USE_V8
						GObject(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
					#elif defined USE_JAVASCRIPTCORE
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

					virtual void setup();
					void setupCursor();

					void addChild(GObject *child);
					GObject *getParent();

					//From GObjectInterface:
					void setFont(CPP::Font *font);
					CPP::Font *getFont();
					void setPosition(RJINT x, RJINT y);
					void setPosition(CPP::Vector2 pos);
					CPP::Vector2 getPosition();
					RJINT getX();
					RJINT getY();
					void setSize(RJINT width, RJINT height);
					void setSize(CPP::Vector2 size);
					CPP::Vector2 getSize();
					RJINT getWidth();
					RJINT getHeight();
					void setText(String text);
					String getText();
					void setVisibility(RJBOOL visible);
					RJBOOL getVisibility();
					void setEnabled(RJBOOL enabled);
					RJBOOL getEnabled();

					#if defined USE_V8 || defined USE_JAVASCRIPTCORE
						/// Execute when an event is triggered.
						virtual void on(String event, RJ_FUNC_TYPE func) = 0;
					#endif

					/// Call the parent GObject to inform it, one of its children has finished creating.
					void _callChildCreated(GObject *child);

				public:
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
					GObjectWidget* _appObj;
					/// Execute this function when a child is added. If this function
					/// returns false, the child will not be added.
					std::function<void(GUI::GObject *)> onChildCreated;
			};

			/// Base class for events utils/handling
			class RADJAV_EXPORT GObjectEvents: public Events
			{
				public:
					GObjectEvents();
					GObjectEvents(const GObjectEvents& other) = delete;
					GObjectEvents& operator =(const GObjectEvents& other) = delete;
				
					#ifdef GUI_USE_WXWIDGETS
						void addNewEvent(String event, wxWindow *object,
								#ifdef USE_V8
									v8::Local<v8::Function> func
								#endif
								#ifdef USE_JAVASCRIPTCORE
									JSObjectRef func
								#endif
							);
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
					#endif
			};
			
			/// Base class for wrapped native controls using composition method (not used for wxWidgets based controls)
			class RADJAV_EXPORT GObjectWidget : public GObjectInterface
												,public GObjectEvents
			{
			public:
				GObjectWidget();
				virtual ~GObjectWidget() {};
				
				virtual void addChild(GObjectWidget *child);
				virtual GObjectWidget *getParent();

				void setFont(CPP::Font *font);
				CPP::Font *getFont();
				void setPosition(RJINT x, RJINT y);
				void setPosition(CPP::Vector2 pos);
				CPP::Vector2 getPosition();
				RJINT getX();
				RJINT getY();
				void setSize(RJINT width, RJINT height);
				void setSize(CPP::Vector2 size);
				CPP::Vector2 getSize();
				RJINT getWidth();
				RJINT getHeight();
				void setText(String text);
				String getText();
				void setVisibility(RJBOOL visible);
				RJBOOL getVisibility();
				void setEnabled(RJBOOL enabled);
				RJBOOL getEnabled();

				#ifdef GUI_USE_WXWIDGETS
					virtual wxWindow* getNativeWidget() = 0;
				#elif defined USE_IOS
					virtual UIView* getNativeWidget() = 0;
				#elif defined USE_ANDROID
					jobject getNativeWidget();
				#endif

			//TODO: this must be protected but WindowFrame class has custom addChild method which touch this
			//protected:
				GObjectWidget* parent;
				
			#ifdef USE_ANDROID
			protected:
				jobject widget;

			protected:
				//Java methods IDs
				static jmethodID nativeSetLeft;
				static jmethodID nativeSetTop;
				static jmethodID nativeSetRight;
				static jmethodID nativeSetBottom;
				static jmethodID nativeSetVisibility;
				static jmethodID nativeSetEnabled;

				static jmethodID nativeGetLeft;
				static jmethodID nativeGetTop;
				static jmethodID nativeGetRight;
				static jmethodID nativeGetBottom;
				static jmethodID nativeGetWidth;
				static jmethodID nativeGetHeight;
				static jmethodID nativeGetVisibility;
				static jmethodID nativeIsEnabled;

				static jclass nativeViewClass;
			#endif
			};

            struct EventData
            {
                EventData(GObjectWidget* widget, String eventName, void* event)
                {
                    _widget = widget;
                    _event = event;
                    _eventName = eventName;
                }

                GObjectWidget* _widget;
                String _eventName;
                void* _event;
            };

		}
	}
}
#endif
