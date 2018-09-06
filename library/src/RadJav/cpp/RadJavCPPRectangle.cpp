/*
	MIT-LICENSE
	Copyright (c) 2017-2018 Higher Edge Software, LLC

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
#include "cpp/RadJavCPPRectangle.h"
#include "cpp/RadJavCPPColor.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		Rectangle::Rectangle(RJNUMBER x, RJNUMBER y, RJNUMBER w, RJNUMBER h)
		{
			this->x = x;
			this->y = y;
			this->width = w;
			this->height = h;
			top = this->y + this->height;
			bottom = this->y - this->height;
			left = this->x - this->width;
			right = this->x + this->width;
		}

		#ifdef USE_V8
			Rectangle::Rectangle(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> obj)
			{
				RJINT x = jsEngine->v8GetInt(obj, "x");
				RJINT y = jsEngine->v8GetInt(obj, "y");
				RJINT width = jsEngine->v8GetInt(obj, "width");
				RJINT height = jsEngine->v8GetInt(obj, "height");

				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;
				top = this->y + this->height;
				bottom = this->y - this->height;
				left = this->x - this->width;
				right = this->x + this->width;
			}

			v8::Local<v8::Object> Rectangle::toV8Object(V8JavascriptEngine *jsEngine, Rectangle *obj)
			{
				v8::Handle<v8::Function> func = jsEngine->v8GetFunction(jsEngine->radJav->Get (jsEngine->isolate), "Rectangle");
				v8::Local<v8::Object> objv8 = jsEngine->v8CallAsConstructor(func, 0, NULL);

				jsEngine->v8SetNumber(objv8, "x", obj->x);
				jsEngine->v8SetNumber(objv8, "y", obj->y);
				jsEngine->v8SetNumber(objv8, "width", obj->width);
				jsEngine->v8SetNumber(objv8, "height", obj->height);
				jsEngine->v8SetNumber(objv8, "top", obj->top);
				jsEngine->v8SetNumber(objv8, "bottom", obj->bottom);
				jsEngine->v8SetNumber(objv8, "left", obj->left);
				jsEngine->v8SetNumber(objv8, "right", obj->right);

				return (objv8);
			}
		#endif

        #ifdef USE_JAVASCRIPTCORE
            Rectangle::Rectangle(JSCJavascriptEngine *jsEngine, JSObjectRef obj)
            {
                RJINT x = jsEngine->jscGetInt(obj, "x");
                RJINT y = jsEngine->jscGetInt(obj, "y");
                RJINT width = jsEngine->jscGetInt(obj, "width");
                RJINT height = jsEngine->jscGetInt(obj, "height");

                this->x = x;
                this->y = y;
                this->width = width;
                this->height = height;
                top = this->y + this->height;
                bottom = this->y - this->height;
                left = this->x - this->width;
                right = this->x + this->width;
            }

            JSObjectRef Rectangle::toJSCObject(JSCJavascriptEngine *jsEngine, Rectangle *obj)
            {
                JSObjectRef func = jsEngine->jscGetFunction(jsEngine->radJav, "Rectangle");
                JSObjectRef objJSC = jsEngine->jscCallAsConstructor(func, 0, NULL);

                jsEngine->jscSetNumber(objJSC, "x", obj->x);
                jsEngine->jscSetNumber(objJSC, "y", obj->y);
                jsEngine->jscSetNumber(objJSC, "width", obj->width);
                jsEngine->jscSetNumber(objJSC, "height", obj->height);
                jsEngine->jscSetNumber(objJSC, "top", obj->top);
                jsEngine->jscSetNumber(objJSC, "bottom", obj->bottom);
                jsEngine->jscSetNumber(objJSC, "left", obj->left);
                jsEngine->jscSetNumber(objJSC, "right", obj->right);

                return (objJSC);
            }
        #endif

		void Rectangle::setPosition(Vector2 pos)
		{
			x = pos.x;
			y = pos.y;

			this->x = x;
			this->y = y;
			this->top = this->y + this->height;
			this->bottom = this->y - this->height;
			this->left = this->x - this->width;
			this->right = this->x + this->width;
		}

		void Rectangle::setPosition(RJNUMBER x, RJNUMBER y)
		{
			this->x = x;
			this->y = y;
			this->top = this->y + this->height;
			this->bottom = this->y - this->height;
			this->left = this->x - this->width;
			this->right = this->x + this->width;
		}

		Vector2 Rectangle::getPosition()
		{
			return (Vector2(this->x, this->y));
		}

		void Rectangle::setSize(Vector2 size)
		{
			this->width = size.x;
			this->height = size.y;
			this->top = this->y + this->height;
			this->bottom = this->y - this->height;
			this->left = this->x - this->width;
			this->right = this->x + this->width;
		}

		void Rectangle::setSize(RJNUMBER w, RJNUMBER h)
		{
			this->width = w;
			this->height = h;
			this->top = this->y + this->height;
			this->bottom = this->y - this->height;
			this->left = this->x - this->width;
			this->right = this->x + this->width;
		}

		Vector2 Rectangle::getSize()
		{
			return (Vector2(this->width, this->height));
		}

		RJBOOL Rectangle::pointInside(RJNUMBER x, RJNUMBER y)
		{
			if (x > this->right)
				return (false);

			if (x < this->left)
				return (false);

			if (y > this->top)
				return (false);

			if (y < this->bottom)
				return (false);

			return (true);
		}

		RJBOOL Rectangle::xInside(RJNUMBER x)
		{
			if (x > this->right)
				return (false);

			if (x < this->left)
				return (false);

			return (true);
		}

		RJBOOL Rectangle::yInside(RJNUMBER y)
		{
			if (y > this->top)
				return (false);

			if (y < this->bottom)
				return (false);

			return (true);
		}
	}
}

