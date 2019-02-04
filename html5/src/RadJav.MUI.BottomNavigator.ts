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
/// <reference path="RadJav.MUI.View.ts" />

namespace RadJav
{
	export namespace MUI
	{
		/** @class RadJav.MUI.BottomNavigator
		 * A mobile view tab control.
		 * Available on platforms: iOS,Android,HTML5
		 */
		export class BottomNavigator
		{
			static xmlTag: TagType = { tag: "BottomNavigator", type: "BottomNavigator" };

			/** @property {string} [type=""]
			 * The type of object.
			 */
			type: string;

			/** @property {RadJav.MUI.Window} [type=""]
			 * The root Window object.
			 */
			rootWin: RadJav.MUI.Window;

			constructor(view? :RadJav.MUI.Window)
			{
				this.type = "RadJav.MUI.BottomNavigator";
				this.rootWin = view;

				if(this._init != null)
				{
					this._init.apply(this, arguments);
				}
			}
			
			
			public addTab(view: RadJav.MUI.Window, replace?: boolean)
			{
				if(this._addTab != null)
				{
					this._addTab.apply(this, arguments);
				}
			}
		}
	}
}
