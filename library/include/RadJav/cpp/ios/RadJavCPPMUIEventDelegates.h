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

#import <objc/NSObject.h>
#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>
#include <string>

namespace RadJAV
{
	namespace CPP
	{
		namespace GUI
		{
			class GObjectWidget;
		}
	}
}

namespace RadJAV
{
    namespace CPP
    {
        namespace MUI
        {
            class TableViewFrame;
            class TableViewModel;
			class BottomNavigatorFrame;
        }
    }
}

@interface ButtonDelegate : NSObject

- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName;
- (void)touchUp;
@property (nonatomic, assign) RadJAV::CPP::GUI::GObjectWidget* _Nullable widget;

@end

@interface SwitchDelegate : NSObject

- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName;
- (void)valueChanged;
@property (nonatomic, assign) RadJAV::CPP::GUI::GObjectWidget* _Nullable widget;

@end

@interface TextFieldDelegate : NSObject <UITextFieldDelegate>

- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName;
- (void)valueChanged;
@property (nonatomic, assign) RadJAV::CPP::GUI::GObjectWidget* _Nullable widget;

@end

@interface WebViewDelegate : NSObject <WKUIDelegate>

- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName;
@property (nonatomic, assign) RadJAV::CPP::GUI::GObjectWidget* _Nullable widget;

@end

@interface WebViewNavigationDelegate : NSObject <WKNavigationDelegate>

- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName;
@property (nonatomic, assign) RadJAV::CPP::GUI::GObjectWidget* _Nullable widget;

@end

@interface TableViewDelegate : NSObject <UITableViewDelegate,UITableViewDataSource>

- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName;

- (CGFloat)tableView:(nonnull UITableView *)tableView heightForRowAtIndexPath:(nonnull NSIndexPath *)indexPath;
- (CGFloat)tableView:(nonnull UITableView *)tableView heightForHeaderInSection:(NSInteger)section;
- (CGFloat)tableView:(nonnull UITableView *)tableView heightForFooterInSection:(NSInteger)section;
- (void)tableView:(nonnull UITableView *)tableView didSelectRowAtIndexPath:(nonnull NSIndexPath *)indexPath;


@property (nonatomic, assign) RadJAV::CPP::MUI::TableViewFrame* _Nullable widget;
@property (nonatomic, assign) RadJAV::CPP::MUI::TableViewModel* _Nullable model;
//@end
//TODO: decide should we decouple datasource
//@interface TableViewDataSource : NSObject <UITableViewDataSource>

//- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName;
//@property (nonatomic, assign) RadJAV::CPP::MUI::TableViewFrame* _Nullable widget;

- (NSInteger)tableView:(nonnull UITableView *)tableView numberOfRowsInSection:(NSInteger)section;
- (nonnull UITableViewCell *)tableView:(nonnull UITableView *)tableView cellForRowAtIndexPath:(nonnull NSIndexPath *)indexPath;

@end

@interface BottomNavigatorDelegate : NSObject<UITabBarControllerDelegate>

- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName;
@property (nonatomic, assign) RadJAV::CPP::MUI::BottomNavigatorFrame* _Nullable widget;

@end
