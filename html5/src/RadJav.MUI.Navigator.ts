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
/// <reference path="RadJav.Animation.ts" />
/// <reference path="RadJav.MUI.View.ts" />

namespace RadJav
{
	export namespace MUI
	{
		/** @class RadJav.MUI.Navigator
		 * A mobile view navigator.
		 * Available on platforms: iOS,Android,HTML5
		 */
		export class Navigator extends RadJav.GUI.GObject
		{
			static xmlTag: TagType = { tag: "navigator", type: "Navigator" };

			/** @property {string} [type=""]
			 * The type of object.
			 */
			type: string;
			/** @property {RadJav.MUI.View} [type=""]
			 * The root view object.
			 */
			rootWin: RadJav.MUI.View;
			/// The views in this navigator.
			views: RadJav.MUI.View[];
			/// The animation to use, not available for iOS/Android.
			animation: RadJav.Animation;

			constructor(rootView: RadJav.MUI.View, obj?: any, text?: string, parent?: RadJav.GUI.GObject)
			{
				if (obj == null)
					obj = {};

				if (typeof obj == "string")
				{
					var name = obj;
					obj = { name: name };
				}

				if (obj.size == null)
				{
					obj.size = new RadJav.Vector2();
					obj.size.x = 300;
					obj.size.y = 300;
				}

				super(obj, text, parent);

				this.type = "RadJav.MUI.Navigator";
				this.rootWin = rootView;
				this.views = [];
				this.animation = new RadJav.Animation ();
			}

			/// Set the animation to use.
		 	/// Available on platforms: Windows,Linux,Mac,HTML5
			public setAnimation (animation: RadJav.Animation)
			{
				this.animation = animation;
			}

			/// Get the animation to use.
		 	/// Available on platforms: Windows,Linux,Mac,HTML5
			public getAnimation (): RadJav.Animation
			{
				return (this.animation);
			}

			/// Push a view on to a navigator.
			public push(view: RadJav.MUI.View, replace?: boolean)
			{
				if (view.type != "RadJav.MUI.View")
				{
					throw new Error ("View must be of type RadJav.MUI.View!");

					return;
				}

				if ((RadJav.OS.type == "windows") && 
					(RadJav.OS.type == "linux") && 
					(RadJav.OS.type == "macosx") && 
					(RadJav.OS.type == "html5"))
				{
					if (this.views.length > 0)
					{
						let pos: RadJav.Vector2 = this.getSize ();
						pos.setY (0);

						view.setPosition (pos);

						this.animation.attach (view);
						this.animation.lerp (pos, new RadJav.Vector2 (0, 0), 1.3);
						this.animation.play ();
					}
				}

				this.views.push (view);

				if(this["_push"] != null)
				{
					this["_push"].apply(this, arguments);

					return;
				}
				else
				{
				}
			}

			/// Pop a view off the navigator.
			public pop(view?: RadJav.MUI.View)
			{
				if (view.type != "RadJav.MUI.View")
				{
					throw new Error ("View must be of type RadJav.MUI.View!");

					return;
				}

				if(this["_pop"] != null)
				{
					this["_pop"].apply(this, arguments);

					return;
				}
				else
				{
					RadJav.currentTheme.eventSync(this.type, "pop", this, view);

					for (let iIdx = 0; iIdx < this.views.length; iIdx++)
					{
						if (this.views[iIdx].name == view.name)
							this.views.splice (iIdx, 1);
					}
				}
			}
		}
	}
}

if (RadJav.GUI != null)
	RadJav.GUI["Navigator"] = RadJav.MUI.Navigator;
