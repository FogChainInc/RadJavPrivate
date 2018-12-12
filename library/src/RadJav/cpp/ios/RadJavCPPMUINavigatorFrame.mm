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

#include "cpp/RadJavCPPMUINavigator.h"
#include "cpp/RadJavCPPMUIView.h"

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
			NavigatorFrame::NavigatorFrame(ViewFrame *view)
            {
				if (view)
				{
					rootView = view->getNativeWidget();
                    UIViewController * controller = [[UIViewController alloc] init];
                    [controller.view addSubview:rootView];
                    widget = [[UINavigationController alloc] init];
                    [[UIApplication sharedApplication].keyWindow addSubview:widget.view];
				}
				else
				{
					//rootView = [UIApplication sharedApplication].keyWindow;
				}
                
            }

			NavigatorFrame::~NavigatorFrame()
			{
				//TODO: need to add implementation
			}

			void NavigatorFrame::push(ViewFrame* view, bool replace)
			{
				
				//Do nothing if there is no view provided
				if (!view || !rootView)
					return;
                dispatch_async(dispatch_get_main_queue(), ^{
                    
                    UIViewController * controller = [[UIViewController alloc] init];
                    [controller.view addSubview:view->getNativeWidget()];
                    
                    //if there's exactly one pop won't work
                    if (replace && widget.viewControllers.count <= 1){
                        
                        [widget setViewControllers:@[controller] animated:YES];
                    } else
                    
                    //if we have multiple controllers on stack pop and replace
                    if (replace && widget.viewControllers.count > 1){
                        
                        [widget popViewControllerAnimated:NO];
                        [widget pushViewController:controller animated:YES];
                    }
                    else {
                        [widget pushViewController:controller animated:YES];
                    }
                });
			}

			void NavigatorFrame::pop(ViewFrame* view)
			{
				if (!rootView)
					return;

                dispatch_async(dispatch_get_main_queue(), ^{
                    
                    UIView * nativeView = view->getNativeWidget();
                    for (UIViewController * i in widget.viewControllers){
                        
                        if (nativeView.superview == i.view) {
                            [widget popToViewController:i animated:YES];
                            return;
                        }
                    }
                    //we perform pop to root if param was empty or not on stack
                    pop();
                });
			}

			void NavigatorFrame::pop()
			{
                if (!rootView)
                    return;
                
                dispatch_async(dispatch_get_main_queue(), ^{
                    
                    [widget popViewControllerAnimated:YES];
                });
			}
		}
    }
}
