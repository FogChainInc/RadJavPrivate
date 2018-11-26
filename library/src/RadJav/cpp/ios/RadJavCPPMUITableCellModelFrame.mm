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
#import "cpp/RadJavCPPMUITableCellModel.h"
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"
#import <UIKit/UIKit.h>
#import "cpp/RadJavCPPGUIGObject.h"

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            TableCellModelFrame::TableCellModelFrame()
            
            {

            }
			TableCellModelFrame::~TableCellModelFrame()
			{
			}

            bool TableCellModelFrame::bindEvent(const String& eventName, const GUI::Event* /*event*/)
            {
                return true;
            }
            
            void TableCellModelFrame::addNewEvent(String event,
#ifdef USE_V8
                                            v8::Local<v8::Function> func
#elif defined USE_JAVASCRIPTCORE
                                            JSObjectRef func
#endif
            )
            {
                
                
                bindEvent(event,createEvent(event, func));
                
            }

        }
    }
}
