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
#import "cpp/ios/RadJavCPPMUIUtils.h"

extern RadJAV::String RadJavStringFromCocoaString(NSString* string)
{
    const char * charbuf = [string UTF8String];
    return * (new RadJAV::String(charbuf));
}

NSString* RadJavCocoaStringFromRadJavString(RadJAV::String string)
{
    JSStringRef jsString = string.toJSCString();
    
    return RadJavCocoaStringFromJSCString(jsString);
}

NSString* RadJavCocoaStringFromJSCString(JSStringRef jsString)
{
    size_t maxBufferSize = JSStringGetMaximumUTF8CStringSize(jsString);
    char* utf8Buffer = new char[maxBufferSize];
    JSStringGetUTF8CString(jsString, utf8Buffer, maxBufferSize);
    
    return [NSString stringWithUTF8String:utf8Buffer];

}

static BOOL radJavRootControllerWasSet = false;

extern BOOL RadJavRootControllerWasSet()
{
    return radJavRootControllerWasSet;
}
extern void RadJavSetRootViewController(UIViewController *controller)
{
    
    if (controller == nil){
        return;
    }
    radJavRootControllerWasSet = true;
    
    UIWindow * keyWindow = [UIApplication sharedApplication].keyWindow;
    [keyWindow setRootViewController:controller];
}

extern RadJAV::String RadJavApplicationDocumentsDirectory()
{
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    
    return RadJavStringFromCocoaString(documentsDirectory);
}

extern RadJAV::String RadJavApplicationTempDirectory()
{
    return RadJavStringFromCocoaString(NSTemporaryDirectory());
}

extern RadJAV::String RadJavApplicationDirectory()
{
    NSString* appPath = [[[NSBundle mainBundle] executablePath] stringByDeletingLastPathComponent];
    return RadJavStringFromCocoaString(appPath);
}
