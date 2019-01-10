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
#ifndef _RADJAV_MUI_CPP_TableCellModel_H_
#define _RADJAV_MUI_CPP_TableCellModel_H_

#include "cpp/RadJavCPPGUIGObject.h"

#ifdef USE_V8
	#include "v8/RadJavV8GUIGObject.h"
#elif defined USE_JAVASCRIPTCORE
	#include "jscore/RadJavJSCGUIGObject.h"
#endif

#ifdef USE_IOS
	OBJC_CLASS(UITableView);
    OBJC_CLASS(TableViewDelegate);
#elif defined USE_ANDROID
	JNI_CLASS(jobject);
#endif


	namespace RadJAV
	{
		namespace CPP
		{
			namespace MUI
			{
                class RADJAV_EXPORT TableCellModelFrame : public GUI::GObjectEvents
                {
                public:
                    TableCellModelFrame();
                    ~TableCellModelFrame();
					
                    bool bindEvent(const String& eventName,const GUI::Event* evt);

                    #ifdef USE_IOS
                        TableViewDelegate* widgetDelegate;
                    #endif

                    void addNewEvent(String event,
#ifdef USE_V8
                                                          v8::Local<v8::Function> func
#elif defined USE_JAVASCRIPTCORE
                                                          JSObjectRef func
#endif
                    );
                };
                
                class RADJAV_EXPORT TableCellModel : public CPP::ChainedPtr
				{
					public:
						#ifdef USE_V8
							TableCellModel(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#endif
                        #ifdef USE_JAVASCRIPTCORE
                            TableCellModel(JSCJavascriptEngine *jsEngine, JSObjectRef thisObj, size_t numArgs, const JSValueRef args[]);
                        #endif
						TableCellModel(String name, String text = "", CPP::GUI::GObject *parent = NULL);
                    	JSObjectRef thisJS;
                        String name;
                        String subtitle;
                        bool getUsesAccessoryButton();
                        void setUsesAccessoryButton(bool value);
                        bool getUsesCheckmark();
                        void setUsesCheckmark(bool value);
                        bool getIsSelected();
                        void setIsSelected(bool value);
                        bool getIsDeletable();
                        void setIsDeletable(bool value);
                        bool getIsHeader();
                        void setIsHeader(bool value);
                        bool getIsFooter();
                        void setIsFooter(bool value);

						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
                        	/// Execute when an event is triggered.
                        	void on(String event, RJ_FUNC_TYPE func);
						#endif

						#ifdef USE_IOS
                            TableCellModelFrame *nativeImplementation;
						#elif defined USE_ANDROID
                            //TODO:add Android native implementation?
							//TableCellModelFrame *nativeImplementation;
						#endif

					private:
						bool usesAccessoryButton;
						bool usesCheckmark;
						bool isSelected;
						bool isDeletable;
						bool isHeader;
						bool isFooter;
				};
			}
		}
	}
#endif

