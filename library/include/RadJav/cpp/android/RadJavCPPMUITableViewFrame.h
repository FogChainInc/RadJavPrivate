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
#ifndef _RADJAV_MUI_CPP_TABLEVIEW_FRAME_H_
#define _RADJAV_MUI_CPP_TABLEVIEW_FRAME_H_

#include "cpp/RadJavCPPGUIGObject.h"
#include "cpp/RadJavCPPMUITableViewModel.h"
#include "cpp/RadJavCPPMUIView.h"
#include "cpp/RadJavCPPMUITableViewCellCreator.h"
#include "cpp/RadJavCPPEvent.h"

namespace RadJAV
{
	namespace CPP
	{
		class Persistent;
		
		namespace MUI
		{
			class TableView;
			class TableViewDelegate;

			class RADJAV_EXPORT TableViewFrame : public GUI::GObjectWidget
											, public ChainedPtr
			{
			public:
				TableViewFrame(GUI::GObjectWidget *parent, TableViewCellCreator& cellCreator, const String &text, const Vector2 &pos, const Vector2 &size);
				~TableViewFrame();

				bool bindEvent(const String& eventName, const CPP::Event* event);
				void setModel(MUI::TableViewModel *model);
				void reload();
				CPP::MUI::View* viewForCellModel(int index);

				MUI::TableViewModel *model;
				TableView *tableView;

			private:
				TableViewDelegate* widgetDelegate;

				static jclass nativeListViewClass;

				static jmethodID nativeConstructor;
				static jmethodID nativeSetAdapter;
			};
		}
	}
}
#endif
