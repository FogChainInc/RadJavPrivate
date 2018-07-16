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
		export function parseExpressions (expression: string): string[]
		{
			let expressions = [];
			let expBegin = expression.indexOf ("{{");
			let expEnd = expression.indexOf ("}}");

			while (expBegin > -1)
			{
				if (expEnd < 0)
					throw new Error ("Expression does not end properly!");

				let foundExp = expression.substr ((expBegin + 2), (expEnd - (expBegin + 2)));

				expressions.push (foundExp);

				expBegin = expression.indexOf ("{{", expBegin + 2);
				expEnd = expression.indexOf ("}}", expEnd + 2);
			}

			return (expressions);
		}

		/// The app to run.
		export class App
		{
			public name: string;
			public currentView: View;
			public _views: { [name: string]: View };
			public root: string | HTMLElement | RadJav.GUI.GObject;
			public options: { [optionName: string]: any };

			public html: CompiledHTML;
			public xrjApp: CompiledXRJApp;

			constructor (name: string)
			{
				this.name = name;
				this._views = {};
				this.html = null;
				this.xrjApp = null;
				this.root = null;
				this.options = {};
			}

			createView (name: string, root: string | HTMLElement | RadJav.GUI.GObject | RadJav.Interact.View = null): View
			{
				let view = new RadJav.Interact.View (this, name);

				if (root != null)
				{
					let counter = 0;

					// This is essentially the ES3 version of Object.keys(this._views).length
					for (let key in this._views)
					{
						counter++;

						break;
					}

					if (counter == 0)
						this.setView (root, view);
					else
						this.root = (<string | HTMLElement | RadJav.GUI.GObject>root);
				}
				else
					this.addView (view);

				return (view);
			}

			addView (view: View): void
			{
				view._app = this;
				this._views[view.name] = view;
			}

			setView (root: string | HTMLElement | RadJav.GUI.GObject | RadJav.Interact.View, view: RadJav.Interact.View): void
			{
				if (typeof (root) == "object")
				{
					if (root.constructor["name"] == "View")
						view = (<RadJav.Interact.View>root);
				}

				this.addView (view);
				this.currentView = view;

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

				if (root.constructor["name"] != "View")
					this.root = (<string | HTMLElement | RadJav.GUI.GObject>root);

				if (RadJav.OS.HTML5 != null)
				{
					if (this.options.useJQuery == null)
						this.options.useJQuery = false;

					if (this.options.bootstrap4 != null)
					{
						if (this.options.bootstrap4 == true)
							this.options.useJQuery = true;
					}

					if (typeof (root) == "string")
						root = (<HTMLElement>document.querySelector ((<string>root)));

					//(<RadJav.GUI.GObject>this.currentView._mainComponent.display).create ();
					if (RadJav.OS.HTML5 != null)
					{
						let htmlElm = (<RadJav.GUI.GObject>this.currentView._mainComponent.display)._html;

						(<HTMLElement>root).appendChild (htmlElm);
					}

					/// Go through each component and create the tag.
					for (let comp in view._components)
					{
					}
				}
			}

			setComponentEvent (name: string, eventName: string, eventFunction: Function | EventListener): void
			{
				let elm: HTMLElement = document.getElementById (name);

				for (let iIdx = 0; iIdx < elm.childNodes.length; iIdx++)
				{
					let child = elm.childNodes[iIdx];

					child.addEventListener (eventName, (<EventListener>eventFunction));
				}
			}

			build (newApp: CompiledApp = null): CompiledApp
			{
				if (this.currentView == null)
					throw new Error ("A current view has not been set!");

				let app: CompiledApp = newApp;

				if (app == null)
					app = new CompiledApp (this.name);

				app.mainView = this.currentView.name;
				app.compiledStr += "app.options = " + JSON.stringify (this.options) + "\n";

				if (this.root != null)
					app.root = this.root.toString ();

				for (let key in this._views)
				{
					let view: CompiledView = this._views[key].build (newApp);

					app.views.push (view);
				}

				return (app);
			}

			static loadApp (file: string | Function): void
			{
				if (typeof (file) == "string")
				{
					
					include ((<string>file));
				}
				else
					(<Function>file)();
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
			/// The app that contains this view.
			public _app: App;

			constructor (app: App, name: string)
			{
				if (app == null)
					throw new Error ("View must an app!");

				if (name == null)
					throw new Error ("View must have a name!");

				if (name == "")
					throw new Error ("View must have a name!");

				this.name = name;
				this._components = {};
				this._refCount = 0;
				this._events = {};
				this._mainComponent = null;
				this._app = app;
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

			build (newApp: CompiledApp): CompiledView
			{
				let view: CompiledView = new CompiledView (this.name, this._app.root.toString ());

				/// @todo Do component tree-shaking here? At minimum, give the compiler an option to do so.
				for (let compName in this._components)
				{
					let component: Component = this._components[compName];
					let componentStr: string = "";
					let css: string = "";
					let display: string = "";

					if (typeof (component.display) == "string")
						display = JSON.stringify (component.display);

					componentStr += "var " + compName + " = " + view.name + ".createComponent ({\n";
					componentStr += "\t\tname: \"" + component.name + "\", \n";
					componentStr += "\t\ttag: \"" + component.tag + "\", \n";
					componentStr += "\t\tdisplay: " + display + "\n";
					componentStr += "\t});\n";

					for (let eventName in component._events)
						componentStr += compName + ".on (\"" + eventName + "\", " + component._events[eventName].toString () + ");\n";

					view.components[compName] = componentStr;

					if (component.css != null)
					{
						for (let iIdx = 0; iIdx < component.css.length; iIdx++)
						{
							let compCSS = component.css[iIdx];

							if (typeof (compCSS) == "string")
								css += RadJav.IO.TextFile.readEntireFile (compCSS) + "\n";

							if (typeof (compCSS) == "object")
								css += compCSS.css + "\n";
						}
					}

					if (css != "")
					{
						view.compiledHTMLCSS += "/* --- View " + this.name + ", Component " + compName + " CSS Begin --- */\n";
						view.compiledHTMLCSS += css;
						view.compiledHTMLCSS += "/* --- View " + this.name + ", Component " + compName + " CSS End --- */\n";
					}
				}

				if (this._mainComponent != null)
					view.mainComponentName = this._mainComponent.name;

				return (view);
			}
		}

		/// A component.
		export class Component
		{
			public name: string;
			public tag: string;
			public display: string | RadJav.GUI.GObject | RadJav.C3D.Object3D;
			public css: string[] | { css: string }[];
			public _data: { [name: string]: ComponentData };
			public _events: { [eventName: string]: Function | string };

			constructor (obj: Component, parentView: View)
			{
				if (obj.name == null)
					throw new Error ("Component must have a name!");

				if (obj.name == "")
					throw new Error ("Component must have a name!");

				if (parentView == null)
					throw new Error ("Component must have a parent view!");

				if (obj["display"] != null)
				{
					if (typeof obj.display == "string")
					{
						if (RadJav.OS.HTML5 != null)
						{
							if (parentView._app.options.useJQuery == true)
								obj.display = $(obj.display);

							obj.display = new RadJav.GUI.HTMLElement (obj.name, obj.display);
						}

						parentView._refCount++;
					}
				}

				this.name = obj.name;
				this.tag = RadJav.setDefaultValue (obj.tag, "");
				this.display = RadJav.setDefaultValue (obj.display, null);
				this.css = RadJav.setDefaultValue (obj.css, []);
				this._data = RadJav.setDefaultValue (obj._data, {});
				this._events = RadJav.setDefaultValue (obj._events, {});

				if (RadJav.OS.HTML5 != null)
				{
					if (this.tag != "")
					{
						var elm = document.createElement (this.tag);

						elm.innerHTML = (<RadJav.GUI.GObject>this.display)._html;
					}
				}

				let ignoreList = ["name", "tag", "display", "css"];

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
									return (this[key].value);
								}, 
								set: RadJav.keepContext (function (value, obj2)
								{
									let obj3 = obj2[0];
									let key2 = obj2[1];
									let parentView2 = obj2[2];

									obj3[key2] = new ComponentData ();
									obj3[key2].name = parentView2._app.name + "-" + parentView2.name + "-" + parentView2._refCount;
									obj3[key2].value = value;
								}, this, [obj, key, parentView])
							});
				}

				this.parse ();
			}

			parse (): void
			{
				if (this.display == null)
					return;

				let beginningBraces = "";
				let endBraces = "";
				let str = "";
				let compName = "";

				if (typeof this.display == "string")
				{
					str = (<string>this.display);
					beginningBraces = "<div id = \"" + compName + "\">";
					endBraces = "</div>";
				}
				else
				{
					if (this.display.constructor["name"] == "GObject")
						str = (<RadJav.GUI.GObject>this.display).getText ();

					/*if (this.display.constructor["name"] == "Object3D")
						str = (<RadJav.C3D.Object3D>this.display).getText ();*/
				}

				let expressions: string[] = RadJav.Interact.parseExpressions (str);

				for (let iIdx = 0; iIdx < expressions.length; iIdx++)
				{
					
				}
			}

			on (eventName: string, eventFunction: Function): any
			{
				this._events[eventName] = eventFunction;

				if (RadJav.OS.HTML5 != null)
					return ((<RadJav.GUI.GObject>this.display).on (eventName, eventFunction));

				return (null);
			}
		}

		/// Data that is stored by a component.
		export class ComponentData
		{
			public name: string;
			public value: any;

			constructor ()
			{
				this.name = "";
				this.value = null;
			}
		}

		/// The compiled HTML.
		export class CompiledHTML
		{
			/// The file name to output.
			public filename: string;
			/// The title of the HTML page.
			public title: string;
			public interactAppPath: string;
			/// HTML events to fire.
			public htmlEvents: string[];
			/// The HTML body to display.
			public body: string;
			/// CSS files to use.
			public cssFiles: string[];
			/// JavaScript files to use.
			public jsFiles: string[];

			/// The HTML page to output.
			public html: string;

			constructor (name: string)
			{
				this.filename = name + ".htm";
				this.title = name;
				this.interactAppPath = "./" + name + ".js";
				this.htmlEvents = [];
				this.cssFiles = [];
				this.jsFiles = ["./" + name + ".xrj"];
				this.body = "";

				this.html = "<!DOCTYPE html>\n";
				this.html += "<html>\n";
				this.html += "<head>\n";
				this.html += "\t<title>{{title}}</title>\n";
				this.html += "\t<link href = \"./RadJav/themes/default/dojo/themes/claro/claro.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				this.html += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/Grid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				this.html += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/claroGrid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				this.html += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/Calendar.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				this.html += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/MonthColumnView.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				this.html += "\t<link href = \"./RadJav/themes/default/dojo/resources/FloatingPane.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				this.html += "\t<link href = \"./RadJav/themes/default/dojo/resources/ResizeHandle.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
				this.html += "{{cssFiles}}";
				this.html += "\t<script type = \"text/javascript\" src = \"./dependencies/three.js/three.min.js\"></script>\n";
				this.html += "\t<script type = \"text/javascript\" src = \"./dependencies/dojo/dojo/dojo.js\"></script>\n";
				this.html += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.js\"></script>\n";
				this.html += "\n";
				this.html += "\t<script type = \"text/javascript\">\n";
				this.html += "\t\tRadJav.baseUrl = \"./RadJav\";\n";
				this.html += "\t\tRadJav.themeUrl = \"./RadJav/themes/default\";\n";
				this.html += "\t\tRadJav.useAjax = true;\n";
				this.html += "\n";
				this.html += "\t\tfunction setup ()\n";
				this.html += "\t\t{\n";
				this.html += "{{htmlEvents}}";
				this.html += "\t\t}\n";
				this.html += "\t</script>\n";
				this.html += "\n";
				this.html += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.Interact.js\"></script>\n";
				this.html += "{{jsFiles}}";
				this.html += "</head>\n";
				this.html += "\n";
				this.html += "<body onload = \"setup ();\">\n";
				this.html += "{{body}}\n";
				this.html += "</body>\n";
				this.html += "</html>\n\n";
			}

			build (): string
			{
				let output = this.html;
				let additions = ["title", "body", "interactAppPath", "htmlEvents", "cssFiles", "jsFiles"];

				for (let iIdx = 0; iIdx < additions.length; iIdx++)
				{
					let additionType = additions[iIdx];
					let additionOutput = "";

					if (typeof (this[additionType]) != "string")
					{
						for (let iJdx = 0; iJdx < this[additionType].length; iJdx++)
						{
							if (additionType == "htmlEvents")
								additionOutput += "\t\t\t" + this.htmlEvents[iJdx] + "\n";

							if (additionType == "cssFiles")
								additionOutput += "\t<link href = \"" + this.cssFiles[iJdx] + "\" rel = \"stylesheet\" type = \"text/css\" />\n";

							if (additionType == "jsFiles")
								additionOutput += "\t<script type = \"text/javascript\" src = \"" + this.jsFiles[iJdx] + "\"></script>\n";
						}
					}
					else
						additionOutput = this[additionType];

					output = output.replaceAll ("{{" + additionType + "}}", additionOutput);
				}

				return (output);
			}

			writeToFile (path: string): void
			{
				let output = this.build ();
				RadJav.IO.TextFile.writeTextToFile (path, output);
			}
		}

		/// The compiled XRJ app.
		export class CompiledXRJApp
		{
			public filename: string;
			public name: string;
			public version: string;
			public interactAppPath: string;
			public xrjApp: string;
			public generateAsHTML5App: boolean;
			/// GUI events to fire.
			public guiEvents: string[];
			
			constructor (name: string)
			{
				this.filename = name + ".xrj";
				this.name = name;
				this.version = "1.0";
				this.interactAppPath = "./" + name + ".js";
				this.guiEvents = [];
				this.generateAsHTML5App = true;

				this.xrjApp = "var appName = \"{{name}}\";\n";
				this.xrjApp += "var version = \"{{version}}\";\n";
				this.xrjApp += "\n";
				this.xrjApp += "function onReady ()\n";
				this.xrjApp += "{\n";
				this.xrjApp += "\tRadJav.initialize (RadJav.getStandardLibrary (), RadJav.getGUILibrary ()).then (function ()\n";
				this.xrjApp += "\t\t{\n";
				this.xrjApp += "\t\t\tRadJav.runApplication (function ()\n";
				this.xrjApp += "\t\t\t\t{\n";

				if (this.generateAsHTML5App == true)
				{
				}

				this.xrjApp += "\t\t\t\t\tRadJav.Interact.App.loadApp (\"{{interactAppPath}}\");\n";
				this.xrjApp += "\t\t\t\t});\n";
				this.xrjApp += "\t\t});\n";
				this.xrjApp += "}\n";
				this.xrjApp += "\n";
				this.xrjApp += "RadJav.OS.onReady (onReady);\n";
				this.xrjApp += "\n\n";
			}

			build (): string
			{
				let output = this.xrjApp;
				let additions = ["name", "version", "interactAppPath", "guiEvents"];

				for (let iIdx = 0; iIdx < additions.length; iIdx++)
				{
					let additionType = additions[iIdx];
					let additionOutput = "";

					if (typeof (this[additionType]) != "string")
					{
						for (let iJdx = 0; iJdx < this[additionType].length; iJdx++)
						{
							if (additionType == "guiEvents")
								additionOutput += "\t\t\t" + this.guiEvents[iJdx] + "\n";
						}
					}
					else
						additionOutput = this[additionType];

					output = output.replaceAll ("{{" + additionType + "}}", additionOutput);
				}

				return (output);
			}

			writeToFile (path: string): void
			{
				let output = this.build ();
				RadJav.IO.TextFile.writeTextToFile (path, output);
			}
		}

		/// The compiled app.
		export class CompiledApp
		{
			public name: string;
			public html: CompiledHTML;
			public xrjApp: CompiledXRJApp;
			public views: CompiledView[];
			public mainView: string;
			public root: string;

			public compiledStr: string;

			public _assetsPath: string;
			public _fileCopyFunc: (src: string, dest: string) => boolean;

			constructor (name: string)
			{
				this.name = name;
				this.html = new CompiledHTML (name);
				this.xrjApp = new CompiledXRJApp (name);
				this.views = [];
				this.compiledStr = "var app = new RadJav.Interact.App (\"" + this.name + "\");\n";
				this.mainView = "";
				this.root = "";

				this._assetsPath = "";
				this._fileCopyFunc = null;
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

				let output: string = this.compiledStr + "\n";

				for (let iIdx = 0; iIdx < this.views.length; iIdx++)
				{
					let view: CompiledView = this.views[iIdx];

					view.writeHTMLCSSToFile (path + "/" + view.name + ".css");
					output += view.build ();
				}

				output += "app.setView (\"" + this.root + "\", " + this.mainView + ");\n";

				RadJav.IO.TextFile.writeTextToFile (path + "/" + this.name + ".js", output);

				if (this.html != null)
					this.html.writeToFile (path + "/" + this.html.filename);

				if (this.xrjApp != null)
					this.xrjApp.writeToFile (path + "/" + this.xrjApp.filename);
			}
		}

		/// A compiled view.
		export class CompiledView
		{
			public name: string;
			public compiledHTMLCSS: string;
			public components: { [name: string]: string };
			public mainComponentName: string;

			public compiledStr: string;

			constructor (name: string, root: string)
			{
				this.name = name;
				this.compiledHTMLCSS = "";
				this.components = {};
				this.mainComponentName = "";
				this.compiledStr = "var " + this.name + " = app.createView (\"" + this.name + "\", \"" + root + "\");\n";
			}

			build (): string
			{
				let output: string = this.compiledStr + "\n";

				for (let key in this.components)
				{
					let comp: string = this.components[key];

					output += comp;
				}

				if (this.mainComponentName != "")
					output += this.name + ".setMainComponent (" + this.mainComponentName + ");\n";

				return (output);
			}

			writeHTMLCSSToFile (path: string): void
			{
				if (this.compiledHTMLCSS != "")
					RadJav.IO.TextFile.writeTextToFile (path, this.compiledHTMLCSS);
			}

			writeToFile (path: string): void
			{
				let output: string = this.build ();
				RadJav.IO.TextFile.writeTextToFile (path, output);
			}
		}
	}
}

