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
#ifndef _RADJAV_GUI_CPP_COMBOBOX_H_
	#define _RADJAV_GUI_CPP_COMBOBOX_H_

	#include "RadJavPreprocessor.h"
	#include "RadJavString.h"

	#include "cpp/RadJavCPPGUIGObject.h"

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
					class RADJAV_EXPORT ComboboxFrame : public wxComboBox, public GObjectBase, public ChainedPtr
					{
					public:
						ComboboxFrame(wxWindow *parent, const wxString &text, const wxPoint &pos, const wxSize &size);

						void onChanged(wxCommandEvent &event);
						void onDropdown(wxCommandEvent &event);
						void onCloseup(wxCommandEvent &event);
						void onText(wxCommandEvent &event);

					protected:
						wxDECLARE_EVENT_TABLE();
					};
				#endif

				class RADJAV_EXPORT Combobox : public CPP::GUI::GObject
				{
					public:
						#ifdef USE_V8
							Combobox(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args);
						#elif defined USE_JAVASCRIPTCORE
							Combobox(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[]);
						#endif
					
						Combobox(String name, String text = "", CPP::GUI::GObject *parent = NULL);
						~Combobox();

						void create();
						void setText(String text);
						String getText();

						#if defined USE_V8 || defined USE_JAVASCRIPTCORE
							void on(String event, RJ_FUNC_TYPE func);
						#endif

						class RADJAV_EXPORT Item
						{
							public:
								inline Item()
								{
									this->name = "";
									this->text = "";
								}

								inline Item(String text)
								{
									this->name = "";
									this->text = text;
								}

								#ifdef USE_V8
									static v8::Local<v8::Object> toV8Object(V8JavascriptEngine *jsEngine, Item obj);
									static v8::Local<v8::Array> toV8Array(V8JavascriptEngine *jsEngine, Array<Item> *objs);
								#elif defined USE_JAVASCRIPTCORE
									static JSObjectRef toJSCObject(JSCJavascriptEngine *jsEngine, JSContextRef ctx, Item obj);
									static JSObjectRef toJSCArray(JSCJavascriptEngine *jsEngine, JSContextRef ctx, Array<Item> *objs);
								#endif

								String name;
								String text;
						};

						void addItem(String item);
						void addItem(Item item);
						void setItems(Array<Item> *items);
						void deleteItem(RJINT index);

						Item getItem(RJINT index);
						Array<Item> *getItems();
						RJUINT getNumItems();

						void clear();
						void setSelectedItemIndex(RJINT index);
						RJINT getSelectedItemIndex();

						Array<Item> *_items;
				};
			}
		}
	}
#endif

