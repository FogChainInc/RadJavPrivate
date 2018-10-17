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

/// <reference path="RadJav.ts" />

namespace RadJav
{
	/** @class RadJav.Testing
	* The testing class.
	* Available on platforms: Windows,Linux,OSX,iOS,Android
	*/
	export namespace Testing
	{
		/** Simulates keyboard input.
		* Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class KeyboardSimulator
		{
			/// Press a key.
			static keyPress (key: string): void
			{
			}
		}

		/** Simulates mouse input.
		* Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class MouseSimulator
		{
			/// Click a mouse button.
			static click (button: number): void
			{
			}

			/// Click a mouse button.
			static setPosition (pos: Vector2): void
			{
			}
		}

		/** Simulates touch input.
		* Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class TouchSimulator
		{
			/// Click a mouse button.
			static setPosition (pos: Vector2): void
			{
			}
		}

		/** Simulates accelerometer input.
		* Available on platforms: iOS,Android
		*/
		export class AccelerometerSimulator
		{
			/// Do a shake.
			static shake (pos: Vector2): void
			{
			}
		}
    }
}
