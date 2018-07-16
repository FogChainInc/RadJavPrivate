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
#include "cpp/RadJavCPPGUIMenuItem.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			RJINT MenuItemGUI::nextId = 0;

			#ifdef USE_V8
				MenuItem::MenuItem(V8JavascriptEngine *jsEngine, const v8::FunctionCallbackInfo<v8::Value> &args)
					: GObject(jsEngine, args), GObjectBase()
				{
					v8::Handle<v8::Object> eventsV8 = V8_JAVASCRIPT_ENGINE->v8GetObject(args.This(), "_events");
					String click = V8_JAVASCRIPT_ENGINE->v8GetString(eventsV8, "click");

					if (click != "")
						clickEvent = click;
				}
			#endif

			MenuItem::MenuItem(String name, String text, CPP::GUI::GObject *parent)
				: GObject(name, text, parent)
			{
			}

			void MenuItem::create()
			{
				#ifdef GUI_USE_WXWIDGETS
					wxWindow *parentWin = NULL;
					String type = "";

					if (_parent != NULL)
					{
						type = _parent->type;
						parentWin = (wxWindow *)_parent->_appObj;
					}

					MenuItemGUI *menuItem = RJNEW MenuItemGUI();
					MenuItemGUI *parentMenu = NULL;

					if (type == "RadJav.GUI.MenuBar")
					{
						wxMenuBar *parentMenu = (wxMenuBar *)parentWin;
						menuItem->menu = RJNEW wxMenu();
						parentMenu->Append(menuItem->menu, _text.towxString());
						menuItem->parent = parentMenu;
						menuItem->parentType = MenuItemGUIParentTypes::MENU_BAR;
					}

					if (type == "RadJav.GUI.MenuItem")
					{
						parentMenu = (MenuItemGUI *)parentWin;

						if (parentMenu->menuItem != NULL)
						{
							String prevText = parsewxString(parentMenu->menuItem->GetItemLabelText());

							if (parentMenu->parentType == MenuItemGUIParentTypes::MENU_BAR)
								static_cast<wxMenuBar *> (parentMenu->parent)->Remove(parentMenu->menuItem->GetId());

							if (parentMenu->parentType == MenuItemGUIParentTypes::MENU)
								static_cast<wxMenu *> (parentMenu->parent)->Remove(parentMenu->menuItem->GetId());

							DELETE_OBJ(parentMenu->menuItem);
							parentMenu->menu = RJNEW wxMenu();

							if (parentMenu->parentType == MenuItemGUIParentTypes::MENU_BAR)
								static_cast<wxMenuBar *> (parentMenu->parent)->Append(parentMenu->menu, prevText.towxString());

							if (parentMenu->parentType == MenuItemGUIParentTypes::MENU)
								static_cast<wxMenu *> (parentMenu->parent)->AppendSubMenu(parentMenu->menu, prevText.towxString());
						}

						if (parentMenu->menu != NULL)
						{
							if (_text == "-")
								menuItem->menuItem = parentMenu->menu->AppendSeparator();
							else
								menuItem->menuItem = parentMenu->menu->Append(MenuItemGUI::nextId, _text.towxString());

							menuItem->parent = parentMenu->menu;
							menuItem->parentType = MenuItemGUIParentTypes::MENU;
						}
					}

					String click = clickEvent;

					if (parentMenu != NULL)
					{
						if (click != "")
						{
							if (parentMenu->menu != NULL)
							{
								String *str = RJNEW String(click);
								parentMenu->addData(MenuItemGUI::nextId, str, MenuItemGUIDataTypes::STRING);

								if (parentMenu->getNumData() == 1)
									parentMenu->menu->Bind(wxEVT_COMMAND_MENU_SELECTED, MenuItem::onClick, -1, MenuItemGUI::nextId, (wxObject *)parentMenu);
							}
						}
					}

					MenuItemGUI::nextId++;

					_appObj = menuItem;

					linkWith(menuItem);

				#endif

				setup();
			}

			#ifdef USE_V8
				void MenuItem::on(String event, v8::Local<v8::Function> func)
				{
				}
			#endif

			#ifdef GUI_USE_WXWIDGETS
				void MenuItem::onClick(wxCommandEvent &event)
				{
					MenuItemGUI *data = (MenuItemGUI *)event.GetEventUserData();
					RJINT menuId = event.GetId();

					if (data->getDataType(menuId) == MenuItemGUIDataTypes::STRING)
					{
						String *str = (String *)data->getData(menuId);
						String code = str->c_str();
						V8_JAVASCRIPT_ENGINE->executeScriptNextTick(code, "");
					}
				}
			#endif

			void MenuItemGUI::addData(RJINT menuId, void *data, RJINT dataType)
			{
				auto found = this->data.find(menuId);
				auto end = this->data.end();

				if (found != end)
				{
					void *temp = this->data.at(menuId);
					DELETE_OBJ(temp);

					this->data.erase(menuId);
					this->dataType.erase(menuId);
				}

				this->data.insert(HashMapPair<RJINT, void *>(menuId, data));
				this->dataType.insert(HashMapPair<RJINT, RJINT>(menuId, dataType));
			}
		}
	}
}

