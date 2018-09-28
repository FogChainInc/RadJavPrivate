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
