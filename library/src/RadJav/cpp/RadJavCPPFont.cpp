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
#include "cpp/RadJavCPPFont.h"
#include "cpp/RadJavCPPColor.h"

#include "RadJav.h"
#include "RadJavString.h"

namespace RadJAV
{
	namespace CPP
	{
		Font::Font()
		{
			fontFamily = "Open Sans";
			size = 10;
			color = Color::Black;
			underline = false;
			bold = false;
			italic = false;
		}

		#ifdef USE_V8
			Font::Font(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> obj)
			{
				fontFamily = "Open Sans";
				size = 10;
				color = Color::Black;
				underline = false;
				bold = false;
				italic = false;

				fontFamily = parseV8Value(obj->Get(String("fontFamily").toV8String(jsEngine->isolate)));
				v8::Handle<v8::Object> colorv8 = v8::Handle<v8::Object>::Cast(obj->Get(String("color").toV8String(jsEngine->isolate)));
				color.r = colorv8->Get(String("r").toV8String(jsEngine->isolate))->NumberValue();
				color.g = colorv8->Get(String("g").toV8String(jsEngine->isolate))->NumberValue();
				color.b = colorv8->Get(String("b").toV8String(jsEngine->isolate))->NumberValue();
				color.a = colorv8->Get(String("a").toV8String(jsEngine->isolate))->NumberValue();
				size = obj->Get(String("size").toV8String(jsEngine->isolate))->IntegerValue();
				underline = obj->Get(String("underline").toV8String(jsEngine->isolate))->BooleanValue();
				bold = obj->Get(String("bold").toV8String(jsEngine->isolate))->BooleanValue();
				italic = obj->Get(String("italic").toV8String(jsEngine->isolate))->BooleanValue();
			}

			#ifdef GUI_USE_WXWIDGETS
			v8::Local<v8::Object> Font::toV8Object(V8JavascriptEngine *jsEngine, Font *font)
			{
				v8::Handle<v8::Function> func = jsEngine->v8GetFunction(jsEngine->radJav->Get(jsEngine->isolate), "Font");
				v8::Local<v8::Object> objv8 = jsEngine->v8CallAsConstructor(func, 0, NULL);

				wxFont wfont;

				if (wfont.IsOk() == true)
				{
					String fontFamily = parsewxString(wfont.GetFaceName());
					wxColor color;
					RJNUMBER r = font->color.r / 255;
					RJNUMBER g = font->color.g / 255;
					RJNUMBER b = font->color.b / 255;
					RJNUMBER a = font->color.a / 255;
					RJINT size = wfont.GetPixelSize().x;
					RJBOOL underlined = false;
					RJBOOL bold = false;
					RJBOOL italic = false;

					if (wfont.GetUnderlined() == true)
						underlined = true;

					if (wfont.GetWeight() == wxFontWeight::wxFONTWEIGHT_BOLD)
						bold = true;

					if (wfont.GetStyle() == wxFontStyle::wxFONTSTYLE_ITALIC)
						italic = true;

					jsEngine->v8SetString(objv8, "fontFamily", fontFamily);

					v8::Local<v8::Object> ocolor = jsEngine->v8GetObject(objv8, "color");
					jsEngine->v8SetNumber(ocolor, "r", r);
					jsEngine->v8SetNumber(ocolor, "g", g);
					jsEngine->v8SetNumber(ocolor, "b", b);
					jsEngine->v8SetNumber(ocolor, "a", a);

					jsEngine->v8SetNumber(objv8, "size", size);
					jsEngine->v8SetBool(objv8, "underline", underlined);
					jsEngine->v8SetBool(objv8, "bold", bold);
					jsEngine->v8SetBool(objv8, "italic", italic);
				}

				return (objv8);
			}
			#endif
		#endif

        #ifdef USE_JAVASCRIPTCORE
            Font::Font(JSCJavascriptEngine *jsEngine, JSObjectRef obj)
            {
                fontFamily = "Open Sans";
                size = 10;
                color = Color::Black;
                underline = false;
                bold = false;
                italic = false;

                fontFamily = jsEngine->jscGetString(obj, "fontFamily");
                JSObjectRef colorJSC = jsEngine->jscGetObject(obj, "color");
                color.r = jsEngine->jscGetDecimal(colorJSC, "r");
                color.g = jsEngine->jscGetDecimal(colorJSC, "g");
                color.b = jsEngine->jscGetDecimal(colorJSC, "b");
                color.a = jsEngine->jscGetDecimal(colorJSC, "a");
                size = jsEngine->jscGetInt(obj, "size");
                underline = jsEngine->jscGetBool(obj, "underline");
                bold = jsEngine->jscGetBool(obj, "bold");
                italic = jsEngine->jscGetBool(obj, "italic");
            }

            #ifdef GUI_USE_WXWIDGETS
                JSObjectRef Font::toJSCObject(JSCJavascriptEngine *jsEngine, Font *font)
                {
                    JSObjectRef func = jsEngine->jscGetFunction(jsEngine->radJav, "Font");
                    JSObjectRef objJSC = jsEngine->jscCallAsConstructor(func, 0, NULL);

                    wxFont wfont;

                    if (wfont.IsOk() == true)
                    {
                        String fontFamily = parsewxString(wfont.GetFaceName());
                        wxColor color;
                        RJNUMBER r = font->color.r / 255;
                        RJNUMBER g = font->color.g / 255;
                        RJNUMBER b = font->color.b / 255;
                        RJNUMBER a = font->color.a / 255;
                        RJINT size = wfont.GetPixelSize().x;
                        RJBOOL underlined = false;
                        RJBOOL bold = false;
                        RJBOOL italic = false;
                        
                        if (wfont.GetUnderlined() == true)
                            underlined = true;
                        
                        if (wfont.GetWeight() == wxFontWeight::wxFONTWEIGHT_BOLD)
                            bold = true;
                        
                        if (wfont.GetStyle() == wxFontStyle::wxFONTSTYLE_ITALIC)
                            italic = true;
                        
                        jsEngine->jscSetString(objJSC, "fontFamily", fontFamily);
                        
                        JSObjectRef ocolor = jsEngine->jscGetObject(objJSC, "color");
                        jsEngine->jscSetNumber(ocolor, "r", r);
                        jsEngine->jscSetNumber(ocolor, "g", g);
                        jsEngine->jscSetNumber(ocolor, "b", b);
                        jsEngine->jscSetNumber(ocolor, "a", a);
                        
                        jsEngine->jscSetNumber(objJSC, "size", size);
                        jsEngine->jscSetBool(objJSC, "underline", underlined);
                        jsEngine->jscSetBool(objJSC, "bold", bold);
                        jsEngine->jscSetBool(objJSC, "italic", italic);
                    }

                    return (objJSC);
                }
            #endif
        #endif
	}
}

