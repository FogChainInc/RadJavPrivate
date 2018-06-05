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
	/// The RadJav Interact namespace.
	export namespace Interact
	{
		export function setView (root: HTMLElement | RadJav.GUI.GObject, view: RadJav.Interact.View): void
		{
			if (root.constructor["name"] == "GObject")
			{
				//for (let comp in view._components)
					//(<RadJav.GUI.GObject>root).addChild (view._components[comp].display);

				return;
			}

			if (root.constructor["name"] == "Object3D")
			{
				//for (let comp in view._components)
					//(<RadJav.GUI.Object3D>root).addChild (view._components[comp].display);

				return;
			}

			for (let comp in view._components)
				(<HTMLElement>root).appendChild ((<RadJav.GUI.HTMLElement>view._components[comp].display)._html);
		}

		/// A view that contains all the components.
		export class View
		{
			/// Name of the view.
			public name: string;
			/// The components in the view.
			public _components: { [name: string]: Component };

			constructor (name: string)
			{
				this.name = name;
				this._components = {};
			}

			/** @method addComponent
			* 
			*/
			addComponent (obj: RadJav.Interact.Component): void
			{
				if (obj.name == null)
					throw new Error ("Component cannot be null!");

				if (obj.name == "")
					throw new Error ("Component must have a name!");

				if (this._components[obj.name] != null)
					throw new Error ("Component cannot have the same name as an existing component!");

				if (typeof obj.display == "string")
					obj.display = new RadJav.GUI.HTMLElement (obj.name, obj.display);

				this._components[obj.name] = obj;
			}

			buildHTML (): HTMLOutput
			{
				let output = new HTMLOutput ();
				output.title = this.name;

				for (let comp in this._components)
				{
					let icomp = (<RadJav.GUI.HTMLElement>this._components[comp].display)._html;

					output.display.push (icomp);
				}

				return (output);
			}
		}

		/// A component.
		export class Component
		{
			public name: string;
			public tag: string;
			public display: RadJav.GUI.HTMLElement | RadJav.C3D.Object3D;

			constructor ()
			{
				this.name = "";
				this.tag = "";
				this.display = null;
			}
		}

		/// A view that contains all the components.
		export class HTMLOutput
		{
			public title: string;
			public header: string;
			public display: string[];
			public footer: string;

			constructor ()
			{
				this.header = "<!DOCTYPE html>\n";
				this.header += "<html>\n\n";
				this.header += "<head>\n";
				this.header += "\t<title>" + this.title + "</title>\n";
				this.header += "</head>\n\n";
				this.header += "<body>\n";

				this.footer = "</body>\n";
				this.footer = "</html>\n\n";
			}

			writeToFile (path: string): void
			{
				let output: string = "";

				output += this.header;

				for (let iIdx = 0; iIdx < this.display.length; iIdx++)
					output += this.display[iIdx] + "\n";

				output += this.footer;

				RadJav.IO.TextFile.writeTextToFile (path, output);
			}
		}
	}
}

