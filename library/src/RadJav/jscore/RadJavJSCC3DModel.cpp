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
#include "jscore/RadJavJSCC3DModel.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"
#include "cpp/RadJavCPPC3DModel.h"
#include "cpp/RadJavCPPC3DModelFactory.h"

#define C3DTYPE CPP::C3D::Model

namespace RadJAV
{
	namespace JSC
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			void Model::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", Model::init);
				JSC_CALLBACK(object, "_load", Model::load);
			}
			
			JSValueRef Model::init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				JSValueRef undefined = JSValueMakeUndefined(ctx);
				
				if( argumentCount == 0 ||
				    JSC_JAVASCRIPT_ENGINE->jscIsNull(ctx, arguments[0]))
				{
					JSC_JAVASCRIPT_ENGINE->throwException("Missing Canvas3D parameter");
					return undefined;
				}
				
				//Check if we were already contructed
				std::shared_ptr<C3DTYPE> object = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				if(object)
					return undefined;
				
				JSObjectRef canvasObject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(arguments[0]);
				
				CPP::GUI::Canvas3D* canvas =
					(CPP::GUI::Canvas3D*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, canvasObject, "_appObj");
				
				if(!canvas)
					return undefined;
				
				String name = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "name");
				String path = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "path");
				if(path.empty())
					return undefined;

				object.reset(RJNEW C3DTYPE(*canvas, path, name), [](C3DTYPE* p){DELETEOBJ(p)});
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_c3dObj", object);
				
				return undefined;
			}
			
			JSValueRef Model::load(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<C3DTYPE> object = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				if(!object)
					return JSValueMakeUndefined(ctx);

				return JSValueMakeBoolean(ctx, object->load());
			}
#endif
		}
	}
}
#endif
