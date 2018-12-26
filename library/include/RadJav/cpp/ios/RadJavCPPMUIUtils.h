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
#include <string>
#include <JavaScriptCore/JSStringRef.h>
#include "RadJavString.h"

/** @method RadJavStringFromCocoaString
 * Converts Cocoa string to RadJav::String.
 * @param Cocoa string
 * @return RadJAV::String
 */
extern RadJAV::String RadJavStringFromCocoaString(NSString* string);
/** @method RadJavCocoaStringFromRadJavString
 * Converts RadJav::String to Cocoa string.
 * @param RadJAV::String string
 * @return Cocoa string
 */
extern NSString* RadJavCocoaStringFromRadJavString(RadJAV::String string);
/** @method RadJavCocoaStringFromJSCString
 * Converts JSStringRef to Cocoa string.
 * @param JSStringRef string
 * @return Cocoa string
 */
extern NSString* RadJavCocoaStringFromJSCString(JSStringRef jsString);

/** Following two methods are required for first created view controller to be set as root.
 */
/** @method RadJavRootControllerWasSet
 * Checks underlying static variable.
 * @return BOOL whether root controller was already set.
 */
extern BOOL RadJavRootControllerWasSet();
/** @method RadJavSetRootViewController
 * Sets rootviewcontroller and static bool value so it won't be set twice.
 * @param UIViewController new root viewcontroller for application
 */
extern void RadJavSetRootViewController(UIViewController *controller);

/** Util methods for file management.
 */

/** @method RadJavApplicationDocumentsDirectory
 * Uses system API to receive path to documents directory.
 * @return Absolute documents path
 */
extern RadJAV::String RadJavApplicationDocumentsDirectory();

/** @method RadJavApplicationTempDirectory
 * Uses system API to receive path to temp directory.
 * @return Absolute temporary files path
 */
extern RadJAV::String RadJavApplicationTempDirectory();

/** @method RadJavApplicationDirectory
 * Uses NSBundle API to receive path to application binary.
 * @return Absolute application path
 */
extern RadJAV::String RadJavApplicationDirectory();
