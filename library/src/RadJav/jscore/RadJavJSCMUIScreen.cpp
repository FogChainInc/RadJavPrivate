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
#include "jscore/RadJavJSCMUIScreen.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUIScreen.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::Screen;
			
			void Screen::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				//Define Screen class
				JSClassRef classRef;
				
				JSClassDefinition classDef = kJSClassDefinitionEmpty;
				classDef.className = "Screen";
				classDef.attributes = kJSClassAttributeNone;
				
				JSStaticValue propertiesArray[] = {
					{ "width", getWidth, nullptr, kJSPropertyAttributeReadOnly },
					{ "height", getHeight, nullptr, kJSPropertyAttributeReadOnly },
					{ "scale", getScale, nullptr, kJSPropertyAttributeReadOnly },
					{ 0, 0, 0, 0 }
				};
				
				classDef.staticValues = propertiesArray;
				
				classRef = JSClassCreate(&classDef);
				
				JSObjectRef screenJsObj = JSObjectMake(context, classRef, nullptr);
				JSClassRelease(classRef);
				
				JSStringRef screenProperty = JSStringCreateWithUTF8CString("Screen");
				JSObjectSetProperty(context, object, screenProperty, screenJsObj, kJSPropertyAttributeReadOnly, nullptr);
				JSStringRelease(screenProperty);
			}
			
			JSValueRef Screen::getWidth(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
			{
				return JSValueMakeNumber(ctx, CppMuiObject::getWidth());
			}
			
			JSValueRef Screen::getHeight(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
			{
				return JSValueMakeNumber(ctx, CppMuiObject::getHeight());
			}

			JSValueRef Screen::getScale(JSContextRef ctx, JSObjectRef object, JSStringRef propertyName, JSValueRef* exception)
			{
				return JSValueMakeNumber(ctx, CppMuiObject::getScale());
			}
		}
	}
}
