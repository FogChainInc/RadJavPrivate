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
#include "cpp/RadJavCPPGUICombobox.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			#ifdef GUI_USE_WXWIDGETS
				ComboboxFrame::ComboboxFrame(wxWindow *parent, const wxString &text, const wxPoint &pos, const wxSize &size)
				        : wxComboBox(parent, wxID_ANY, text, pos, size), GObjectBase()
				{
				}

				void ComboboxFrame::onChanged(wxCommandEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void ComboboxFrame::onDropdown(wxCommandEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void ComboboxFrame::onCloseup(wxCommandEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
				void ComboboxFrame::onText(wxCommandEvent &event)
				{
					Event *pevent = (Event *)event.GetEventUserData();
					executeEvent(pevent);
				}
			#endif

			#ifdef USE_V8
				Combobox::Combobox(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject (jsEngine, args)
				{
					_items = RJNEW Array<Item>();
				}
			#elif defined USE_JAVASCRIPTCORE
				Combobox::Combobox(JSCJavascriptEngine *jsEngine, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[])
					: GObject (jsEngine, thisObject, argumentCount, arguments)
				{
					_items = RJNEW Array<Item>();
				}
			#endif

			Combobox::Combobox(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
				_items = RJNEW Array<Item>();
			}

			Combobox::~Combobox()
			{
				DELETEARRAY(_items);
			}

			void Combobox::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;

					if (_parent != NULL)
						parentWin = (wxWindow *)_parent->_appObj;

					ComboboxFrame *object = RJNEW ComboboxFrame(parentWin, _text.towxString(),
						wxPoint(_transform->x, _transform->y), wxSize(_transform->width, _transform->height));
					object->Show(_visible);

					_appObj = object;
				
					linkWith(object);

					setup();
				#endif
			}

			void Combobox::setText(String text)
			{
				_text = text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
						((ComboboxFrame *)_appObj)->SetValue(_text.towxString());
				#endif
			}

			String Combobox::getText()
			{
				String text = _text;

				#ifdef GUI_USE_WXWIDGETS
					if (_appObj != NULL)
					{
						wxString wxtext = ((ComboboxFrame *)_appObj)->GetValue();
						text = parsewxString(wxtext);
					}
				#endif

				return (text);
			}

			#if defined USE_V8 || defined USE_JAVASCRIPTCORE
				void Combobox::on(String event, RJ_FUNC_TYPE func)
				{
					#ifdef GUI_USE_WXWIDGETS
						CPP::GUI::ComboboxFrame *object = (CPP::GUI::ComboboxFrame *)_appObj;

						object->addNewEvent(event, object, func);

						if (event == "change")
						{
							#ifdef GUI_USE_WXWIDGETS
								object->Connect(wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler(ComboboxFrame::onChanged), object->createEvent(event, func));
							#endif
						}

						if (event == "dropdown")
						{
							#ifdef GUI_USE_WXWIDGETS
								object->Connect(wxEVT_COMMAND_COMBOBOX_DROPDOWN, wxCommandEventHandler(ComboboxFrame::onDropdown), object->createEvent(event, func));
							#endif
						}
					
						if (event == "closeup")
						{
							#ifdef GUI_USE_WXWIDGETS
								object->Connect(wxEVT_COMMAND_COMBOBOX_CLOSEUP, wxCommandEventHandler(ComboboxFrame::onCloseup), object->createEvent(event, func));
							#endif
						}
						if (event == "text")
						{
							#ifdef GUI_USE_WXWIDGETS
								object->Connect(wxEVT_COMMAND_COMBOBOX_CLOSEUP, wxCommandEventHandler(ComboboxFrame::onText), object->createEvent(event, func));
							#endif
						}

					#endif
				}
			#endif

			void Combobox::addItem(String item)
			{
				addItem(CPP::GUI::Combobox::Item (item));
			}

			void Combobox::addItem(Item item)
			{
				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;

					object->Append(item.text.towxString());
				#endif
			}

			void Combobox::setItems(Array<Item> *items)
			{
				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;
					wxString *wxitems = RJNEW wxString[items->size ()];

					for (RJUINT iIdx = 0; iIdx < items->size(); iIdx++)
					{
						String text = items->at (iIdx).text;
						wxitems[iIdx] = text.towxString();
					}

					_items = items;
					DELETE_ARRAY(wxitems);
				#endif
			}

			void Combobox::deleteItem(RJINT index)
			{
				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;
					object->Delete(index);
				#endif
			}

			Combobox::Item Combobox::getItem(RJINT index)
			{
				Item item;

				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;
					item.text = parsewxString(object->GetString(index));
					item.name = item.text;
				#endif

				return (item);
			}

			Array<Combobox::Item> *Combobox::getItems()
			{
				Array<Combobox::Item> *items = RJNEW Array<Combobox::Item> ();
				RJUINT size = 0;

				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;
					wxArrayString ary = object->GetStrings();
					size = ary.size();

					for (RJUINT iIdx = 0; iIdx < size; iIdx++)
					{
						wxString str = ary.Item(iIdx);
						String text = parsewxString(str);

						items->push_back(Combobox::Item (text));
					}

					DELETEARRAY(_items);
					_items = items;
				#endif

				return (items);
			}

			RJUINT Combobox::getNumItems()
			{
				RJUINT size = 0;

				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;
					wxArrayString ary = object->GetStrings();
					size = ary.size();
				#endif

				return (size);
			}

			void Combobox::clear()
			{
				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;
					object->Clear();
				#endif
			}

			void Combobox::setSelectedItemIndex(RJINT index)
			{
				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;
					object->Select(index);
				#endif
			}

			RJINT Combobox::getSelectedItemIndex()
			{
				RJINT index = 0;

				#ifdef GUI_USE_WXWIDGETS
					ComboboxFrame *object = (ComboboxFrame *)_appObj;
					index = object->GetSelection();
				#endif

				return (index);
			}

			#ifdef USE_V8
				v8::Local<v8::Object> Combobox::Item::toV8Object(V8JavascriptEngine *jsEngine, Combobox::Item obj)
				{
					v8::Local<v8::Object> lobj = v8::Object::New(jsEngine->isolate);

					lobj->Set(String("name").toV8String(jsEngine->isolate), obj.name.toV8String(jsEngine->isolate));
					lobj->Set(String("text").toV8String(jsEngine->isolate), obj.text.toV8String(jsEngine->isolate));

					return (lobj);
				}

				v8::Local<v8::Array> Combobox::Item::toV8Array(V8JavascriptEngine *jsEngine, Array<Item> *objs)
				{
					v8::Local<v8::Array> newary = v8::Array::New(jsEngine->isolate, objs->size ());

					for (RJUINT iIdx = 0; iIdx < objs->size(); iIdx++)
					{
						Item item = objs->at(iIdx);
						v8::Local<v8::Object> obj = v8::Object::New(jsEngine->isolate);

						obj->Set(String("name").toV8String(jsEngine->isolate), item.name.toV8String(jsEngine->isolate));
						obj->Set(String("text").toV8String(jsEngine->isolate), item.text.toV8String(jsEngine->isolate));

						newary->Set(iIdx, obj);
					}

					return (newary);
				}
			#elif defined USE_JAVASCRIPTCORE
				JSObjectRef Combobox::Item::toJSCObject(JSCJavascriptEngine *jsEngine, JSContextRef ctx, Item obj)
				{
					JSObjectRef lobj = JSObjectMake(ctx, nullptr, nullptr);
					
					jsEngine->jscSetString(lobj, "name", obj.name);
					jsEngine->jscSetString(lobj, "text", obj.text);
					
					return (lobj);
				}

				JSObjectRef Combobox::Item::toJSCArray(JSCJavascriptEngine *jsEngine, JSContextRef ctx, Array<Item> *objs)
				{
					const RJUINT objectsSize = objs->size();
					JSValueRef objects[objectsSize];
					
					for (RJUINT iIdx = 0; iIdx < objectsSize; iIdx++)
					{
						Item item = objs->at(iIdx);
						
						JSObjectRef obj = JSObjectMake(ctx, nullptr, nullptr);
						jsEngine->jscSetString(obj, "name", item.name);
						jsEngine->jscSetString(obj, "text", item.text);
						
						objects[iIdx] = obj;
					}
					
					return JSObjectMakeArray(ctx, objectsSize, objects, nullptr);
				}
			#endif
		}
	}
}

#ifdef GUI_USE_WXWIDGETS
	wxBEGIN_EVENT_TABLE(RadJAV::CPP::GUI::ComboboxFrame, wxComboBox)
	wxEND_EVENT_TABLE()
#endif

