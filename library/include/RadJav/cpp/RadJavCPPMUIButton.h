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
#ifndef _RADJAV_MUI_CPP_BUTTON_H_
	#define _RADJAV_MUI_CPP_BUTTON_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "cpp/RadJavCPPGUIGObject.h"

	#ifdef USE_IOS
		#ifdef __OBJC__
			#define OBJC_CLASS(name) @class name
		#else
			#define OBJC_CLASS(name) typedef struct objc_object name
		#endif

		OBJC_CLASS(UIButton);
        OBJC_CLASS(ButtonDelegate);

	#elif defined USE_ANDROID
		#warning Add Button implementation for Android platform
	#endif

	namespace RadJAV
	{
		namespace CPP
		{
			namespace MUI
			{
				class RADJAV_EXPORT ButtonFrame : public GUI::GObjectInterface
												, public GUI::GObjectEvents
												, public ChainedPtr
				{
				public:
					//TODO: Add correct parent type here, usually some base C++ container class (which still not created)
					ButtonFrame(void *parent, const String &text, const Vector2 &pos, const Vector2 &size);
					~ButtonFrame();

                    void setSize(RJINT x, RJINT y);
                    void setPosition(RJINT x, RJINT y);
                    void setText(String text);
                    String getText();
                    void setVisibility(RJBOOL visible);
                    RJBOOL getVisibility();
                    
					//TODO: Add more specific methods for Button here
					//Other common methods needs to be added to some base interface C++ class
					

					
					void addChild(GUI::GObject *child);
					void setFont(CPP::Font *font);
					CPP::Font *getFont();
					void setPosition(CPP::Vector2 pos);
					CPP::Vector2 getPosition();
					RJINT getX();
					RJINT getY();
					void setSize(CPP::Vector2 size);
					CPP::Vector2 getSize();
					RJINT getWidth();
					RJINT getHeight();

					GUI::GObject *getParent();

					void setEnabled(RJBOOL enabled);
					RJBOOL getEnabled();

                    
#ifdef USE_IOS
                    UIButton* widget;
                    ButtonDelegate* widgetDelegate;
                    void callBack();
#elif defined USE_ANDROID
                    //TODO: Wrap Android specific type here
#endif
                    
				private:
				};
				
				class RADJAV_EXPORT Button : public CPP::GUI::GObject
				{
					public:
						#ifdef USE_V8
							Button(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
                        #ifdef USE_JAVASCRIPTCORE
                            Button(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                        #endif
						Button(String name, String text = "", CPP::GUI::GObject *parent = NULL);

						void create();
						void setPosition(RJINT x, RJINT y);
						CPP::Vector2 getPosition();
						void setSize(RJINT x, RJINT y);
						CPP::Vector2 getSize();
						void setText(String text);
						String getText();
						void setVisibility(RJBOOL visible);
						RJBOOL getVisibility();
						void setEnabled(RJBOOL enabled);
						RJBOOL getEnabled();

						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
                        	/// Execute when an event is triggered.
                        	void on(String event, RJ_FUNC_TYPE func);
						#endif
                        ButtonFrame* _appObject;
						String icon;
				};
			}
		}
	}
#endif

