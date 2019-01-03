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
#ifndef _RADJAV_OS_CPP_SCREENINFO_H_
    #define _RADJAV_OS_CPP_SCREENINFO_H_

    #include "RadJavPreprocessor.h"
    #include "RadJavString.h"

    #ifdef USE_V8
        #include "v8/RadJavV8JavascriptEngine.h"
    #endif

    #ifdef USE_JAVASCRIPTCORE
        #include "jscore/RadJavJSCJavascriptEngine.h"
    #endif

    namespace RadJAV
    {
        namespace CPP
        {
            namespace OS
            {
				/**
				 * @ingroup group_os_cpp
				 * @brief ScreenInfo class.
				 * @details Class that query screen information.
				 */
                class RADJAV_EXPORT ScreenInfo
                {
                    public:
                        ScreenInfo(RJINT width = 0, RJINT height = 0, RJNUMBER scale = 1);
                        #ifdef USE_V8
                            ScreenInfo(V8JavascriptEngine *jsEngine, v8::Local<v8::Object> obj);

                            v8::Local<v8::Object> toV8Object();
                        #endif

                        #ifdef USE_JAVASCRIPTCORE
                            ScreenInfo(JSCJavascriptEngine *jsEngine, JSObjectRef obj);

                            JSObjectRef toJSCObject();
                        #endif

                        RJUINT getWidth();
                        RJUINT getHeight();
                        RJNUMBER getScale();
                    
                        RJUINT width;
                        RJUINT height;
                        RJNUMBER scale;
                    
                        /// Get the number of screens on the device.
                        static RJINT getNumberOfScreens ();
                        /// Get the screen info for the selected screen.
                        static ScreenInfo getScreenInfo (RJINT screenIndex);
                };
            }
        }
    }
#endif

