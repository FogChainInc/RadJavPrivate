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
#import "cpp/ios/RadJavCPPMUIEventDelegates.h"
#import "cpp/ios/RadJavCPPMUIUtils.h"
#include "cpp/RadJavCPPGUIGObject.h"
#include "cpp/RadJavCPPMUITableViewModel.h"
#include "cpp/RadJavCPPMUITableView.h"
#include "cpp/RadJavCPPMUITableCellModel.h"
#include <JavaScriptCore/JSStringRef.h>

@implementation ButtonDelegate
- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName
{
	if (eventName == "click")
	{
		[nativeWidget removeTarget:self action:@selector(touchUp) forControlEvents:UIControlEventTouchUpInside];
		[nativeWidget addTarget:self action:@selector(touchUp) forControlEvents:UIControlEventTouchUpInside];
		
		return true;
	}
	
	return false;
}

- (void)touchUp
{
	self.widget->executeEvent("click");
}

@end

@implementation SwitchDelegate
- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName
{
	if (eventName == "changed")
	{
		[nativeWidget removeTarget:self action:@selector(valueChanged) forControlEvents:UIControlEventValueChanged];
		[nativeWidget addTarget:self action:@selector(valueChanged) forControlEvents:UIControlEventValueChanged];
		
		return true;
	}
	
	return false;
}

- (void)valueChanged
{
	self.widget->executeEvent("changed");
}

@end

@implementation TextFieldDelegate
- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName
{
	//We already subscribed to UITextView events here by this delegate
	//So we need only execute events to which we were subscribed (events subscription added in GObjectEvents class)
	if (eventName == "changed")
		return true;
	
	return false;
}

- (BOOL)textFieldShouldBeginEditing:(UITextField *)textField
{
	//NSLog(@"textFieldShouldBeginEditing");
	return true;
}

- (void)textFieldDidBeginEditing:(UITextField *)textField
{
	//NSLog(@"textFieldDidBeginEditing");
}

- (BOOL)textFieldShouldEndEditing:(UITextField *)textField
{
	//NSLog(@"textFieldShouldEndEditing");
	return true;
}

- (void)textFieldDidEndEditing:(UITextField *)textField reason:(UITextFieldDidEndEditingReason)reason
{
	//NSLog(@"textFieldDidEndEditing:reason");
	[self valueChanged];
}

- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
	//NSLog(@"textField");
	return true;
}

- (BOOL)textFieldShouldClear:(UITextField *)textField
{
	//NSLog(@"textFieldShouldClear");
	return true;
}

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
	//NSLog(@"textFieldShouldReturn");
	return [textField resignFirstResponder];
}

- (void)valueChanged
{
	self.widget->executeEvent("changed");
}

@end


@implementation WebViewDelegate
- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName
{
    //We already subscribed to UITextView events here by this delegate
    //So we need only execute events to which we were subscribed (events subscription added in GObjectEvents class)
    if (eventName == "changed")
        return true;
    
    return false;
}

@end


@implementation WebViewNavigationDelegate
- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName
{
    //We already subscribed to UITextView events here by this delegate
    //So we need only execute events to which we were subscribed (events subscription added in GObjectEvents class)
    if (eventName == "changed")
        return true;
    
    return false;
}

@end

@implementation TableViewDelegate

//- (CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
//{
//
//}
//- (CGFloat)tableView:(UITableView *)tableView heightForHeaderInSection:(NSInteger)section
//{
//
//}
//
//- (CGFloat)tableView:(UITableView *)tableView heightForFooterInSection:(NSInteger)section
//{
//
//}


- (bool)bindEvent:(nullable id)nativeWidget eventName:(const std::string&)eventName
{
    if (eventName == "click")
    {
        //[nativeWidget removeTarget:self action:@selector(touchUp) forControlEvents:UIControlEventTouchUpInside];
       // [nativeWidget addTarget:self action:@selector(touchUp) forControlEvents:UIControlEventTouchUpInside];
        
        return true;
    }
    
    return false;
}

//- (void)touchUp
//{
//    self.widget->executeEvent("click");
//}
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    RadJAV::CPP::MUI::TableCellModel * model = self.model->models->at(indexPath.row);
    
    model->setIsSelected(true);
    
    model->executeEvent("click");
}

- (void)tableView:(UITableView *)tableView didDeselectRowAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    RadJAV::CPP::MUI::TableCellModel * model = self.model->models->at(indexPath.row);
    
    model->setIsSelected(false);
}


- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    RadJAV::CPP::MUI::TableCellModel * model = self.model->models->at(indexPath.row);
    return model->getIsDeletable();
}

- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    RadJAV::CPP::MUI::TableCellModel * model = self.model->models->at(indexPath.row);
    model->executeEvent("delete");
}

- (void)tableView:(UITableView *)tableView accessoryButtonTappedForRowWithIndexPath:(nonnull NSIndexPath *)indexPath
{
    RadJAV::CPP::MUI::TableCellModel * model = self.model->models->at(indexPath.row);
    model->executeEvent("accessory_click");
}

//@end
//@implementation TableViewDataSource
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    
    return 1;//one section for now
}
- (NSInteger)tableView:(nonnull UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    if (self.model == nullptr){
        return 0;
    }
    
    
    return self.model->models->size();
}
- (nonnull UITableViewCell *)tableView:(nonnull UITableView *)tableView cellForRowAtIndexPath:(nonnull NSIndexPath *)indexPath
{
    RadJAV::CPP::MUI::TableCellModel * model = self.model->models->at(indexPath.row);
    model->widgetDelegate = self;
    UITableViewCell *cell = nil;//[tableView dequeueReusableCellWithIdentifier:@"identifier"];//tableView.dequeueReusableCell(withIdentifier: "LabelCell", for: indexPath)
    
    if (cell == nil){
        
        cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:@"identifier"];
        if (model->getUsesAccessoryButton()){
            cell.accessoryType = UITableViewCellAccessoryDetailButton;
        }
        
 
    }
    
    cell.textLabel.text = RadJavCocoaStringFromRadJavString(model->name);
    cell.detailTextLabel.text = RadJavCocoaStringFromRadJavString(model->name);
    
    return cell;
}
@end

