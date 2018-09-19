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
#include "jscore/RadJavJSCC3DObject3D.h"

#include "RadJav.h"

#ifdef USE_JAVASCRIPTCORE
#include "jscore/RadJavJSCJavascriptEngine.h"
#include "cpp/RadJavCPPC3DObject3D.h"

#define C3DTYPE CPP::C3D::Object3D

namespace RadJAV
{
	namespace JSC
	{
		namespace C3D
		{
#ifdef C3D_USE_OGRE
			void Object3D::createJSCCallbacks(JSContextRef context, JSObjectRef object)
			{
				JSC_CALLBACK(object, "_init", Object3D::init);
				JSC_CALLBACK(object, "_setVisibility", Object3D::setVisibility);
				JSC_CALLBACK(object, "_getVisibility", Object3D::getVisibility);
			}

			JSValueRef Object3D::init(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
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

				JSObjectRef canvasObject = JSC_JAVASCRIPT_ENGINE->jscCastValueToObject(ctx, arguments[0]);
				CPP::GUI::Canvas3D* canvas =
					(CPP::GUI::Canvas3D*)JSC_JAVASCRIPT_ENGINE->jscGetExternal(ctx, canvasObject, "_appObj");
				
				if(!canvas)
					return undefined;

				String name = JSC_JAVASCRIPT_ENGINE->jscGetString(thisObject, "name");
				object.reset(RJNEW C3DTYPE(*canvas, name), [](C3DTYPE* p){DELETEOBJ(p)});
				JSC_JAVASCRIPT_ENGINE->jscSetExternal(ctx, thisObject, "_c3dObj", object);
				
				return undefined;
			}

			JSValueRef Object3D::setVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				RJBOOL visible = true;
				
				if (argumentCount > 0)
					visible = JSC_JAVASCRIPT_ENGINE->jscValueToBoolean(ctx, arguments[0]);

				std::shared_ptr<C3DTYPE> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				
				if (c3dObject)
					c3dObject->setVisible(visible);
				
				return JSValueMakeUndefined(ctx);
			}

			JSValueRef Object3D::getVisibility(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef* exception)
			{
				std::shared_ptr<C3DTYPE> c3dObject = JSC_JAVASCRIPT_ENGINE->jscGetExternal<C3DTYPE>(ctx, thisObject, "_c3dObj");
				
				return JSValueMakeBoolean(ctx, c3dObject? c3dObject->getVisible() : false);
			}
#endif
		}
	}
}
#endif

