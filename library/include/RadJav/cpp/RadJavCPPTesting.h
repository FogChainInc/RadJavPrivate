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
#ifndef _RADJAV_CPP_TESTING_H_
    #define _RADJAV_CPP_TESTING_H_

    #include "RadJavPreprocessor.h"
    #include "RadJavString.h"
    #include "RadJavHashMap.h"

	#include "RadJavCPPVector2.h"

    #ifdef __APPLE__
        #include <TargetConditionals.h>

        #if TARGET_OS_OSX == 1
            #include <ApplicationServices/ApplicationServices.h>
        #endif
    #endif

    namespace RadJAV
    {
        namespace CPP
        {
            namespace Testing
            {
				/// Simulates a keyboard.
                class RADJAV_EXPORT KeyboardSimulator
                {
                    public:
                        #ifdef __APPLE__
                            #if TARGET_OS_OSX == 1
                                static CGKeyCode charToKeyCode (RJCHAR key);
                            #endif
                        #endif

                        /// Simulate a key being pressed.
                        static void keyPress(String key);
                    
                        #ifdef __APPLE__
                            #if TARGET_OS_OSX == 1
                                static HashMap<CFStringRef, RJINT> *dictionary;
                            #endif
                        #endif
                };

				/// Simulates a mouse.
				class RADJAV_EXPORT MouseSimulator
				{
					public:
						/// Simulate a mouse click.
						static void click(RJINT button);
						/// Set the mouse at a position.
						static void setPosition(CPP::Vector2 pos);
					
						/// Stored position of the input device
						static CPP::Vector2 position;
				};
            }
        }
    }
#endif

