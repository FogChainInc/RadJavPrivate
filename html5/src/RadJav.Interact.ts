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
	/// The RadJav Interact namespace. RadJav Interact requires ES5.
	export namespace Interact
	{
		/// The app to run.
		export class App
		{
			public name: string;
			public _views: { [name: string]: View };

			public static defaultHeader: string;
			public static defaultFooter: string;

			constructor (name: string)
			{
				this.name = name;
				this._views = {};

				RadJav.Interact.App.defaultHeader = "<!DOCTYPE html>\n";
				RadJav.Interact.App.defaultHeader += "<html>\n";
				RadJav.Interact.App.defaultHeader += "<head>\n";
				RadJav.Interact.App.defaultHeader += "\t<title>{{title}}</title>\n";
				RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojo/themes/claro/claro.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/Grid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/claroGrid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/Calendar.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/MonthColumnView.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojo/resources/FloatingPane.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojo/resources/ResizeHandle.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				RadJav.Interact.App.defaultHeader += "{{cssFiles}}";
				RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\" src = \"./dependencies/three.js/three.min.js\"></script>\n";
				RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\" src = \"./dependencies/dojo/dojo/dojo.js\"></script>\n";
				RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.js\"></script>\n";
				RadJav.Interact.App.defaultHeader += "\n";
				RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\">\n";
				RadJav.Interact.App.defaultHeader += "\t\tRadJav.baseUrl = \"./RadJav\";\n";
				RadJav.Interact.App.defaultHeader += "\t\tRadJav.themeUrl = \"./RadJav/themes/default\";\n";
				RadJav.Interact.App.defaultHeader += "\t\tRadJav.useAjax = true;\n";
				RadJav.Interact.App.defaultHeader += "\n";
				RadJav.Interact.App.defaultHeader += "\t\tfunction setup ()\n";
				RadJav.Interact.App.defaultHeader += "\t\t{\n";
				RadJav.Interact.App.defaultHeader += "{{events}}";
				RadJav.Interact.App.defaultHeader += "\t\t}\n";
				RadJav.Interact.App.defaultHeader += "\t</script>\n";
				RadJav.Interact.App.defaultHeader += "\n";
				RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.Interact.js\"></script>\n";
				RadJav.Interact.App.defaultHeader += "{{jsFiles}}";
				RadJav.Interact.App.defaultHeader += "</head>\n";
				RadJav.Interact.App.defaultHeader += "\n";
				RadJav.Interact.App.defaultHeader += "<body onload = \"setup ();\">\n";

				RadJav.Interact.App.defaultFooter = "</body>\n";
				RadJav.Interact.App.defaultFooter += "</html>\n\n";
			}

			addView (view: View): void
			{
				if (view.name == "")
					throw new Error ("View must have a name!");

				if (view.name == null)
					throw new Error ("View must have a name!");

				this._views[view.name] = view;
			}

			setView (root: string | HTMLElement | RadJav.GUI.GObject, view: RadJav.Interact.View): void
			{
				this.addView (view);

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

				if (typeof (root) == "string")
					root = document.querySelector ((<string>root));

				for (let comp in view._components)
					(<HTMLElement>root).appendChild ((<RadJav.GUI.HTMLElement>view._components[comp].display)._html);
			}

			setComponentEvent (name: string, eventName: string, eventFunction: Function | EventListener): void
			{
				let elm = document.getElementById (name);

				for (let iIdx = 0; iIdx < elm.childNodes.length; iIdx++)
				{
					let child = elm.childNodes[iIdx];

					child.addEventListener (eventName, (<EventListener>eventFunction));
				}
			}

			buildHTMLApp (): HTMLApp
			{
				let app = new HTMLApp ();

				for (let view in this._views)
				{
					let html = this._views[view].buildHTML ();

					html.header = app.header;
					html.footer = app.footer;
					html.displays = app.displays;
					html.events = app.events;
					html.cssFiles = app.cssFiles;
					html.jsFiles = app.jsFiles;
					html._fileCopyFunc = app._fileCopyFunc;

					app._outputs.push (html);
				}

				return (app);
			}
		}

		/// A view that contains all the components.
		export class View
		{
			/// Name of the view.
			public name: string;
			/// The components in the view.
			public _components: { [name: string]: Component };
			/// The number of references counted.
			public _refCount: number;
			/// Events to execute for this view.
			public _events: { [eventName: string]: Function };
			/// The main component to show.
			public _mainComponent: Component;

			constructor (name: string)
			{
				if (name == null)
					throw new Error ("View must have a name!");

				if (name == "")
					throw new Error ("View must have a name!");

				this.name = name;
				this._components = {};
				this._refCount = 0;
				this._events = {};
			}

			/** @method createComponent
			* 
			*/
			createComponent (obj: RadJav.Interact.Component): RadJav.Interact.Component
			{
				if (obj.name == null)
					throw new Error ("Component must have a name!");

				if (obj.name == "")
					throw new Error ("Component must have a name!");

				if (this._components[obj.name] != null)
					throw new Error ("Component cannot have the same name as an existing component!");

				if (typeof obj.display == "string")
				{
					if (RadJav.OS.HTML5 != null)
						obj.display = new RadJav.GUI.HTMLElement (obj.name, obj.display);
				}

				if (obj["on"] == null)
					obj = new RadJav.Interact.Component (obj, this);

				this._components[obj.name] = obj;

				return (obj);
			}

			setMainComponent (component: string | Component): void
			{
				if (typeof component == "string")
				{
					component = this._components[component];

					if (component == null)
						throw new Error ("Component could not be found!");
				}

				this._mainComponent = component;
			}

			getMainComponent (): Component
			{
				return (this._mainComponent);
			}

			on (eventName: string, eventFunction: Function): any
			{
				this._events[eventName] = eventFunction;

				if (RadJav.OS.HTML5 != null)
				{
					if (eventName == "load")
						window.onload = (<EventListener>eventFunction);
					else
						document.addEventListener (eventName, (<EventListener>eventFunction));
				}

				return (null);
			}

			buildHTML (): HTMLOutput
			{
				let output = new HTMLOutput ();
				output.name = this.name;
				output.title = this.name;

				for (let evt in this._events)
				{
					let viewEvent = "";

					if (evt == "load")
						viewEvent = "(" + this._events[evt].toString () + ")()";
					else
						viewEvent = "document.addEventListener (\"" + evt + "\", " + this._events[evt].toString () + ");";

					output.events.push (viewEvent);
				}

				for (let comp in this._components)
				{
					let component = this._components[comp];
					let icomp = null;

					if (RadJav.OS.HTML5 != null)
						icomp = (<RadJav.GUI.GObject>component.display)._text;
					else
						icomp = "<div id = \"" + component.name + "\">" + component.display + "</div>";

					for (let evt in component._events)
					{
						let compEvent = "RadJav.Interact.setComponentEvent (\"" + component.name + "\", \"" + evt + "\", " + component._events[evt].toString () + ");";
						output.events.push (compEvent);
					}

					output.displays.push (icomp);
				}

				return (output);
			}
		}

		/// A component.
		export class Component
		{
			public name: string;
			public tag: string;
			public display: string | RadJav.GUI.GObject | RadJav.C3D.Object3D;
			public _data: { [name: string]: any };
			public _events: { [eventName: string]: Function };

			constructor (obj: Component, parentView: View)
			{
				if (obj.name == null)
					throw new Error ("Component must have a name!");

				if (obj.name == "")
					throw new Error ("Component must have a name!");

				this._data = {};
				this._events = {};

				if (obj["display"] != null)
				{
					if (typeof obj.display == "string")
					{
						this.parse ();

						if (RadJav.OS.HTML5 != null)
							obj.display = new RadJav.GUI.HTMLElement (obj.name, obj.display);

						parentView._refCount++;
					}
				}

				this.name = obj.name;
				this.tag = RadJav.setDefaultValue (obj.tag, "");
				this.display = RadJav.setDefaultValue (obj.display, null);

				let ignoreList = ["name", "tag", "display"];

				for (let key in obj)
				{
					let skip = false;

					for (let iIdx = 0; iIdx < ignoreList.length; iIdx++)
					{
						if (key == ignoreList[iIdx])
						{
							skip = true;

							break;
						}
					}

					if (skip == true)
						break;

					this._data[key] = Object.defineProperty (obj, key, {
								get: function ()
								{
									return (this[key]);
								}, 
								set: RadJav.keepContext (function (value, obj2)
								{
									let obj3 = obj2[0];
									let key2 = obj2[1];

									obj3[key2] = value;
								}, this, [obj, key])
							});
				}
			}

			parse (): void
			{
				//(<string>obj.display).replaceAll ("{{", "<div id = \"" + parentView.name + "-" + parentView._refCount + "\">");
				//(<string>obj.display).replaceAll ("}}", "</div>");
			}

			on (eventName: string, eventFunction: Function): any
			{
				this._events[eventName] = eventFunction;

				if (RadJav.OS.HTML5 != null)
					return ((<RadJav.GUI.GObject>this.display).on (eventName, eventFunction));

				return (null);
			}
		}

		/// The compiled HTML app.
		export class HTMLApp
		{
			public name: string;
			public title: string;
			public header: string;
			public displays: string[];
			public events: string[];
			public cssFiles: string[];
			public jsFiles: string[];
			public footer: string;
			public _assetsPath: string;
			public _fileCopyFunc: (src: string, dest: string) => boolean;

			public _outputs: HTMLOutput[];

			constructor ()
			{
				this.name = "";
				this.title = "";
				this.displays = [];
				this.events = [];
				this.cssFiles = [];
				this.jsFiles = [];
				this._assetsPath = "";
				this._fileCopyFunc = null;
				this._outputs = [];
			}

			assetsDir (path: string, fileCopyFunc: (src: string, dest: string) => boolean): void
			{
				this._assetsPath = path;
				this._fileCopyFunc = fileCopyFunc;
			}

			writeFiles (path: string): void
			{
				/// @todo Do tree shaking here, only copy over the files and directories that are needed.
				/// Also, if the files already exist in that directory, only copy over files that have been 
				/// changed.
				let files = RadJav.IO.listFiles (this._assetsPath);

				for (let iIdx = 0; iIdx < files.length; iIdx++)
				{
					let file = files[iIdx];
					let oldPath = RadJav.IO.normalizePath (this._assetsPath + "/" + file);
					let newPath = RadJav.IO.normalizePath (path + "/" + file);
					let result = true;

					if (this._fileCopyFunc != null)
					{
						result = this._fileCopyFunc (oldPath, newPath);

						if (result == null)
							result = true;
					}

					if (result == true)
					{
						if (RadJav.IO.isDir (oldPath) == true)
							RadJav.IO.copyDir (oldPath, newPath);
						else
							RadJav.IO.copyFile (oldPath, newPath);
					}
				}

				for (let iIdx = 0; iIdx < this._outputs.length; iIdx++)
				{
					let output = this._outputs[iIdx];
					output.writeToFile (path + "/" + output.name + ".htm");
				}
			}
		}

		/// A view that contains all the components.
		export class HTMLOutput
		{
			public name: string;
			public title: string;
			public header: string;
			public displays: string[];
			public events: string[];
			public cssFiles: string[];
			public jsFiles: string[];
			public footer: string;
			public _assetsPath: string;
			public _fileCopyFunc: (src: string, dest: string) => boolean;

			constructor ()
			{
				this.name = "";
				this.title = "";
				this.displays = [];
				this.events = [];
				this.cssFiles = [];
				this.jsFiles = [];
				this._assetsPath = "";
				this._fileCopyFunc = null;

				this.header = RadJav.Interact.App.defaultHeader;
				this.footer = RadJav.Interact.App.defaultFooter;
			}

			assetsDir (path: string, fileCopyFunc: (src: string, dest: string) => boolean): void
			{
				this._assetsPath = path;
				this._fileCopyFunc = fileCopyFunc;
			}

			writeToFile (path: string, includeDependencies: boolean = true): void
			{
				let output: string = "";
				let html: string = "";

				output += this.header;

				/*for (let iIdx = 0; iIdx < this.displays.length; iIdx++)
					html += "\t" + this.displays[iIdx] + "\n";

				let additions = ["events", "cssFiles", "jsFiles"];

				for (let iIdx = 0; iIdx < additions.length; iIdx++)
				{
					let additionType = additions[iIdx];
					let additionOutput = "";

					for (let iJdx = 0; iJdx < this[additionType].length; iJdx++)
					{
						if (additionType == "events")
							additionOutput += "\t\t\t" + this.events[iJdx] + "\n";

						if (additionType == "cssFiles")
							additionOutput += "\t<link href = \"" + this.cssFiles[iJdx] + "\" rel = \"stylesheet\" type = \"text/css\" />\n";

						if (additionType == "jsFiles")
							additionOutput += "\t<script type = \"text/javascript\" src = \"" + this.jsFiles[iJdx] + "\"></script>\n";
					}

					output = output.replaceAll ("{{" + additionType + "}}", additionOutput);
				}

				output += html;
				output += this.footer;*/

				RadJav.IO.TextFile.writeTextToFile (path, output);
			}
		}
	}
}

