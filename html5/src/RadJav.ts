/*
	MIT-LICENSE
	Copyright (c) 2015 Higher Edge Software, LLC

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
declare let define: any;

/// <reference path="RadJav.Animation.ts" />

/** @class Promise
 * An object that executes when a process has completed.
 */
 
// Hack for the desktop/mobile versions.
if (typeof (window) == "undefined")
{
	interface window
	{
		innerWidth: number;
		innerHeight: number;
	}
}

/** @class RadJav
 * @static
 * The main object that starts RadJav.
 */
namespace RadJav
{
	/// Allow the use of eval.
	export let defaults: any;

	/// Allow the use of eval.
	export let useEval: boolean = true;

	/// The minimum version of code that can be ran.
	export let MIN_VERSION: number = 0.05;

	/// The current version.
	export let VERSION: number = 0.05;

	/** @property {String} [baseUrl="./RadJav"]
	* The url to the directory where RadJav is located.
	*/
	export let baseUrl: string = "./RadJav/";

	/** @property {String} [themeUrl="./RadJav/themes/default"]
	* The url to the directory where the theme will be loaded.
	*/
	export let themeUrl: string = "./RadJav/themes/default";

	/** @property {String} [selectedLanguage="en_us"]
	* The selected language.
	*/
	export let selectedLanguage: string = "en_us";

	/** @property {RadJav.Theme} [currentTheme=null]
	* The current theme that has been loaded.
	*/
	export let currentTheme: Theme = null;

	/** @property {Boolean} [_isInitialized=false]
	* If set to true, RadJav has been initialized.
	*/
	export let _isInitialized: boolean = false;

	/** @property {String[]} [_included=[]]
	* If set to true, RadJav has been initialized.
	*/
	export let _included: any[] = [];

	/** @property {String[]} [_lang=[]]
	* If set to true, RadJav has been initialized.
	*/
	export let _lang: { [key: string]: any } = {};

	/** @property {Object} [themeUtils={}]
	* Miscellaneous theme utilities to use.
	*/
	export let themeUtils: any = {};

	/** @property {Boolean} [useAjax=true]
	* If set to true, each file loaded by RadJav will use ajax.
	*/
	export let useAjax: boolean = true;

	/** @property {Boolean} [isMinified=false]
	* Is set to true if RadJav has been minified.
	*/
	export let isMinified: boolean = false;

	/** @property {RadJav.Animation[]} [_animations=[]]
	* Any animations that are playing.
	*/
	export let _animations: RadJav.Animation[] = [];

	/** @property {number} [animationFrameRate=16]
	* The frame rate for how often the animation update is called.
	*/
	export let animationFrameRate: number = 16;

	/** @property {number} [prevTime=0]
	* The previous time since last the last update.
	*/
	export let prevTime: number = (Date.now () / 1000);

	/** @property {RadJav.OS.ScreenInfo[]} [screens=[]]
	* The screens attached to this device.
	*/
	export let screens: RadJav.OS.ScreenInfo[] = [];

	/** 
	* Exit the application.
	* Available on platforms: Windows,Linux,OSX
	* @param {Number} [exitCode=0] The exit code to end the application with.
	*/
	export function quit(exitCode: number = 0) {}

	/** 
	* Exit the application.
	* Available on platforms: Windows,Linux,OSX
	* @param {Number} [exitCode=0] The exit code to end the application with.
	*/
	export function exit(exitCode: number) {}

	/** 
	* Load and return a module. If the module has not already been loaded, this will create
	* an asynchronous connection to the server and include whatever javascript files it needs.
	* @param {String} path The path to the module to load.
	* @return {Promise} The promise containing the loaded module.
	*/
	export function include(path: string): Promise<any>
	{
		var promise: Promise<string> = null;

		if (RadJav.OS.HTML5 == null)
			return (null);

		if (RadJav.useAjax == true)
		{
			promise = RadJav._getResponse(path).then(
				RadJav.keepContext(function(response)
					{
						if (response != null)
						{
							if (response != "")
								_eval(response);
						}
					}, this));
		}
		else
		{
			promise = new Promise<string>(
				RadJav.keepContext(function(resolve, reject)
				{
					var script:any = document.createElement("script");
					script.type = "text/javascript";
					//script.async = false;
					//script.defer = false;
					var str = "";

					if (RadJav._isUsingInternetExplorerTheWorstWebBrowserEver() == true)
						script.text = str;
					else
					{
						var textNode = document.createTextNode(str);
						script.appendChild(textNode);
					}

					script.onreadystatechange = RadJav.keepContext(
							function(evt, script2)
							{
								var s = script2[0];

								if (s.readyState == null)
									s.readyState = "complete";

								if (s.readyState == "complete")
									resolve();

							}, this, [script]);

					script.onload = script.onreadystatechange;

					script.onerror = RadJav.keepContext(function(err)
						{
							throw RadJav.getLangString("errorWhileIncludingFile", err.message, path);
						}, this);

					script.src = path;

					document.documentElement.insertBefore(script, document.documentElement.firstChild);
				}, this));
		}

		return promise;
	}

	/** 
	* Initialize RadJav.
	* @param {Object[]} [libraries=null] The libraries to include.
	* @return {Promise} The promise to execute.
	*/
	export function initialize(libraries: { [key: string]: any }[] = null): Promise<void>
	{
		var promise = new Promise<void>(
			RadJav.keepContext(function(resolve, reject, args)
			{
				if (RadJav._isInitialized == true)
				{
					resolve();

					return;
				}

				var promises = [];
				promises.push(RadJav._loadLanguages());

				if (libraries == null || libraries.length == 0)
				{
					promises.push(RadJav.includeLibraries(RadJav.getStandardLibrary()));
					promises.push(RadJav.includeLibraries(RadJav.getGUILibrary()));
				}

				if (libraries != null)
				{
					if (args.length > 1)
					{
						for (var iIdx = 1; iIdx < args.length; iIdx++)
						{
							var tempArg = args[iIdx];

							for (var iJdx = 0; iJdx < tempArg.length; iJdx++)
							{
								libraries.push(tempArg[iJdx]);
							}
						}
					}

					promises.push(RadJav.includeLibraries(libraries));
				}

				promises.push(RadJav._loadTheme(RadJav.themeUrl));

				Promise.all(promises).then(function()
					{
						RadJav._isInitialized = true;

						if (RadJav.useEval == false)
						{
							var evalu = function()
							{
								var msg = "RadJav disables eval by default. Set RadJav.useEval = true; to enable it.";
								alert(msg);
								throw msg;
							};
						}

						resolve();
					});
			}, RadJav, arguments));

		return promise;
	}

	/** 
	* Get the paths to the standard library.
	* @return {Object[]} The standard library.
	*/
	export function getStandardLibrary(): {
			file: string;
			themeFile: boolean;
			loadFirst?: boolean;
		}[]
	{
		var includes = [
				{ file: "RadJav.Circle", themeFile: false },
				{ file: "RadJav.Rectangle", themeFile: false },
				{ file: "RadJav.Vector2", themeFile: false },
				{ file: "RadJav.Vector3", themeFile: false },
				{ file: "RadJav.Color", themeFile: false },
				{ file: "Math", themeFile: false, loadFirst: true },
				{ file: "String", themeFile: false, loadFirst: true }
			];

		return includes;
	}

	/** 
	* Get the paths to the gui library.
	* @return {Object[]} The gui library.
	*/
	export function getGUILibrary(): {
			file: string;
			themeFile: boolean;
			loadFirst?: boolean;
		}[]
	{
		var includes = [
				{ file: "RadJav.GUI.GObject", themeFile: true, loadFirst: true },
				{ file: "RadJav.Font", themeFile: false, loadFirst: true },
				{ file: "RadJav.Animation", themeFile: false, loadFirst: true },
				{ file: "RadJav.GUI.Window", themeFile: true },
				{ file: "RadJav.GUI.MenuBar", themeFile: true },
				{ file: "RadJav.GUI.MenuItem", themeFile: true },
				{ file: "RadJav.GUI.Button", themeFile: true },
				{ file: "RadJav.GUI.Textbox", themeFile: true },
				{ file: "RadJav.GUI.Checkbox", themeFile: true },
				{ file: "RadJav.GUI.Radio", themeFile: true },
				{ file: "RadJav.GUI.List", themeFile: true },
				{ file: "RadJav.GUI.Image", themeFile: true },
				{ file: "RadJav.GUI.Label", themeFile: true },
				{ file: "RadJav.GUI.Container", themeFile: true },
				{ file: "RadJav.GUI.HTMLElement", themeFile: true },
				{ file: "RadJav.GUI.Combobox", themeFile: true },
				{ file: "RadJav.GUI.Textarea", themeFile: true }, 
				{ file: "RadJav.MUI.ScrollView", themeFile: true }, 
				{ file: "RadJav.MUI.Navigator", themeFile: true }, 
				{ file: "RadJav.MUI.View", themeFile: true }
			];

		return includes;
	}

	/** 
	* Get the paths to the MUI library.
	* @return {Object[]} The MUI library.
	*/
	export function getMUILibrary(): {
			file: string;
			themeFile: boolean;
			loadFirst?: boolean;
		}[]
	{
		var includes = [
				{ file: "RadJav.GUI.GObject", themeFile: true, loadFirst: true },
				{ file: "RadJav.Font", themeFile: false, loadFirst: true },
				{ file: "RadJav.MUI.View", themeFile: true },
				{ file: "RadJav.GUI.Button", themeFile: true }
			];

		return includes;
	}

	/** 
	* Get the paths to the C3D library.
	* @return {Object[]} The C3D library.
	*/
	export function getC3DLibrary(): {
			file: string;
			themeFile: boolean;
			loadFirst?: boolean;
		}[]
	{
		var includes = [
				{ file: "RadJav.GUI.Window", themeFile: true },
				{ file: "RadJav.GUI.Canvas3D", themeFile: true },
				{ file: "RadJav.C3D.Object3D", themeFile: false, loadFirst: true },
				{ file: "RadJav.GUI.GObject", themeFile: false, loadFirst: true },
				{ file: "RadJav.Font", themeFile: false },
				{ file: "RadJav.C3D.Camera", themeFile: false },
				{ file: "RadJav.C3D.Entity", themeFile: false },
				{ file: "RadJav.C3D.Transform", themeFile: false },
				{ file: "RadJav.Vector3", themeFile: false },
				{ file: "RadJav.Vector4", themeFile: false },
				{ file: "RadJav.Quaternion", themeFile: false },
				{ file: "RadJav.C3D.Model", themeFile: false, loadFirst: false },
				{ file: "RadJav.C3D.Material", themeFile: false, loadFirst: false }
			];

		return includes;
	}

	/** 
	* Get the paths to the Net library.
	* @return {Object[]} The Net library.
	*/
	export function getNetLibrary():
		{ file: string; themeFile: boolean }[]
	{
		var includes = [{ file: "RadJav.Net.WebSocketClient", themeFile: false }];

		return includes;
	}

	/** 
	* Get the primary screen being used.
	* @return {RadJav.OS.ScreenInfo} The screen being used.
	*/
	export function getPrimaryScreen(): RadJav.OS.ScreenInfo
	{
		return (this.screens[0]);
	}

	/** 
	* Add a screen.
	* @param {RadJav.OS.ScreenInfo} The screen to add.
	*/
	export function addScreen(screen: RadJav.OS.ScreenInfo): void
	{
		this.screens.push (screen);
	}

	/** 
	* Setup the screens used on this device.
	*/
	export function setupScreens(): void
	{
		if (RadJav.OS.type == "html5")
			RadJav.screens.push (RadJav.OS.ScreenInfo.getScreenInfo (0));
	}

	/** 
	* Connect to the developer tools to help build/debug RadJav apps.
	* @param {string} [server="ws://127.0.0.1:8585/"] The WebSocket server url to connect to.
	*/
	export function connectDevTools(server: string = "ws://127.0.0.1:8585/", showMessage: boolean = true): void
	{
		var displayMessageVis = function (msg, visibility)
			{
				var visible = "visible";

				if (visibility == false)
					visible = "hidden";

				var dialog = document.getElementById ("dialog");
				dialog.style.visibility = visible;

				var message = document.getElementById ("message");
				message.innerHTML = msg;
			};
		var displayMessage = function (msg)
			{
				displayMessageVis (msg, true);
			};

		var html = "<div id = \"dialog\" style = \"position: absolute; background-color: #000000; ";
		html += "color: #FFFFFF; width: 100%; height: 100%; visibility: visible;\">";
		html += "<div id = \"message\">";
		html += "Connecting to server...";
		html += "</div>";
		html += "</div>";

		if (showMessage == true)
			RadJav.OS.HTML5.appendHTML (document.body, html);

		var msg = "Connecting to web socket server at: " + server + "<br />\n";
		displayMessage (msg);

		var webSocket = new WebSocket (server);
		webSocket.onopen = function ()
			{
				displayMessageVis ("", false);
			};
		webSocket.onerror = function (error)
			{
				displayMessage ("Unable to connect to web socket server.");
			};
		webSocket.onmessage = function (message)
			{
				if (message.data == "refresh")
					location.reload ();
			};
		webSocket.onclose = function ()
			{
				displayMessage ("Disconnected from WebSocket server. Please refresh the page to reconnect.");
			};
	}

	/** 
	 * Include libraries.
	 * @param {Object[]} libraries The libraries to include.
	 * @return {Promise} The promise to execute when the including has completed.
	 */
	export function includeLibraries(libraries): Promise<void>
	{
		for (var iIdx = 0; iIdx < libraries.length; iIdx++)
			RadJav._included.push (libraries[iIdx]);

		var promise = new Promise<void> (RadJav.keepContext (function (resolve, reject)
					{
						if (RadJav.OS.HTML5 == null)
						{
							resolve ();

							return;
						}

						var promises = [];

						for (var iIdx = 0; iIdx < RadJav._included.length; iIdx++)
						{
							var includeObj = RadJav._included[iIdx];

							if (typeof (includeObj) != "string")
							{
								if (includeObj.loadFirst == true)
								{
									var file = "";
									var shouldIncludeFile = false;

									if (typeof (includeObj) != "string")
									{
										if (typeof (includeObj.file) == "string")
											file = includeObj.file;
									}
									else
										file = includeObj;

									if (_eval ("if (" + file + " != null){true;}else{false;}") == false)
										shouldIncludeFile = true;

									if (RadJav.isMinified == false)
									{
										if ((file == "Math") || (file == "String"))
											shouldIncludeFile = true;
									}

									if (shouldIncludeFile == true)
									{
										var includeFile = RadJav.baseUrl + "/" + file + ".js";
										promises.push (RadJav.include (includeFile));
									}
								}
							}
						}

						Promise.all (promises).then (function ()
							{
								var promises2 = [];

								for (var iIdx = 0; iIdx < RadJav._included.length; iIdx++)
								{
									var includeObj = RadJav._included[iIdx];
									var file = "";
									var shouldIncludeFile = false;

									if (typeof (includeObj) != "string")
									{
										if (typeof (includeObj.file) == "string")
											file = includeObj.file;
									}
									else
										file = includeObj;

									if (_eval ("if (" + file + " != null){true;}else{false;}") == false)
										shouldIncludeFile = true;

									if (RadJav.isMinified == false)
									{
										if ((file == "Math") || (file == "String"))
											shouldIncludeFile = true;
									}

									if (shouldIncludeFile == true)
									{
										var includeFile = RadJav.baseUrl + "/" + file + ".js";
										promises2.push (RadJav.include (includeFile));
									}
								}

								Promise.all (promises2).then (function ()
									{
										resolve ();
									});
							});
					}, RadJav));

		return (promise);
	}

	/** 
	 * Load the selected language.
	 * @return {Promise} The promise to execute when the language has been loaded.
	 */
	export function _loadLanguages(): Promise<any>
	{
		var promise = new Promise(function(resolve, reject)
			{
				if (RadJav.OS.HTML5 != null)
				{
					RadJav._getResponse(
							RadJav.baseUrl + "/languages/" + RadJav.selectedLanguage + ".json"
						).then(function(data)
							{
								RadJav._lang = JSON.parse(data);
								resolve();
							});
				}
				else
					resolve();
			});

		return promise;
	}

	/** 
	 * Load a theme.
	 * @param {String} themeURL The URL to the theme to load.
	 */
	export function _loadTheme(themeURL: string): Promise<any>
	{
		var url = themeURL;

		if (url[themeURL.length - 1] == "/")
			url = url.substr(0, themeURL.length - 1);

		var promise = new Promise(function(resolve, reject)
			{
				if (RadJav.OS.HTML5 == null)
				{
					resolve ();

					return;
				}

				if (RadJav.useAjax == true)
				{
					RadJav._getResponse(url + "/theme.json").then(function(data)
						{
							var jsonData = JSON.parse (data);
							var theme = RadJav.Theme.loadTheme(url, jsonData);
							RadJav.currentTheme = theme;
							var promises2 = [];

							promises2.push(RadJav.currentTheme.loadInitializationFile());
							promises2.push(RadJav.currentTheme.loadJavascriptFiles());

							Promise.all(promises2).then(function()
								{
									resolve();
								});
						});
				}
				else
				{
					var theme = RadJav.Theme.loadTheme(url, RadJav.currentTheme);
					RadJav.currentTheme = theme;
					var promises2 = [];

					promises2.push(RadJav.currentTheme.loadInitializationFile());
					promises2.push(RadJav.currentTheme.loadJavascriptFiles());

					Promise.all(promises2).then(function() {
						resolve();
					});
				}
			});

		return promise;
	}

	/** 
	* Run an application from a file or a function.
	* Available on platforms: Windows,Linux,OSX,HTML5
	* @param {String/Function} file The path to the file to execute the javascript. Or a
	* function that will immediately be executed.
	* @return {Promise} The promise that will be executed when this module has completed executing.
	*/
	export function runApplication(file: string | Function, createRootGObj: boolean = false): Promise<any>
	{
		let promise = RadJav.initialize ().then (RadJav.keepContext (function ()
			{
				let promise = null;
				let rootGObj: RadJav.GUI.GObject = null;

				if (createRootGObj == true)
				{
					/// @fixme This is done both synchronously and asynchronously. Fix this.
					if (RadJav.isMobile () == true)
					{
						rootGObj = new RadJav.MUI["View"] ("win", "Navigator example");
						promise = rootGObj["createMainView"] ();

						if (promise == null)
						{
							// Temporary hack.
							promise = new Promise<void> (function (resolve, reject)
								{
									resolve ();
								});
						}
					}
					else
					{
						rootGObj = new RadJav.GUI.Window ("win", "Window Example");
						promise = rootGObj.create ();
					}
				}
				else
				{
					promise = new Promise<void> (function (resolve, reject)
						{
							resolve ();
						});
				}

				promise.then (RadJav.keepContext(function (createdGObj)
					{
						if (typeof file == "string")
						{
							promise = RadJav.include(file).then(
								RadJav.keepContext(function(data)
								{
									let func = new _Function(data);
									func(createdGObj);
								}, this));
						}
						else
						{
							promise = new Promise(RadJav.keepContext(
								function(resolve, reject, func)
								{
									func(createdGObj);
									resolve();
								}, this, file));
						}
					}, this));

				return (promise);
			}, this));

		return promise;
	}

	/// An alias for runApplication
	export function runApp(file: string | Function, createRootGObj: boolean = false): Promise<any>
	{
		return (RadJav.runApplication (file, createRootGObj));
	}

	/** 
	* Run an application from a file.
	* Available on platforms: Windows,Linux,OSX,HTML5
	* @param {String} file The path to the file to execute the javascript. Or a
	* function that will immediately be executed.
	* @return {Promise} The promise that will be executed when this module has completed executing.
	*/
	export function runApplicationFromFile(file: string): Promise<void>
	{
		return RadJav.runApplication(file);
	}

	/** 
	* Load RadJav objects.
	* Available on platforms: Windows,Linux,OSX,HTML5
	* @param {String/RadJav.GUI.GObject[]/RadJav.C3D.Object3D[]} objs The objects to load.
	* @return {Promise} When loading has completed, all loaded objects will be passed into
	* the "then" function as an object with key/value pairs.
	*/
	export function loadObjects(objs: any[]): Promise<{ [key: string]: any }>
	{
		let promise = new Promise(function(resolve, reject)
			{
				let promises = [];
				let createdObjs = {};

				if (typeof objs == "string")
					objs = JSON.parse(objs);

				for (let iIdx = 0; iIdx < objs.length; iIdx++)
				{
					let obj = objs[iIdx];
					let type = obj.type;
					let name = obj.name;
					let createdObj = null;
					let promise2 = null;

					if (type.indexOf(".GUI") > -1)
					{
						createdObj = new RadJav.GUI[type](obj);
						promise2 = createdObj.create();
					}

					if (type.indexOf(".C3D") > -1)
					{
						createdObj = new RadJav.C3D[type](obj);
						promise2 = createdObj.create();
					}

					if (createdObj != null)
						createdObjs[name] = createdObj;

					if (promise2 != null)
						promises.push(promise2);
				}

				Promise.all(promises).then(function()
					{
						resolve(createdObjs);
					});
			});

		return promise;
	}

	/** 
	* Load RadJav objects from XML.
	* Available on platforms: Windows,Linux,OSX,HTML5
	* @param {String} xml The XML to load.
	* @return {Promise} When loading has completed, all loaded objects will be passed into
	* the "then" function as an object with key/value pairs.
	*/
	export function loadXML(xml: string): Promise<any>
	{
		let promise = new Promise<any> (function (resolve, reject)
			{
				let file: RadJav.XML.XMLFile = RadJav.XML.XMLFile.loadString (xml);
				let tag: string = file.root.tag.toLowerCase ();
				let promises: Promise<any>[] = [];

				if (tag == "radjav")
				{
					let apps: RadJav.XML.XMLTag[] = file.root.getTags ("app");

					for (let iIdx = 0; iIdx < apps.length; iIdx++)
					{
						let app: RadJav.XML.XMLTag = apps[iIdx];
						let name: string = app.getAttributeString ("name", "mainWindow");
						let platform: string = app.getAttributeString ("platform", "");
						let html5parent: string = app.getAttributeString ("html5parent", "body");
						let autoCreateWindow: boolean = app.getAttributeBoolean ("autoCreateWindow", true);
						let newWindow: HTMLElement | RadJav.GUI.GObject = null;
						let appPromise: Promise<RadJav.GUI.GObject> = null;

						if (autoCreateWindow == true)
						{
							if (typeof (RadJav.OS.HTML5) != "undefined")
							{
								let elm: RadJav.GUI.HTMLElement = new RadJav.GUI.HTMLElement (name, html5parent);

								appPromise = elm.create ().then (function (mainWindow)
															{
																return (mainWindow);
															});
							}
							else
							{
								let win: RadJav.GUI.Window = new RadJav.GUI.Window (name);

								appPromise = win.create ().then (function (mainWindow)
															{
																return (mainWindow);
															});
							}
						}

						if (appPromise != null)
						{
							appPromise.then (function (win)
								{
									RadJav.loadAppXML (win, app.children);
								});
						}
					}

					return (Promise.all (promises).then (function ()
									{

									}));
				}
			});

		return promise;
	}

	export function loadAppXML (parent: RadJav.GUI.GObject, 
					objs: RadJav.XML.XMLTag[]): RadJav.GUI.GObject[]
	{
		let tags: TagType[] = [];

		if (typeof (RadJav.GUI) != "undefined")
		{
			for (let key in RadJav.GUI)
			{
				let xmlTag: TagType = null;

				if (typeof (RadJav.GUI[key].xmlTag) != "undefined")
					xmlTag = RadJav.GUI[key].xmlTag;

				tags.push (xmlTag);
			}
		}

		let newObjs: RadJav.GUI.GObject[] = [];

		for (let iIdx = 0; iIdx < tags.length; iIdx++)
		{
			let xmlTag: TagType = tags[iIdx];

			for (let iJdx = 0; iJdx < objs.length; iJdx++)
			{
				let obj: RadJav.XML.XMLTag = objs[iJdx];
				let foundTags: RadJav.XML.XMLTag[] = obj.getTags (xmlTag.tag);

				for (let iKdx = 0; iKdx < foundTags.length; iKdx++)
				{
					let foundTag: RadJav.XML.XMLTag = foundTags[iKdx];
					let newObj: RadJav.GUI.GObject = new RadJav.GUI[xmlTag.type] (foundTag);

					newObjs.push (newObj);
				}
			}
		}

		return (newObjs);
	}

	/// Check if this is a mobile application.
	export function isMobile ()
	{
		if (RadJav.OS.HTML5 != null)
		{
			let os: string = RadJav.OS.HTML5.getOS ();

			if (os == "android")
				return (true);

			if (os == "iphone")
				return (true);

			if (os == "ipad")
				return (true);

			if (os == "ipod")
				return (true);
		}
		else
		{
			if (RadJav.OS.type == "android")
				return (true);

			if (RadJav.OS.type == "ios")
				return (true);
		}

		return (false);
	}

	/** 
	* Checks to see if the current web browser is using Internet Explorer.
	* @return {Boolean} Returns true if the web browser is Internet Explorer.
	*/
	export function _isUsingInternetExplorerTheWorstWebBrowserEver(): boolean
	{
		if (navigator.appName)
		{
			if (navigator.appName == "Microsoft Internet Explorer")
				return true;
		}

		return false;
	}

	/** 
	 * Get a synchronous response from HTTP. This will lock whatever thread it is currently on!
	 * @param {String} addr The address to connect to.
	 * @return {String} The response from the HTTP server.
	 */
	export function _getSyncResponse(addr: string): string {
		var request = null;
		var response = null;

		if (RadJav.useAjax == false) {
			throw RadJav.getLangString("cannotGetAjaxResponse");
		}

		if ((<any>window).XMLHttpRequest) {
			request = new XMLHttpRequest();
		} else {
			request = new ActiveXObject("Microsoft.XMLHTTP");
		}

		request.onreadystatechange = RadJav.keepContext(
			function(evt, request2) {
				var req = request2[0];

				try {
					if (req.readyState == 4 && req.status == 200) {
						response = req.responseText;
					}
				} catch (ex) {}
			},
			this,
			[request]
		);

		request.open("GET", addr, false);
		request.send();

		return response;
	}

	/** 
	 * Get an asynchronous response from HTTP.
	 * @param {String} addr The address to connect to.
	 * @return {String} The response from the HTTP server.
	 */
	export function _getResponse(addr: string): Promise<string> {
		var promise = null;

		if (RadJav.useAjax == true) {
			promise = RadJav.Net.httpRequest(addr);
		} else {
			RadJav._lang["cannotGetAjaxResponse"] =
				"Cannot get ajax response, RadJav is set to not use Ajax.";
			throw RadJav.getLangString("cannotGetAjaxResponse");
		}

		return promise;
	}

	/** 
	 * Perform a deep copy of an object. This has been copied from jQuery.
	 * Thank you jQuery!
	 * Available on platforms: Windows,Linux,OSX,HTML5
	 * @param {Object} obj The object to clone.
	 * @return {Object} The cloned object.
	 */
	export function clone(...obj:any[]): any[] {
		var options,
			name,
			src,
			copy,
			copyIsArray,
			clone,
			target = arguments[0] || {},
			i = 1,
			length = arguments.length,
			deep = false;

		// Handle a deep copy situation
		if (typeof target === "boolean") {
			deep = target;

			// Skip the boolean and the target
			target = arguments[i] || {};
			i++;
		}

		var isPlainObject = function(obj) {
			var proto, Ctor;

			// Detect obvious negatives
			// Use toString instead of jQuery.type to catch host objects
			if (!obj || obj.toString() !== "[object Object]") {
				return false;
			}

			proto = Object.getPrototypeOf(obj);

			// Objects with no prototype (e.g., `Object.create( null )`) are plain
			if (!proto) {
				return true;
			}

			// Objects with prototype are plain iff they were constructed by a global Object function
			Ctor = proto.hasOwnProperty("constructor") && proto.constructor;
			return (
				typeof Ctor === "function" &&
				Ctor.toString() === Ctor.toString.call(Object)
			);
		};

		var isFunction = function(obj) {
			return typeof obj === "function";
		};

		// Handle case when target is a string or something (possible in deep copy)
		if (typeof target !== "object" && !isFunction(target)) {
			target = {};
		}

		// Extend jQuery itself if only one argument is passed
		if (i === length) {
			target = this;
			i--;
		}

		for (; i < length; i++) {
			// Only deal with non-null/undefined values
			if ((options = arguments[i]) != null) {
				// Extend the base object
				for (name in options) {
					src = target[name];
					copy = options[name];

					// Prevent never-ending loop
					if (target === copy) {
						continue;
					}

					// Recurse if we're merging plain objects or arrays
					if (
						deep &&
						copy &&
						(isPlainObject(copy) || (copyIsArray = Array.isArray(copy)))
					) {
						if (copyIsArray) {
							copyIsArray = false;
							clone = src && Array.isArray(src) ? src : [];
						} else {
							clone = src && isPlainObject(src) ? src : {};
						}

						// Never move original objects, clone them
						target[name] = RadJav.clone(deep, clone, copy);

						// Don't bring in undefined values
					} else if (copy !== undefined) {
						target[name] = copy;
					}
				}
			}
		}

		// Return the modified object
		return target;
	}

	/** 
	* Perform a deep copy of an object.
	* Available on platforms: Windows,Linux,OSX,HTML5
	* @param {Object} obj The object to clone.
	* @return {Object} The cloned object.
	*/
	export function cloneObject(obj: any): any
	{
		return RadJav.clone({}, obj);
	}

	/** 
	 * Perform a deep copy of an array.
	 * Available on platforms: Windows,Linux,OSX,HTML5
	 * @param {Array} obj The array to clone.
	 * @return {Array} The cloned array.
	 */
	export function cloneArray(obj: any[]): any[] {
		return RadJav.clone([], obj);
	}

	/** 
	 * Copy the properties of one object to another.
	 * Available on platforms: Windows,Linux,OSX,HTML5
	 * @param {Object} obj1 The object to receive the properties.
	 * @param {Object} obj2 The object to send the properties.
	 * @param {Boolean} [overwriteExisting=true] If set to true, this will overwrite any
	 * existing keys.
	 * @return {Object} The completed object.
	 */
	export function copyProperties(
		obj1: object,
		obj2: object,
		overwriteExisting: boolean
	): object {
		if (overwriteExisting == null) {
			overwriteExisting = true;
		}

		for (var key in obj2) {
			if (overwriteExisting == false) {
				if (obj1[key] == null) {
					obj1[key] = obj2[key];
				}
			} else {
				obj1[key] = obj2[key];
			}
		}

		return obj1;
	}
	
	/** 
	* Set a default value to a parameter should it the parameter be set to 
	* undefined.
	* Available on platforms: Windows,Linux,OSX,HTML5
	* @param {Mixed} param The parameter value to check.
	* @param {Mixed} defaultValue The default value to set should param be set to undefined.
	* @param {Function} [onValue=null] This function is called when a value can be retrieved from the 
	* param parameter.
	* @return {Mixed} Will return the value of param should it not be set to undefined. If param 
	* is set to undefined, defaultValue will be returned instead.
	*/
	 export function setDefaultValue (param: any, defaultValue: any, onValue?: Function): any
	{
		if (param == undefined)
			return (defaultValue);

		if (onValue != null)
			return (onValue (param));

		return (param);
	}

	/**Load a CommonJS module.
	 */
	export function loadModule (moduleData: string)
	{
		return (_eval("(function (module){ " + moduleData + "; return module; })({});"));
	}

	/** 
	 * Keep the context the object is currently in.
	 * Available on platforms: Windows,Linux,OSX,HTML5
	 * @param {Function} func The document element's id.
	 * @param {Object} context The object to remain in context.
	 * @param {Mixed} [val=undefined] An additional value to pass to the context.
	 * @return {Mixed} The returned result from the function func.
	 */
	export function keepContext(func: Function, context: object, val?: any): any {
		var objReturn = function() {
			var aryArgs = Array.prototype.slice.call(arguments);

			if (val != undefined) {
				aryArgs.push(val);
			}

			if (context == null) {
				return func.apply(this, aryArgs);
			} else {
				return func.apply(context, aryArgs);
			}
		};

		return objReturn;
	}

	/** 
	 * Get a language string from the current lanuage. Additional arguments can be
	 * added to this method to combine the strings together using Utils.combineString.
	 * @param {String} keyword The keyword to use when getting the language string.
	 * @return {String} The string associated with the keyword.
	 */
	export function getLangString(keyword,...other): string {
		var args = Array.prototype.slice.call(arguments);
		args.splice(0, 1);
		args.splice(0, 0, RadJav._lang[keyword]);

		return RadJav.combineString.apply(RadJav, args);
	}

	/** 
	* Combine multiple strings together using %s in the first argument.
	*
	*     @example
	*     var firstName = "John";
	*     var lastName = "Doe";
	*     var result = RadJav.combineString ("Hi there %s %s!", firstName, lastname);
	*     // The result will contain:
	*     // Hi there John Doe!
	* @param {String} primaryString The primary string that contains %s. Each %s will be
	* replaced with an argument specified in the order in which each argument is received.
	* @return {String} The result of the string combining.
	*/
	export function combineString(primaryString: string, ...otherStrings: string[]): string
	{
		let strReturn = "";

		if (primaryString != null)
			strReturn = primaryString;

		for (let iIdx = 1; iIdx < arguments.length; iIdx++)
			strReturn = strReturn.replace("%s", arguments[iIdx]);

		return (strReturn);
	}

	/// For easily handling resolves on desktop/mobile.
	export function _resolveThis (resolve: Function, reject: Function): void
	{
		resolve (this);
	}

	/// For easily handling resolves on desktop/mobile.
	export function _emptyResolve (resolve: Function, reject: Function): any
	{
		return (resolve ());
	}

	/// For handling created desktop/mobile GUI objects.
	export function _guiFinishedCreatingGObject (resolve, reject)
	{
		var promises = [];

		for (var iIdx = 0; iIdx < this._children.length; iIdx++)
		{
			let justCreateTheObject: boolean = true;

			if (this.type == "RadJav.MUI.Navigator")
			{
				if (this["pushView"] != "")
				{
					justCreateTheObject = false;

					this._children[iIdx] = RadJav.GUI.initObj(
							this._children[iIdx],
							"",
							"",
							this
						);

					promises.push(this._children[iIdx].create().then (
						RadJav.keepContext(function (result)
						{
							this["push"] (result);

							return (result);
						}, this)));
				}
			}

			if (justCreateTheObject == true)
			{
				this._children[iIdx] = RadJav.GUI.initObj(
					this._children[iIdx],
					"",
					"",
					this
				);

				promises.push(this._children[iIdx].create());
			}
		}

		Promise.all(promises).then(
			RadJav.keepContext(function()
			{
				for (var key in this._events)
				{
					if (this._events[key] != null)
					{
						var func = new Function(this._events[key]);

						if (RadJav["currentTheme"] != null)
							RadJav.currentTheme.event(this.type, "on", this, key, func);
					}
				}

				if (this.onCreated != null)
					this.onCreated();

				resolve(this);
			}, this));
	}

	export function getTime (): number
	{
		return (Date.now () / 1000);
	}

	export function addAnimation (anim: RadJav.Animation): void
	{
		let startAnimationLoop: boolean = false;

		if (RadJav._animations.length < 1)
			startAnimationLoop = true;

		RadJav._animations.push (anim);

		if (startAnimationLoop == true)
			setTimeout (RadJav.animationUpdate, RadJav.animationFrameRate);
	}

	export function animationUpdate (): void
	{
		let currentTime: number = RadJav.getTime ();
		let deltaTime: number = (currentTime - prevTime);

		prevTime = currentTime;

		for (let iIdx = 0; iIdx < RadJav._animations.length; iIdx++)
		{
			let anim: RadJav.Animation = RadJav._animations[iIdx];

			anim.update (deltaTime);
		}

		if (RadJav._animations.length > 0)
			setTimeout (RadJav.animationUpdate, RadJav.animationFrameRate);
	}

	export class TagType
	{
		tag: string;
		type: string;

		constructor ()
		{
			this.tag = "";
			this.type = "";
		}
	}

	export class Theme
	{
		/** @property {String} [name=""]
		* The name of the theme.
		*/
		name: string = "";

		/** @property {String} [version=""]
		* The theme's version.
		*/
		version: string = "";

		/** @property {String} [author=""]
		* The theme's author.
		*/
		author: string = "";

		/** @property {Date} [lastUpdated=null]
		* The theme's last update date.
		*/
		lastUpdated: Date = null;

		/** @property {String} [description=""]
		* The theme's description.
		*/
		description: string = "";

		/** @property {String} [url=""]
		* The url to this theme.
		*/
		url: string = "";

		/** @property {String} [initFile=""]
		* The initialization file to start.
		*/
		initFile: string = "";

		/** @property {String[]} [cssFiles=[]]
		* CSS files to load.
		*/
		cssFiles: string[] = [];

		/** @property {Object[]}[fonts=[]]
		* Fonts to load.
		*/
		fonts: object[] = [];

		/** @property exports
		 * @static
		 * The functions and properties associated with this theme.
		 */
		public exports: any;

		/** @property themeObjects
		 * @static
		 * The theme objects associated with this theme.
		 */
		public themeObjects: any[];

		constructor(obj: Theme = null)
		{
			this.name = RadJav.setDefaultValue(obj.name, "");
			this.version = RadJav.setDefaultValue(obj.version, "");
			this.author = RadJav.setDefaultValue(obj.author, "");
			this.lastUpdated = RadJav.setDefaultValue(obj.lastUpdated, null);
			this.description = RadJav.setDefaultValue(obj.description, "");
			this.url = RadJav.setDefaultValue(obj.url, "");
			this.initFile = RadJav.setDefaultValue(obj.initFile, "");
			this.cssFiles = RadJav.setDefaultValue(obj.cssFiles, []);
			this.fonts = RadJav.setDefaultValue(obj.fonts, []);
			this.exports = RadJav.setDefaultValue(obj.exports, null);
			this.themeObjects = RadJav.setDefaultValue(obj.themeObjects, {});
		}

		/** 
		 * Load the initialization file and execute it.
		 * @return {Promise} Executes when the loading has completed.
		 */
		loadInitializationFile(): Promise<any>
		{
			let promise = new Promise(
				RadJav.keepContext(function(resolve, reject)
				{
					let func = RadJav.keepContext(function(data)
					{
						try
						{
							var promises = [];

							if (typeof data == "string")
								RadJav.currentTheme.exports = RadJav.loadModule (data).exports;

							if (typeof (RadJav.currentTheme.exports.init) != "undefined")
							{
								var hasPromise = RadJav.currentTheme.exports.init();

								if (hasPromise != null)
									promises.push (hasPromise);
							}

							let fontCSS = "";

							for (let iIdx = 0; iIdx < this.fonts.length; iIdx++)
							{
								let fontName = this.fonts[iIdx].name;
								let fontUrl = this.url + "/" + this.fonts[iIdx].relPath;

								fontCSS += "@font-face\n";
								fontCSS += "{\n";
								fontCSS += '\tfont-family: "' + fontName + '";\n';
								fontCSS += '\tsrc: url("' + fontUrl + '");\n';
								fontCSS += "}\n\n";
							}

							if (this.fonts.length > 0)
							{
								var style = document.createElement("style");
								style.innerHTML = fontCSS;
								document.head.appendChild(style);
							}

							if (RadJav.useAjax == true)
							{
								for (var iIdx = 0; iIdx < this.cssFiles.length; iIdx++)
								{
									promises.push(
										RadJav._getResponse(this.url + "/" + this.cssFiles[iIdx]).then(
											function(data)
											{
												var style = document.createElement("style");
												style.innerHTML = data;
												document.head.appendChild(style);
											}));
								}
							}

							Promise.all(promises).then(function()
							{
								resolve();
							});
						}
						catch (ex)
						{
							throw RadJav.getLangString("themeThrewErrorInFile", this.name, this.initFile, ex.message);
						}
					}, this);

					if (RadJav.useAjax == true)
						RadJav._getResponse(this.url + "/" + this.initFile).then(func);
					else
					{
						func(RadJav.currentTheme.exports);
						resolve();
					}
				}, this)
			);

			return promise;
		}

		/** 
		 * Load the javascript files for this theme.
		 * @return {Promise} Executes when the loading has completed.
		 */
		loadJavascriptFiles(): Promise<any> {
			var promise = new Promise(
				RadJav.keepContext(function(resolve, reject)
				{
					var files = [];

					for (var iIdx = 0; iIdx < RadJav._included.length; iIdx++) {
						var includeObj = RadJav._included[iIdx];

						if (typeof includeObj != "string") {
							if (typeof includeObj.themeFile == "string") {
								files.push(includeObj.themeFile);
							} else {
								if (includeObj.themeFile == true) {
									files.push(includeObj.file);
								} else {
									continue;
								}
							}
						}
					}

					for (var iIdx = 0; iIdx < files.length; iIdx++) {
						var file = files[iIdx];

						(function(theme, url, tfile, index, numFiles) {
							try {
								if (RadJav.currentTheme.themeObjects[tfile] == null) {
									RadJav.currentTheme.themeObjects[tfile] = new Object();
								}

								if (RadJav.useAjax == true) {
									RadJav._getResponse(url + "/" + tfile + ".js").then(
										function(data)
										{
											try {
												RadJav.currentTheme.themeObjects[tfile] = RadJav.loadModule (data);
											} catch (ex)
											{
												throw RadJav.getLangString(
													"themeThrewErrorInFile",
													theme.name,
													tfile + ".js",
													ex.message
												);
											}

											/*var js = "return (function (module, theme){";
													js += data + "\n";
													js += "});";

													try
													{
														var obj = new Function (js);
														RadJav.Theme.themeObjects[tfile].javascript = obj.apply (this, [{}, theme]);
													}
													catch (ex)
													{
														throw (RadJav.getLangString ("themeThrewErrorInFile", theme.name, 
																tfile + ".js", ex.message));
													}*/

											if (index >= numFiles - 1) {
												resolve();
											}
										});
								} else {
									resolve();
								}
							} catch (ex) {}
						})(this, this.url, file, iIdx, files.length);
					}
				}, this)
			);

			return promise;
		}

		/** 
		 * Execute a theme event.
		 * @param {String} file The file associated with the event.
		 * @param {String} event The name of the event to trigger.
		 * @return {Promise} The promise to execute when this event is completed.
		 */
		 event(file: string, event: string,...other): Promise<any>
		 {
			var args = new Array();

			for (var iIdx = 2; iIdx < arguments.length; iIdx++)
				args.push(arguments[iIdx]);

			try
			{
				if (RadJav.currentTheme.themeObjects[file] != null)
				{
					if (RadJav.currentTheme.themeObjects[file].exports[event] != null)
					{
						return RadJav.currentTheme.themeObjects[file].exports[event].apply(
							RadJav.currentTheme.themeObjects[file].exports,
							args
						);
					}
					else
					{
						if ((file.indexOf("GUI") > -1) || (file.indexOf("MUI") > -1))
						{
							var tempfile = "RadJav.GUI.GObject";

							if (RadJav.currentTheme.themeObjects[tempfile].exports[event] != null)
							{
								return RadJav.currentTheme.themeObjects[tempfile].exports[event].apply(
									RadJav.currentTheme.themeObjects[tempfile].exports,
									args
								);
							}
						}
					}
				}
			} catch (ex) {
				throw "Error in " +
					file +
					" message: " +
					ex.message +
					"\nStack: " +
					ex.stack;
			}

			/*var jsModule = null;

			if (RadJav.theme.themeObjects[file] != null)
				jsModule = RadJav.theme.themeObjects[file].javascript;

			if (jsModule != null)
			{
				var mod = {};
				jsModule (mod, this);

				if (mod.exports[event] != null)
					return (mod.exports[event].apply (mod.exports, args));
			}*/

			return null;
		}

		/** 
		 * Execute a theme event synchronously.
		 * @param {String} file The file associated with the event.
		 * @param {String} event The name of the event to trigger.
		 * @return {Mixed} The data returned from the event.
		 */
		eventSync(file: string, event: string, ...other): any
		{
			var args = new Array();
			var result = null;

			for (var iIdx = 2; iIdx < arguments.length; iIdx++)
				args.push(arguments[iIdx]);

			try
			{
				if (RadJav.currentTheme.themeObjects[file] != null)
				{
					if (RadJav.currentTheme.themeObjects[file].exports[event] != null)
					{
						result = RadJav.currentTheme.themeObjects[file].exports[event].apply(
							RadJav.currentTheme.themeObjects[file].exports,
							args
						);
					}
					else
					{
						if ((file.indexOf("GUI") > -1) || (file.indexOf("MUI") > -1))
						{
							var tempfile = "RadJav.GUI.GObject";

							if (RadJav.currentTheme.themeObjects[tempfile].exports[event] != null) {
								result = RadJav.currentTheme.themeObjects[tempfile].exports[event].apply(
									RadJav.currentTheme.themeObjects[tempfile].exports,
									args
								);
							}
						}
					}
				}
			}
			catch (ex)
			{
				throw "Error in " +
					file +
					" message: " +
					ex.message +
					"\nStack: " +
					ex.stack;
			}

			/*var jsModule = null;

			if (RadJav.Theme.themeObjects[file] != null)
				jsModule = RadJav.Theme.themeObjects[file].javascript;

			if (jsModule != null)
			{
				var mod = {};
				jsModule (mod, this);

				if (mod.exports[event] != null)
					result = mod.exports[event].apply (mod.exports, args);
			}*/

			return result;
		}

		/** 
		 * Load the theme.
		 * @param {String} url The URL to this theme.
		 * @param {String} data The JSON to parse and get the data from.
		 */
		public static loadTheme(url: string, data: any): Theme
		{
			var theme = null;

			try
			{
				theme = new RadJav.Theme(data);
				theme.url = url;
			}
			catch (ex)
			{
				console.error(ex.message);
			}

			return theme;
		}
	}

	export namespace GUI
	{
		/** 
		* Initialize a GUI object.
		* @param {String} type The object type to create.
		* @param {String/Mixed} name The name of the object.
		* @param {String} text The text associated with the object.
		* @param {RadJav.GUI.GObject} parent The parent of this object.
		* @param {Promise} The promise to execute when this object has finished being created.
		*/
		export function initObj(type: string | object | any, 
			name: any, text: string, parent: object, args: any[] = []): RadJav.GUI.GObject
		{
			let tempType = type;

			if (typeof type == "object") {
					tempType = type.type;

					if (type.name != null) {
					name = type.name;
				}

				if (type.text != null) {
					text = type.text;
				}

				if (type._text != null) {
					text = type._text;
				}
			}

			if (tempType.indexOf("RadJav.GUI") > -1)
				tempType = tempType.substr(11);

			if (tempType.indexOf("RadJav.MUI") > -1)
				tempType = tempType.substr(11);

			if (RadJav.GUI[tempType] == null)
				throw RadJav.getLangString("unableToFindClass", tempType);

			var properties = {
					name: name,
					text: text,
					parent: parent
				};

			if (typeof type == "object") {
				RadJav.copyProperties(properties, type, false);
			}

			var obj = null;

			if (tempType == "Navigator")
			{
				let parentObj: any = null;

				if (args.length > 0)
					parentObj = args[0];

				obj = new RadJav.GUI["Navigator"](parentObj, properties);
			}
			else
				obj = new RadJav.GUI[tempType](properties);

			return obj;
		}

		/** 
		* Create a GUI object.
		* @param {String} type The object type to create.
		* @param {String/Mixed} name The name of the object.
		* @param {String} text The text associated with the object.
		* @param {RadJav.GUI.GObject} parent The parent of this object.
		* @param {Promise} The promise to execute when this object has finished being created.
		*/
		export function create(type: string, name: string, text: string, parent: object, args: any[] = []): any
		{
			var obj = this.initObj(type, name, text, parent, args);

			return obj.create();
		}

		/** 
		* Create GUI objects.
		* @param {String/RadJav.GUI.GObject[]} objects The objects to create.
		* @param {RadJav.GUI.GObject} parent The parent of this object.
		* @param {Function} [beforeCreated=null] The function to execute before the object is created.
		* If this function returns false, the object will not be created.
		* @return {Promise} The promise to execute when the objects have finished being created.
		*/
		export function createObjects(objects: any, parent: object, beforeCreated: Function = null): Promise<any>
		{
			var promises = [];

			if (beforeCreated == undefined)
				beforeCreated = null;

			for (var iIdx = 0; iIdx < objects.length; iIdx++)
			{
				var obj = objects[iIdx];
				var createObject = true;

				if (beforeCreated != null)
				{
					obj.onBeforeChildCreated = beforeCreated;
					var result = beforeCreated(obj, parent);

					if (result != null)
						createObject = result;
				}

				if (createObject == true)
				{
					let justCreateTheObject: boolean = true;

					if (obj.type == "RadJav.MUI.Navigator")
					{
						if (obj["useParentForRootView"] != null)
						{
							promises.push(this.create(obj, "", "", parent, [parent]));
							justCreateTheObject = false;
						}
					}

					if (justCreateTheObject == true)
						promises.push(this.create(obj, "", "", parent));
				}
			}

			return Promise.all(promises);
		}
	}

	/// The mobile UI API.
	export namespace MUI
	{
		/** 
		* Initialize a MUI object.
		* @param {String} type The object type to create.
		* @param {String/Mixed} name The name of the object.
		* @param {String} text The text associated with the object.
		* @param {RadJav.GUI.GObject} parent The parent of this object.
		* @param {Promise} The promise to execute when this object has finished being created.
		*/
		export function initObj(type: string | object | any, name: any, text: string, parent: object): object
		{
			let tempType = type;

			if (typeof type == "object") {
					tempType = type.type;

					if (type.name != null) {
					name = type.name;
				}

				if (type.text != null) {
					text = type.text;
				}

				if (type._text != null) {
					text = type._text;
				}
			}

			if (tempType.indexOf("RadJav.MUI") > -1) {
				tempType = tempType.substr(11);
			}

			if (RadJav.MUI[tempType] == null) {
				throw RadJav.getLangString("unableToFindClass", tempType);
			}

			var properties = {
					name: name,
					text: text,
					parent: parent
				};

			if (typeof type == "object") {
				RadJav.copyProperties(properties, type, false);
			}

			var obj = new RadJav.MUI[tempType](properties);

			return obj;
		}

		/** 
		* Create a MUI object.
		* @param {String} type The object type to create.
		* @param {String/Mixed} name The name of the object.
		* @param {String} text The text associated with the object.
		* @param {RadJav.GUI.GObject} parent The parent of this object.
		* @param {Promise} The promise to execute when this object has finished being created.
		*/
		export function create(type: string, name: string, text: string, parent: object): any
		{
			var obj = this.initObj(type, name, text, parent);

			return obj.create();
		}

		/** 
		* Create MUI objects.
		* @param {String/RadJav.GUI.GObject[]} objects The objects to create.
		* @param {RadJav.GUI.GObject} parent The parent of this object.
		* @param {Function} [beforeCreated=null] The function to execute before the object is created.
		* If this function returns false, the object will not be created.
		* @return {Promise} The promise to execute when the objects have finished being created.
		*/
		export function createObjects(objects: any, parent: object, beforeCreated: Function = null): Promise<any>
		{
			var promises = [];

			if (beforeCreated == undefined) {
				beforeCreated = null;
			}

			for (var iIdx = 0; iIdx < objects.length; iIdx++) {
				var obj = objects[iIdx];
				var createObject = true;

				if (beforeCreated != null) {
					obj.onBeforeChildCreated = beforeCreated;
					var result = beforeCreated(obj, parent);

					if (result != null) {
						createObject = result;
					}
				}

				if (createObject == true) {
					promises.push(this.create(obj, "", "", parent));
				}
			}

			return Promise.all(promises);
		}
	}

	/** @class C3D
	* @static
	* Contains classes for 3d operations in a RadJav.GUI.Canvas3D object.
	*/
	export namespace C3D
	{
		/** 
		* Create a 3D object.
		* @param {String} type The object type to create.
		* @param {String|Mixed} name The name of the object.
		* @param {RadJav.C3D.Object3D} parent The parent of this object.
		* @param {Promise} The promise to execute when this object has finished being created.
		*/
		function create(type: string, name: any, parent: any): any
		{
			if (type.indexOf("RadJav.C3D") > -1) {
				type = type.substr(10);
			}

			if (RadJav.C3D[type] == null) {
				throw RadJav.getLangString("unableToFindClass", type);
			}

			var obj = new RadJav.C3D[type](name, parent);

			return obj.create();
		}
	}

	/** @class Net
	* @static
	* Contains classes for network operations.
	*/
	export namespace Net
	{
		/** 
		* Make an ajax request to a HTTP server.
		* Available on platforms: Windows,Linux,OSX,HTML5
		* @param {String/Object} req The URL or request object to send to the server.
		* @return {Promise} The promise to execute when the request has completed.
		*/
		export function httpRequest(req: string | object): Promise<any>
		{
			var promise = new Promise(RadJav.keepContext(function(resolve, reject)
				{
					var addr = req;
					var request = null;
					var response = null;

					try
					{
						if (XMLHttpRequest != null)
							request = new XMLHttpRequest();
						else
							request = new ActiveXObject("Microsoft.XMLHTTP");

						request.onreadystatechange = RadJav.keepContext(function(evt, request2)
							{
								var req2 = request2[0];

								try
								{
									if (req2.readyState == 4 && req2.status == 200)
										resolve(req2.responseText);
								}
								catch (ex)
								{
									reject(ex);
								}
							}, this, [request]);

						request.open("GET", addr);
						request.send();
					}
					catch (ex)
					{
						reject(ex);
					}
				}, this));

			return promise;
		}
	}

	/// Contains classes handling console operations.
	export class Console
	{
		/** 
		* Print a message to the console.
		* @param {String} message The message to output.
		*/
		public static print(message: string): void
		{
			console.log(message);
		}

		/** 
		* Print a message to the console with a new line at the end.
		* @param {String} message The message to output.
		*/
		public static println(message: string): void
		{
			this.print(message + "\n");
		}

		/** 
		* Print a message to the console with a new line at the end.
		* @param {String} message The message to output.
		*/
		public static log(message: string): void
		{
			this.print(message + "\n");
		}
	}

	/// Contains Operating System specific functions.
	export namespace OS
	{
		/** Represents the current type of operating system.
		* Can be:
		* * windows
		* * linux
		* * macosx
		* * android
		* * ios
		* * html5
		*/
		export let type: string = "html5";

		/// The number of bits this operating system is.
		export let numBits: number = 32;

		/// The command line arguments.
		export let args: string[] = [];

		/** Describes the screen used by the user.
		 * Available on platforms: Windows,Linux,OSX,iOS,Android,HTML5
		 */
		export class ScreenInfo
		{
			/// The width of the device's screen.
			public width: number;

			/// The height of the device's screen.
			public height: number;

			/// The scale of points to pixels on the user's screen.
			public scale: number;

			constructor (width: number = 0, height: number = 0, scale: number = 1)
			{
				this.width = width;
				this.height = height;
				this.scale = scale;
			}

			/** 
			 * Get the width of the current screen.
			 * @return {Number} The width of the current screen.
			 */
			public getWidth (): number
			{
				/// @note THE - 16 IS A TEMPORARY HACK TO MATCH THE DESKTOP VERSION OF RADJAV
				return (this.width - 16);
			}

			/** 
			 * Get the height of the current screen.
			 * @return {Number} The height of the current screen.
			 */
			public getHeight (): number
			{
				/// @note THE - 38 IS A TEMPORARY HACK TO MATCH THE DESKTOP VERSION OF RADJAV
				return (this.height - 38);
			}

			/** 
			 * Get the scale of point to pixel.
			 * @return {Number} The point to pixel scale.
			 */
			public getScale (): number
			{
				return (this.scale);
			}

			/** 
			 * Get the number of screens on the device.
			 * @return {Number} The height of the current screen.
			 */
			static getNumberOfScreens (): number
			{
				return (1);
			}

			/** 
			 * Get the screen info for the selected screen.
			 * @return {Number} The height of the current screen.
			 */
			static getScreenInfo (screenIndex: number): ScreenInfo
			{
				return (new ScreenInfo (window.innerWidth, window.innerHeight, 1));
			}
		}

		/** 
		* Execute code when RadJav has finished loading.
		* Available on platforms: Windows,Linux,OSX,HTML5
		* @param {Function} func The function to execute.
		* @return {Promise} The promise to execute.
		*/
		export function onReady(func: any): Promise<any>
		{
			return RadJav.OS.HTML5.ready(window).then(func);
		}

		/** 
		 * Get the path to the user's documents folder.
		 * Available on platforms: Windows,Linux,OSX
		 * @return {String} The current user's documents folder path.
		 */
		/*export function getDocumentsPath() {
		}*/

		/** 
		 * Get the path to the user's temporary files folder.
		 * Available on platforms: Windows,Linux,OSX
		 * @return {String} The current user's temporary files path.
		 */
		/*export function getTempPath() {
		}*/

		/** 
		 * Get the path to the user's data files folder.
		 * Available on platforms: Windows,Linux,OSX
		 * @return {String} The current user's data files path.
		 */
		/*export function getUserDataPath() {
		}*/

		/** 
		 * Get the path to the application.
		 * Available on platforms: Windows,Linux,OSX
		 * @return {String} The path to the application.
		 */
		/*export function getApplicationPath(){
		}*/

		/** 
		 * Get the current working path.
		 * Available on platforms: Windows,Linux,OSX
		 * @return {String} The current working path.
		 */
		/*export function getCurrentWorkingPath(){
		}*/

		/** 
		 * Open a URL in the default web browser.
		 * Available on platforms: Windows,Linux,OSX,HTML5
		 * @param {String} url The url to open.
		 */
		export function openWebBrowserURL(url: string): void {
			window.open(url, "_blank");
		}

		/** Contains HTML5 specific functions.
		 * Available on platforms: HTML5
		 */
		export namespace HTML5 {
			/** If set to true, all objects will be positioned using absolute positioning.
			 * @default true
			 */
			export let absolutePositioning: boolean = true;

			/** 
			 * Show a HTML element.
			 * @param {String/HTMLElement} elm The element to show or hide.
			 * @param {Boolean} [show=true] If set to true the element will be shown.
			 */
			export function showElement(elm: any, show: boolean): void {
				if (typeof elm == "string") {
					elm = RadJav.OS.HTML5.selectDOM(null,elm);
				}

				if (show == true) {
					elm.style.visibility = "visible";
				} else {
					elm.style.visibility = "hidden";
				}
			}

			/** @static
			 * Get the operating system from the browser's user agent.
			 * @return {string} The operating system.
			 */
			export function getOS(): string {
				var userAgent = navigator.userAgent.toLowerCase();

				if (userAgent.indexOf("win32") > -1) {
					return "windows";
				}

				if (userAgent.indexOf("win64") > -1) {
					return "windows";
				}

				if (userAgent.indexOf("windows") > -1) {
					return "windows";
				}

				if (userAgent.indexOf("android") > -1) {
					return "android";
				}

				if (userAgent.indexOf("iphone") > -1) {
					return "iphone";
				}

				if (userAgent.indexOf("ipad") > -1) {
					return "ipad";
				}

				if (userAgent.indexOf("ipod") > -1) {
					return "ipod";
				}

				if (userAgent.indexOf("mac os x") > -1) {
					return "macosx";
				}

				if (userAgent.indexOf("linux") > -1) {
					return "linux";
				}

				return "";
			}

			/** @static
			 * Get the URL parameters as an object.
			 * @return {Object} The url parameters.
			 */
			export function getUrlParamObj(): any {
				var params = window.location.search;
				var paramsObj = {};

				if (params == "") {
					return paramsObj;
				}

				var nextStart = "?";
				var prevPos = -1;
				var pos = -1;

				while (true) {
					prevPos = params.indexOf(nextStart, pos);
					pos = params.indexOf("=", prevPos);

					if (prevPos > -1 && pos > -1) {
						nextStart = "&";
						var key = params.substring(prevPos + 1, pos);
						prevPos = params.indexOf(nextStart, pos);

						if (prevPos < 0) {
							prevPos = undefined;
						}

						var value: any = params.substring(pos + 1, prevPos);

						if (isNaN(value) == false) {
							value = parseFloat(value);
						}

						paramsObj[key] = value;
					} else {
						break;
					}
				}

				return paramsObj;
			}

			/** @static
			 * Get a URL parameters value.
			 * @param {String} name The url parameters name.
			 * @return {Mixed} The url parameters value. Returns undefined if the parameter was
			 * not able to be found.
			 */
			export function getUrlParam(name: string): any {
				var params = RadJav.OS.HTML5.getUrlParamObj();

				return params[name];
			}

			/** @static
			 * Set whether or not each gui object placed is placed using absolute positioning.
			 * @param {Boolean} absolutePositioning If set to true, all objects will be
			 * positioned using absolute positioning.
			 */
			export function useAbsolutePositioning(
				absolutePositioning: boolean
			): void {
				RadJav.OS.HTML5.absolutePositioning = absolutePositioning;
			}

			/** @static
			 * Start downloading a text file.
			 * @param {String} text The text to download.
			 * @param {String} fileName The filename.
			 * @param {String} [mimeType="text/plain"] The mime type.
			 */
			export function downloadTextAsFile(
				text: string,
				fileName: string,
				mimeType: string
			): void {
				if (mimeType == null) {
					mimeType = "text/plain";
				}

				var elm = document.createElement("a");
				elm.setAttribute("href", "data:" + mimeType + "," + text);
				elm.setAttribute("download", fileName);

				if (document.createEvent != null) {
					var evt = document.createEvent("MouseEvents");
					evt.initEvent("click", true, true);
					elm.dispatchEvent(evt);
				} else {
					elm.click();
				}
			}

			/** @static
			 * Reloads the current page.
			 * @param {Boolean} [forceNewPage=false] If set to true, this will force the browser
			 * to get a new page from the server.
			 */
			export function reloadPage(forceNewPage: boolean): void {
				if (forceNewPage == null) {
					forceNewPage = false;
				}

				location.reload(forceNewPage);
			}

			/** Get the parent HTML from an object.
			 * @param {RadJav.GUI.GObject} obj The parent object to get the HTML from.
			 * @return {Mixed} The parent HTML object.
			 */
			export function getParentHTML(obj: any): any {
				var parent = obj.getParent();
				var parentHTML = null;

				if (parent == null) {
					parentHTML = document.body;
				} else {
					if (parent["getHTML"] == null)
						debugger;

					parentHTML = parent.getHTML();
				}

				return parentHTML;
			}

			/** Get the HTML DOM object from some HTML string.
			 * @param {String} str The string to convert into an HTML DOM.
			 * @return {Mixed} The HTML DOM object.
			 */
			export function getHTMLDOM(str: string): Object {
				var div = document.createElement("div");
				div.innerHTML = str;

				return div.firstChild;
			}

			/** Append HTML to an existing HTML DOM object.
			 * @param {Mixed} obj The HTML DOM object to append this HTML to.
			 * @param {String/Mixed} html The HTML to append.
			 */
			export function appendHTML(
				obj: HTMLElement,
				html: any
			): any {
				if (typeof obj == "string") {
					var tempObj = document.querySelector(obj);

					if (tempObj == null) {
						throw RadJav.getLangString("unableToFindSelector", obj);
					}
				}

				if (typeof html == "string") {
					html = RadJav.OS.HTML5.getHTMLDOM(html);
				}

				return obj.appendChild(html);
			}

			/** Use a selector to get a DOM object.
			 * @param {Mixed/String} obj The HTML DOM object to get the selection from. If
			 * this is a string, it will be treated as the selector.
			 * @param {String} selector The selector to use to get the DOM object.
			 * @return {Mixed} The selected DOM object.
			 */
			export function selectDOM(
				obj: HTMLElement,
				selector: any
			): any {
				if (typeof obj == "string") {
					selector = obj;
					obj = null;
				}

				if (obj == null) obj = document.body;

				var dom = obj.querySelector(selector);

				return dom;
			}

			/** When a dom object has finished loading, execute a promise.
			 * @param {Object} obj The object to check.
			 * @return {Promise} The promise to execute.
			 */
			export function ready(obj: any): Promise<void> {
				var promise = new Promise<void>(function(resolve, reject) {
					if (obj.readyState != null) {
						if (obj.readyState == "complete") {
							resolve();

							return;
						}
					}

					obj.addEventListener("load", function() {
						resolve();
					});
				});

				return promise;
			}

			/** 
			 * For use when using a javascript interface to a webview callback. It will attempt
			 * to call the native javascript interface using the connectorName.
			 * @param {String/Object} connectorName On Android, this would be the name of the
			 * Javascript interface that is connected to the webview. On iOS this would be
			 * the name of the message handler for WKWebView. If you are using WebView on iOS
			 * you must set webViewType to iOSWebView in order to be captured, since a reload on
			 * the page is necessary. iOSWebView will not return any result, and when reloading
			 * the page, the next url will be in the format:
			 * connectorName://methodName/arguments in json string
			 *
			 * If this is an object, this is the JSON Schema:
			 * {
			 *	"title": "Interface Connector JSON Schema",
			 *	"type": "object",
			 *	"properties": {
			 *			"name": {
			 *					"description": "The name of the javascript interface.",
			 *					"type": "string"
			 *				},
			 *			"webViewType": {
			 *					"description": "The type of webview that's being used by the application. This can be: AndroidWebView,iOSWKWebView,iOSWebView",
			 *					"type": "string"
			 *				}
			 *		},
			 *	"required": ["name"]
			 * }
			 * @param {String} methodName The name of the method to call. On iOS, this will be passed
			 * as an additional argument.
			 * @return {Mixed} The returned result from the interface.
			 */
			export function interfaceConnector(
				connectorName: string | object | any,
				methodName: string
			): any {
				var result = null;
				var args = Array.prototype.slice.call(arguments);
				args.splice(0, 2);
				var name = "";
				var webViewType = "";

				if (typeof connectorName == "string") {
					name = connectorName;
				} else {
					name = connectorName.name;

					if (connectorName.webViewType != null) {
						webViewType = connectorName.webViewType;
					}
				}

				if (name == null || name == "") {
					throw RadJav.getLangString(
						"connectorNameCannotBeEmptyForInterfaceConnector"
					);
				}

				var found = false;

				if (window[name] != null) {
					// Android
					result = window[name][methodName].apply(window, args);
					found = true;
				}

				if ((<any>window).webkit != null) {
					// iOS WKWebView
					if ((<any>window).webkit.messageHandlers != null) {
						if ((<any>window).webkit.messageHandlers[name] != null) {
							args = Array.prototype.slice.call(arguments);
							args.splice(0, 1);
								(<any>window).webkit.messageHandlers[name].postMessage(args);
							found = true;
						}
					}
				}

				if (found == false)
				{
					if (webViewType == "iOSWebView")
					{
						var userAgent = window.navigator.userAgent.toLowerCase();

						if (
							userAgent.match(/iphone/i) ||
							userAgent.match(/ipad/i) ||
							userAgent.match(/ipod/i)
						)
						{
							var standalone = (<any>window.navigator).standalone;
							var isSafari = userAgent.match(/safari/i);

							if (standalone == false && isSafari == null) {
								document.location.href =
									name + "://" + methodName + "/" + JSON.stringify(args);
							}
						}
					}
				}

				return result;
			}
		}
	}
}

function parseBoolean (str: string): boolean
{
	str = str.toLowerCase ();

	if (str == "yes")
		return (true);
	else if (str == "no")
		return (false);
	else if (str == "true")
		return (true);
	else if (str == "false")
		return (false);
	else
	{
		var iValue = parseInt (str);

		if (iValue != 0)
			return (true);
	}

	return (false);
}

// @ts-ignore
// Do not change anything about this next line.
RadJav.GENERATORS_INJECT_RADJAV_OS_CODE_HERE;

if (typeof (console) == "undefined")
{
	// @ts-ignore
	var console = function ()
	{
	}

	console.log = RadJav.Console.println;
}

RadJav.setupScreens ();

var _eval = eval;
var _Function = Function;
RadJav.defaults = RadJav;

// This may be needed for Dojo, idk.
/*if (typeof (define) != "undefined")
{
	define(function()
		{
			return RadJav;
		});
}*/
