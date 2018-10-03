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
#include "jscore/RadJavJSCMUIImage.h"

#include "RadJav.h"

#include "jscore/RadJavJSCJavascriptEngine.h"

#include "cpp/RadJavCPPMUIImage.h"

namespace RadJAV
{
	namespace JSC
	{
		namespace MUI
		{
			using CppMuiObject = CPP::MUI::Image;
			
			void Image::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "create", Image::create);
				JSC_CALLBACK(object, "setImage", Image::setImage);
				JSC_CALLBACK(object, "setScaleMode", Image::setScaleMode);
				JSC_CALLBACK(object, "getScaleMode", Image::getScaleMode);
			}
			
			JSValueRef Image::create(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				CppMuiObject *appObject = RJNEW CppMuiObject(JSC_JAVASCRIPT_ENGINE, thisObject, argumentCount, arguments);
				appObject->create();
				
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_appObj", appObject);
				JSObjectRef _guiFinishedCreatingGObject = JSC_JAVASCRIPT_ENGINE->jscGetFunction(JSC_RADJAV, "_guiFinishedCreatingGObject");
				JSObjectRef promise = JSC_JAVASCRIPT_ENGINE->createPromise(thisObject, _guiFinishedCreatingGObject);
				
				return promise;
			}
			
			JSValueRef Image::setImage(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject *appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Image not initialized");
					return undefined;
				}
				
				JSValueRef imagePathValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!imagePathValue ||
					!JSValueIsString(ctx, imagePathValue))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Path argument required");
					return undefined;
				}
				
				appObject->setImage(parseJSCValue(ctx, imagePathValue));
				
				return undefined;
			}
			
			JSValueRef Image::setScaleMode(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject *appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Image not initialized");
					return undefined;
				}
				
				JSValueRef scaleModeValue = JSC_JAVASCRIPT_ENGINE->jscGetArgument(arguments, argumentCount, 0);
				
				if (!scaleModeValue ||
					!JSValueIsNumber(ctx, scaleModeValue))
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "ScaleMode argument required");
					return undefined;
				}
				
				RJINT scaleMode = JSC_JAVASCRIPT_ENGINE->jscValueToInt(scaleModeValue);
				
				switch (scaleMode)
				{
					case 1:
						appObject->setScaleMode(CPP::MUI::Image::ScaleMode::AspectFit);
						break;
					case 2:
						appObject->setScaleMode(CPP::MUI::Image::ScaleMode::AspectFill);
						break;
					default:
						JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Unsupported ScaleMode");
				}
				return undefined;
			}

			JSValueRef Image::getScaleMode(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				CppMuiObject *appObject = (CppMuiObject*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, thisObject, "_appObj");
				
				if (!appObject)
				{
					JSC_JAVASCRIPT_ENGINE->throwException(ctx, exception, "Image not initialized");
					return undefined;
				}

				auto scaleMode = appObject->getScaleMode();
				
				switch (scaleMode)
				{
					case CPP::MUI::Image::ScaleMode::AspectFill:
						return JSValueMakeNumber(ctx, 2.0);
					case CPP::MUI::Image::ScaleMode::AspectFit:
					default:
						return JSValueMakeNumber(ctx, 1.0);
				}
			}
		}
	}
}
