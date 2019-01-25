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
/// <reference path="RadJav.Vector2.ts" />

namespace RadJav
{
	export namespace MUI
	{
		/** @class RadJav.MUI.TableViewModel
		 * A model for TableView control.
		 * Available on platforms: iOS,Android,HTML5
		 */
		export class TableViewModel
		{
			static xmlTag: TagType = { tag: "tableviewmodel", type: "TableViewModel" };

			/** @property {string} [type=""]
			 * The type of object.
			 */
			type: string;

			/** @property {Array} [items=[]]
			 * The items hold by this model.
			 */
			private items: object[];

			/** @property {any} [_appObj=null]
			 * The type of object.
			 */
			private _appObj: any;

			constructor()
			{
				this.type = "RadJav.MUI.TableViewModel";
				this.items = [];
				this._appObj = null;

				if (this._init != null)
				{
					this._init.apply(this, arguments);
				}
			}

			push (item :any)
			{
				this.items.push(item);

				if (this._itemPushed != null)
				{
					this._itemPushed(this.items.length-1);
				}
			}

			remove (index :number)
			{
				if (index < 0 || index > this.items.length-1)
					return;
				
				this.items.splice(index, 1);

				if (this._itemRemoved != null)
				{
					this._itemRemoved(index);
				}
			}

			pop ()
			{
				this.remove(this.items.length-1);
			}

			clear ()
			{
				this.items.splice(0, this.items.length);

				if (this._itemsCleared != null)
				{
					this._itemsCleared();
				}
			}

			get (index :number)
			{
				if (index < 0 || index > this.items.length-1)
					return null;

				return this.items[index];
			}
		}
	}
}
