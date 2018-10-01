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
#include "cpp/RadJavCPPMUIAlert.h"

#import <UIKit/UIKit.h>

namespace RadJAV
{
	namespace CPP
	{
		namespace MUI
		{
			AlertFrame::~AlertFrame()
			{
			}
			
			void AlertFrame::show(const String& caption, const String& message)
			{
				NSString* capt = [NSString stringWithUTF8String:caption.c_str()];
				NSString* msg = [NSString stringWithUTF8String:message.c_str()];

				UIAlertController* alert = [UIAlertController alertControllerWithTitle:capt
																			   message:msg
																		preferredStyle:UIAlertControllerStyleAlert];
				
				UIAlertAction* defaultAction = [UIAlertAction actionWithTitle:@"OK" style:UIAlertActionStyleDefault
																	  handler:^(UIAlertAction * action) {}];
				
				[alert addAction:defaultAction];
				
				//TODO: Maybe additional checks will be required here to get top most controller
				UIViewController *topViewController = [UIApplication sharedApplication].keyWindow.rootViewController;
				while (true)
				{
					if (topViewController.presentedViewController) {
						topViewController = topViewController.presentedViewController;
					} else if ([topViewController isKindOfClass:[UINavigationController class]]) {
						UINavigationController *nav = (UINavigationController *)topViewController;
						topViewController = nav.topViewController;
					} else if ([topViewController isKindOfClass:[UITabBarController class]]) {
						UITabBarController *tab = (UITabBarController *)topViewController;
						topViewController = tab.selectedViewController;
					} else {
						break;
					}
				}
				
				[topViewController presentViewController:alert animated:YES completion:nil];
			}
		}
	}
}
