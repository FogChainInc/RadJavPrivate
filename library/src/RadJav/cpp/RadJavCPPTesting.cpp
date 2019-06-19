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
#include "cpp/RadJavCPPTesting.h"

#include "RadJav.h"
#include "RadJavString.h"

#ifdef WIN32
	#include <windows.h>
#endif

#ifdef __APPLE__
    #if TARGET_OS_OSX == 1
        #include <CoreFoundation/CoreFoundation.h>
        #include <Carbon/Carbon.h>
    #endif
#endif

namespace RadJAV
{
	namespace CPP
	{
		namespace Testing
		{
			#ifdef __APPLE__
				#if TARGET_OS_OSX == 1
            		HashMap<CFStringRef, RJINT> *KeyboardSimulator::dictionary = NULL;

                    CGKeyCode KeyboardSimulator::charToKeyCode (RJCHAR key)
                    {
                        if (dictionary == NULL)
                        {
                            dictionary = RJNEW HashMap<CFStringRef, RJINT> ();

                            TISInputSourceRef keyboard = TISCopyCurrentKeyboardInputSource ();
                            CFDataRef layoutData = (CFDataRef)TISGetInputSourceProperty (keyboard, kTISPropertyUnicodeKeyLayoutData);
                            const UCKeyboardLayout *layout = (const UCKeyboardLayout *)CFDataGetBytePtr(layoutData);

                            for (RJINT iIdx = 0; iIdx < 128; iIdx++)
                            {
                                UInt32 keyDown = 0;
                                UniChar chars[4];
                                UniCharCount count;

                                UCKeyTranslate (layout, (CGKeyCode)iIdx, kUCKeyActionDisplay, 0, LMGetKbdType(), kUCKeyTranslateNoDeadKeysBit, &keyDown, sizeof (chars) / sizeof (chars[0]), &count, chars);
                                CFStringRef str = CFStringCreateWithCharacters(kCFAllocatorDefault, chars, 1);

                                if (str != NULL)
                                {
                                    dictionary->insert (std::pair<CFStringRef, RJINT> (str, iIdx));
                                    CFRelease (str);
                                }
                            }

                            CFRelease (keyboard);
                        }

                        UniChar unichar = key;
                        CFStringRef str = CFStringCreateWithCharacters (kCFAllocatorDefault, &unichar, 1);
                        CGKeyCode result = (CGKeyCode)dictionary->at (str);
                        CFRelease (str);

                        return (result);
                    }
                #endif
            #endif

            void KeyboardSimulator::keyPress(String key)
            {
				#ifdef WIN32
                    String temp = key.toUpperCase();
                    RJCHAR value = temp.at (0);
					INPUT input;

					input.type = INPUT_KEYBOARD;
					input.ki.wScan = 0;
					input.ki.dwExtraInfo = 0;

					input.ki.wVk = value;
					input.ki.time = 0;
					input.ki.dwFlags = 0;

					SendInput(1, &input, sizeof (INPUT));

					input.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &input, sizeof(INPUT));
				#endif

                #ifdef __APPLE__
                    #if TARGET_OS_OSX == 1
                        String temp = key.toLowerCase();
                        RJCHAR value = temp.at (0);
                        CGKeyCode keyCode = charToKeyCode (value);
                        CGEventRef event = CGEventCreateKeyboardEvent (NULL, keyCode, true);

                        CGEventPost (kCGHIDEventTap, event);
                        CFRelease (event);
                    #endif
                #endif
            }

			CPP::Vector2 MouseSimulator::position = CPP::Vector2(0,0);
			
			void MouseSimulator::click(RJINT button)
			{
				#ifdef WIN32
					INPUT input;
                    RJINT flagDown = MOUSEEVENTF_ABSOLUTE;
                    RJINT flagUp = MOUSEEVENTF_ABSOLUTE;

					if (button == 0)
					{
						flagDown |= MOUSEEVENTF_LEFTDOWN;
						flagUp |= MOUSEEVENTF_LEFTUP;
					}

					if (button == 1)
					{
						flagDown |= MOUSEEVENTF_RIGHTDOWN;
						flagUp |= MOUSEEVENTF_RIGHTUP;
					}

					if (button == 2)
					{
						flagDown |= MOUSEEVENTF_MIDDLEDOWN;
						flagUp |= MOUSEEVENTF_MIDDLEUP;
					}

					input.type = INPUT_MOUSE;
					input.mi.dx = position.x;
					input.mi.dy = position.y;
					input.mi.mouseData = 0;
					input.mi.dwExtraInfo = 0;

					input.mi.time = 0;
					input.mi.dwFlags = flagDown;

					SendInput(1, &input, sizeof(INPUT));

					input.mi.dwFlags = flagUp;
					SendInput(1, &input, sizeof(INPUT));
				#endif

                #ifdef __APPLE__
                    #if TARGET_OS_OSX == 1
                        CGEventRef eventDown = NULL;
                        CGEventRef eventUp = NULL;
						const CGPoint pos = CGPointMake(position.x, position.y);

                        if (button == 0)
                        {
                            eventDown = CGEventCreateMouseEvent (NULL, kCGEventLeftMouseDown, pos, kCGMouseButtonLeft);
                            eventUp = CGEventCreateMouseEvent (NULL, kCGEventLeftMouseUp, pos, kCGMouseButtonLeft);
                        }

                        if (button == 1)
                        {
                            eventDown = CGEventCreateMouseEvent (NULL, kCGEventRightMouseDown, pos, kCGMouseButtonRight);
                            eventUp = CGEventCreateMouseEvent (NULL, kCGEventRightMouseUp, pos, kCGMouseButtonRight);
                        }

                        if (button == 2)
                        {
                            eventDown = CGEventCreateMouseEvent (NULL, kCGEventOtherMouseDown, pos, kCGMouseButtonCenter);
                            eventUp = CGEventCreateMouseEvent (NULL, kCGEventOtherMouseUp, pos, kCGMouseButtonCenter);
                        }

                        CGEventPost (kCGHIDEventTap, eventDown);
                        CFRelease (eventDown);

                        CGEventPost (kCGHIDEventTap, eventUp);
                        CFRelease (eventUp);
                    #endif
                #endif
			}

			void MouseSimulator::setPosition(CPP::Vector2 pos)
			{
				position = pos;
				#ifdef WIN32
					SetCursorPos(position.x, position.y);

					/*INPUT input;

					input.type = INPUT_MOUSE;
					input.mi.dx = pos.x;
					input.mi.dy = pos.y;
					input.mi.mouseData = 0;
					input.mi.dwExtraInfo = 0;

					input.mi.time = 0;
					input.mi.dwFlags = MOUSEEVENTF_MOVE | MOUSEEVENTF_VIRTUALDESK | MOUSEEVENTF_ABSOLUTE;

					SendInput(1, &input, sizeof(INPUT));*/
				#endif

                #ifdef __APPLE__
                    #if TARGET_OS_OSX == 1
                        CGEventRef event = CGEventCreateMouseEvent (NULL, kCGEventMouseMoved, CGPointMake(position.x, position.y), kCGMouseButtonLeft);

                        CGEventPost (kCGHIDEventTap, event);
                        CFRelease (event);
                    #endif
                #endif
			}
			
			void MouseSimulator::wheel(RJINT vertical, RJINT horizontal)
			{
				#ifdef WIN32
					INPUT input;
					RJINT flagVerticalWheel = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_WHEEL;
					RJINT flagHorizontalWheel = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_HWHEEL;
				
					input.type = INPUT_MOUSE;
					input.mi.dwFlags = flagVerticalWheel;
					input.mi.mouseData = vertical;
					input.mi.dx = position.x;
					input.mi.dy = position.y;
					input.mi.dwExtraInfo = 0;
					input.mi.time = 0;
				
					SendInput(1, &input, sizeof(INPUT));
				
					input.mi.dwFlags = flagHorizontalWheel;
					input.mi.mouseData = horizontal;
				
					SendInput(1, &input, sizeof(INPUT));
				#endif
				
				#ifdef __APPLE__
					#if TARGET_OS_OSX == 1
						CGEventRef eventWheel = nullptr;
				
						eventWheel = CGEventCreateScrollWheelEvent(nullptr, kCGScrollEventUnitLine, 2, vertical, horizontal);
						CGEventSetLocation(eventWheel, CGPointMake(position.x, position.y));
						
						CGEventPost (kCGHIDEventTap, eventWheel);
						CFRelease (eventWheel);
					#endif
				#endif
			}
		}
	}
}

