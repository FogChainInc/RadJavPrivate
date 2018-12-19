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

#include "cpp/RadJavCPPMUIBottomNavigator.h"
#include "cpp/RadJavCPPMUIView.h"
#include "cpp/ios/RadJavCPPMUIUtils.h"
#include "cpp/ios/RadJavCPPMUIEventDelegates.h"

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
			BottomNavigatorFrame::BottomNavigatorFrame(ViewFrame *view)
            {
				if (view)
				{
					rootView = view->getNativeWidget();
                    RJViewController * controller = [[RJViewController alloc] init];
                    [controller.view addSubview:rootView];
                    widget = [[UITabBarController alloc] init];
                    [[UIApplication sharedApplication].keyWindow addSubview:widget.view];
				}
				else
				{
					//rootView = [UIApplication sharedApplication].keyWindow;
				}
                
            }

			BottomNavigatorFrame::~BottomNavigatorFrame()
			{
				//TODO: need to add implementation
			}
			int BottomNavigatorFrame::getSelectedTab(){
				if (widget)
					return (int)[widget viewControllers].count;
				
				return 0;
			}
			void BottomNavigatorFrame::addTab(View* view, String icon)
			{
				
				//Do nothing if there is no view provided
				if (!view || !rootView)
					return;
                dispatch_async(dispatch_get_main_queue(), ^{
					
					ViewFrame* viewFrame = (ViewFrame*)view->_appObj;
                    RJViewController * controller = [[RJViewController alloc] init];
                    [controller.view addSubview:viewFrame->getNativeWidget()];
					controller.delegate = viewFrame->widgetDelegate;
					NSString * name = RadJavCocoaStringFromRadJavString(view->_text);
					UIImage *nativeImage = nil;
					
					NSString * iconName = RadJavCocoaStringFromRadJavString(icon);
					if (iconName != nil && iconName.length > 0){
						NSString *path = [RadJavCocoaStringFromRadJavString(RadJavApplicationDirectory()) stringByAppendingString: iconName];
						nativeImage = [UIImage imageWithContentsOfFile:path];
					}
					controller.tabBarItem = [[UITabBarItem alloc] initWithTitle:name image:nativeImage tag:0];
					NSArray *newControllers = [widget.viewControllers arrayByAddingObject:controller];
					if (newControllers == nil){
						newControllers = @[controller];
					}
                    [widget setViewControllers:newControllers];
                });
			}
		}
    }
}
