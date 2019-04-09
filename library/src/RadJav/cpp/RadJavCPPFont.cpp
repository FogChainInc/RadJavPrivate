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

        #ifdef GUI_USE_WXWIDGETS
            Font::Font (wxFont font)
            {
                fontFamily = font.GetFamilyString ();
                size = font.GetPixelSize ().y;
                color = Color::Black;
                underline = font.GetUnderlined ();
                bold = font.GetUnderlined ();
                italic = font.GetUnderlined ();
            }

            wxFont Font::towxFont ()
            {
                wxFont wxfont;

                if (fontFamily != "")
                    wxfont.SetFaceName(fontFamily.towxString());
                else
                    wxfont = *wxSMALL_FONT;

                wxfont.SetPixelSize(wxSize(0, size));

                if (underline == true)
                    wxfont.MakeUnderlined();

                if (bold == true)
                    wxfont.MakeBold();

                if (italic == true)
                    wxfont.MakeItalic();
				
				return wxfont;
            }
        #endif

        #ifdef __APPLE__
            #if TARGET_OS_IOS == 1
                Font::Font (UIFont *font)
                    : Font ()
                {
                    fontFamily = parseNSString (font.familyName);
                    size = font.pointSize;
                }

                UIFont *Font::toUIFont ()
                {
					if (fontFamily.empty())
						return nil;
					
                    NSString *attr = @"";

                    if (bold == true)
                        attr = @"Bold";

                    if (italic == true)
                        attr = @"Italic";

                    if ((bold == true) && (italic == true))
                        attr = @"Bold Italic";

                    UIFontDescriptor *desc = [UIFontDescriptor fontDescriptorWithFontAttributes: @{
                                              @"NSFontFamilyAttribute": fontFamily.toNSString (),
                                              @"NSFontFaceAttribute": attr
                                              }];
					
                    UIFont *font = [UIFont fontWithDescriptor: desc size: size];

                    return (font);
                }
            #endif
        #endif

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
				color.r = colorv8->Get(String("r").toV8String(jsEngine->isolate))->NumberValue(jsEngine->globalContext).FromMaybe (0);
				color.g = colorv8->Get(String("g").toV8String(jsEngine->isolate))->NumberValue(jsEngine->globalContext).FromMaybe(0);
				color.b = colorv8->Get(String("b").toV8String(jsEngine->isolate))->NumberValue(jsEngine->globalContext).FromMaybe(0);
				color.a = colorv8->Get(String("a").toV8String(jsEngine->isolate))->NumberValue(jsEngine->globalContext).FromMaybe(0);
				size = obj->Get(String("size").toV8String(jsEngine->isolate))->IntegerValue(jsEngine->globalContext).FromMaybe(0);
				underline = obj->Get(String("underline").toV8String(jsEngine->isolate))->BooleanValue(jsEngine->globalContext).FromMaybe(false);
				bold = obj->Get(String("bold").toV8String(jsEngine->isolate))->BooleanValue(jsEngine->globalContext).FromMaybe(false);
				italic = obj->Get(String("italic").toV8String(jsEngine->isolate))->BooleanValue(jsEngine->globalContext).FromMaybe(false);
			}

			v8::Local<v8::Object> Font::toV8Object()
			{
                V8JavascriptEngine *jsEngine = V8_JAVASCRIPT_ENGINE;
				v8::Handle<v8::Function> func = jsEngine->v8GetFunction(jsEngine->radJav->Get(jsEngine->isolate), "Font");
				v8::Local<v8::Object> objv8 = jsEngine->v8CallAsConstructor(func, 0, NULL);

                #ifdef GUI_USE_WXWIDGETS
				wxFont wfont;

				if (wfont.IsOk() == true)
				{
					String nfontFamily = parsewxString(wfont.GetFaceName());
					RJNUMBER nr = color.r / 255;
					RJNUMBER ng = color.g / 255;
					RJNUMBER nb = color.b / 255;
					RJNUMBER na = color.a / 255;
					RJINT nsize = wfont.GetPixelSize().x;
					RJBOOL nunderlined = false;
					RJBOOL nbold = false;
					RJBOOL nitalic = false;

					if (wfont.GetUnderlined() == true)
						nunderlined = true;

					if (wfont.GetWeight() == wxFontWeight::wxFONTWEIGHT_BOLD)
						nbold = true;

					if (wfont.GetStyle() == wxFontStyle::wxFONTSTYLE_ITALIC)
						nitalic = true;

					jsEngine->v8SetString(objv8, "fontFamily", nfontFamily);

					v8::Local<v8::Object> ocolor = jsEngine->v8GetObject(objv8, "color");
					jsEngine->v8SetNumber(ocolor, "r", nr);
					jsEngine->v8SetNumber(ocolor, "g", ng);
					jsEngine->v8SetNumber(ocolor, "b", nb);
					jsEngine->v8SetNumber(ocolor, "a", na);

					jsEngine->v8SetNumber(objv8, "size", nsize);
					jsEngine->v8SetBool(objv8, "underline", nunderlined);
					jsEngine->v8SetBool(objv8, "bold", nbold);
					jsEngine->v8SetBool(objv8, "italic", nitalic);
				}
                #endif

				return (objv8);
			}
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

            JSObjectRef Font::toJSCObject()
            {
                JSCJavascriptEngine *jsEngine = JSC_JAVASCRIPT_ENGINE;
                JSObjectRef func = jsEngine->jscGetFunction(jsEngine->radJav, "Font");
                JSObjectRef objJSC = jsEngine->jscCallAsConstructor(func, 0, NULL);

                #ifdef GUI_USE_WXWIDGETS
                    wxFont wfont;

                    if (wfont.IsOk() == true)
                    {
                        String nfontFamily = parsewxString(wfont.GetFaceName());
                        RJNUMBER nr = color.r / 255;
                        RJNUMBER ng = color.g / 255;
                        RJNUMBER nb = color.b / 255;
                        RJNUMBER na = color.a / 255;
                        RJINT nsize = wfont.GetPixelSize().x;
                        RJBOOL nunderlined = false;
                        RJBOOL nbold = false;
                        RJBOOL nitalic = false;

                        if (wfont.GetUnderlined() == true)
                            nunderlined = true;

                        if (wfont.GetWeight() == wxFontWeight::wxFONTWEIGHT_BOLD)
                            nbold = true;

                        if (wfont.GetStyle() == wxFontStyle::wxFONTSTYLE_ITALIC)
                            nitalic = true;

                        jsEngine->jscSetString(objJSC, "fontFamily", nfontFamily);

                        JSObjectRef ocolor = jsEngine->jscGetObject(objJSC, "color");
                        jsEngine->jscSetNumber(ocolor, "r", nr);
                        jsEngine->jscSetNumber(ocolor, "g", ng);
                        jsEngine->jscSetNumber(ocolor, "b", nb);
                        jsEngine->jscSetNumber(ocolor, "a", na);

                        jsEngine->jscSetNumber(objJSC, "size", nsize);
                        jsEngine->jscSetBool(objJSC, "underline", nunderlined);
                        jsEngine->jscSetBool(objJSC, "bold", nbold);
                        jsEngine->jscSetBool(objJSC, "italic", nitalic);
                    }
                #endif

                return (objJSC);
            }
        #endif
	}
}

