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

/** The base GUI object.
 * Available on platforms: Windows,Linux,OSX,HTML5
 */
/// <reference path="RadJav.ts" />
/// <reference path="RadJav.Vector2.ts" />
/// <reference path="RadJav.Rectangle.ts" />
/// <reference path="RadJav.Font.ts" />

 namespace RadJav
 {
	export namespace GUI
	{
		export class GObject
		{
			/// The name of this object.
			name: string;
			/// The type of object.
			type: string;
			/// The transform of this object.
			_transform: Rectangle;
			/** 
			 * The visibility of the object.
			 */
			_visible: boolean;
			/** 
			 * The initial z-index of this object. The higher the value the more "on top" the
			 * object will be compared to other objects.
			 */
			_zIndex: number;
			/** 
			 * The text associated with this object.
			 */
			_text: string;
			/** 
			 * The font associated with this object.
			 */
			_font: Font;
			/** 
			 * The cursor to use.
			 */
			_cursor: string;
			/** 
			 * The parent of this object.
			 */
			_parent: GObject;
			/** 
			 * This object's children.
			 */
			_children: GObject[];
			/** 
			 * The native web object associated with this object.
			 */
			_html: any;
			/** 
			 * The native os gui object associated with this object.
			 */
			_appObj: any;
			/** 
			 * How this object is handled with an account.
			 */
			access: Account;
			/** 
			 * Create this object only on certain platforms. All platforms will be enabled
			 * by default, only the ones listed in each key will either be created or
			 * denied by their boolean value. Ex: { "WIN32": true, "HTML5": false }.
			 * Possible keys are:
			 * WIN32
			 * WIN64
			 * LINUX32
			 * LINUX64
			 * MACOSX32
			 * MACOSX64
			 * HTML5
			 */
			createOnPlatforms: Object;
			/** 
			 * The function to execute before a child is created.
			 */
			onBeforeChildCreated: ((obj: Object,parent?:any) => void);
			/** 
			 * The function to execute once the object has been created.
			 */
			onCreated: ((obj: Object) => void);
			/// Execute this function when a child is added. If this function 
			/// returns false, the child will not be added.
			onChildAdded: ((obj: GObject) => boolean);

			/** 
			 * Events to call.
			 */
			_events: ((...args: any[]) => any);

			constructor(
					obj?: any,
					text?: string,
					parent?: GObject,
					beforeCreatedChild?: boolean
				)
			{
				if (obj == null)
					obj = new Object();

				if (typeof (RadJav["XML"]) != "undefined")
				{
					if (obj instanceof RadJav["XML"]["XMLTag"])
						obj = GObject.parseGObjectXML (obj);
				}

				if (typeof obj == "string")
				{
					var tempObj = obj;
					obj = {};
					obj.name = tempObj;
				}

				if (beforeCreatedChild == undefined)
					beforeCreatedChild = null;

				if (text != null)
					obj._text = text;

				if (parent != null)
					obj._parent = parent;

				if (obj.text != null)
					obj._text = obj.text;

				if (obj.cursor != null)
					obj._cursor = obj.cursor;

				if (obj.visible != null)
					obj._visible = obj.visible;

				if (obj.visibility != null)
					obj._visible = obj.visibility;

				if (obj.zIndex != null)
					obj._zIndex = obj.zIndex;

				if (obj.font != null)
					obj._font = new RadJav.Font(obj.font);

				if (obj.parent != null)
					obj._parent = obj.parent;

				if (obj._parent != null)
				{
					if (obj._parent.type == "RadJav.MUI.Navigator")
					{
						if ((RadJav.OS.type == "android") || 
							(RadJav.OS.type == "ios"))
						{
							obj._parent = null;
						}
						else
						{
							obj._visible = false;
						}
					}
				}

				this.name = RadJav.setDefaultValue(obj.name, "");
				this.type = RadJav.setDefaultValue(obj.type, "");
				this._transform = RadJav.setDefaultValue(
					obj._transform,
					new RadJav.Rectangle()
				);
				this._visible = RadJav.setDefaultValue(obj._visible, true);
				this._zIndex = RadJav.setDefaultValue(obj._zIndex, 0);
				this._text = RadJav.setDefaultValue(obj._text, "");
				this._font = RadJav.setDefaultValue(obj._font, new RadJav.Font());
				this._cursor = RadJav.setDefaultValue(obj._cursor, "default");
				this._parent = RadJav.setDefaultValue(obj._parent, null);
				this._children = RadJav.setDefaultValue(obj._children, []);
				this._html = RadJav.setDefaultValue(obj._html, null);
				this._appObj = RadJav.setDefaultValue(obj._appObj, null);
				this.createOnPlatforms = RadJav.setDefaultValue(
					obj.createOnPlatforms,
					null
				);
				this.onBeforeChildCreated = RadJav.setDefaultValue(
					obj.onBeforeChildCreated,
					null
				);
				this.onCreated = RadJav.setDefaultValue(obj.onCreated, null);
				this.onChildAdded = RadJav.setDefaultValue(obj.onChildAdded, null);
				this._events = RadJav.setDefaultValue(obj._events, {});

				if (obj.events != null) {
					this._events = obj.events;
				}

				if (obj.click != null) {
					(<any>this._events).click = obj.click;
				}

				if (obj.children != null)
				{
					for (var iIdx = 0; iIdx < obj.children.length; iIdx++)
					{
						var obj2: RadJav.GUI.GObject = obj.children[iIdx];
						var createObject: any = obj2;

						if (this.onBeforeChildCreated != null)
							createObject = this.onBeforeChildCreated(obj2, parent);

						this.addChild (createObject);
					}
				}

				if (obj.position != null) {
					var position = new RadJav.Vector2();

					if (typeof obj.position == "string") {
						position = RadJav.Vector2.parseVector2(obj.position);
					} else {
						position = obj.position;
					}

					this._transform.setPosition(position);
				}

				if (obj.size != null) {
					var size = new RadJav.Vector2();

					if (typeof obj.size == "string") {
						size = RadJav.Vector2.parseVector2(obj.size);
					} else {
						size = obj.size;
					}

					this._transform.setSize(size);
				}
			}

			/// Add a child.
			public addChild (child: GObject): void
			{
				if (this.onChildAdded != null)
				{
					if (this.onChildAdded (child) === false)
						return;
				}

				let doParentLOL: boolean = true;

				if (this._parent.type == "RadJav.MUI.Navigator")
				{
					child._visible = false;

					if ((RadJav.OS.type == "android") || 
						(RadJav.OS.type == "ios"))
					{
						child._parent = null;
						doParentLOL = false;
					}
				}

				if (doParentLOL == true)
					child._parent = this;

				this._children.push(child);
			}

			/**
			 * Using the existing parameters in this object, create it.
			 * Theme Event: create
			 * Is Theme Event Asynchronous: Yes
			 * @return {Promise} The promise to execute when the creation is completed.
			 */
			public create(): Promise<GObject>
			{
				var promise = new Promise<GObject>(
					RadJav.keepContext(function(resolve, reject)
					{
						if (this.createOnPlatforms != null)
						{
							for (var key in this.createOnPlatforms)
							{
								if (key == "HTML5")
								{
									if (this.createOnPlatforms[key] == false)
									{
										resolve(this);

										return;
									}
								}
							}
						}

						var promise2 = RadJav.currentTheme.event(this.type, "create", this);

						if (promise2 == null)
							debugger;

						promise2.then(
							RadJav.keepContext(function(htmlObj)
							{
								this._html = htmlObj;
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
												RadJav.currentTheme.event(this.type, "on", this, key, func);
											}
										}

										if (this.onCreated != null)
											this.onCreated();

										resolve(this);
									}, this));
							}, this));
					}, this));

				return (promise);
			}

			/// Parse an XML object.
			static parseGObjectXML (xmlTag: RadJav.XML.XMLTag): GObject
			{
				let obj: GObject = new GObject ();

				if (xmlTag.hasAttribute ("name") == true)
					obj.name = xmlTag.getAttributeString ("name", "");

				let pos: RadJav.Vector2 = new RadJav.Vector2 ();
				let size: RadJav.Vector2 = new RadJav.Vector2 ();

				if (xmlTag.hasAttribute ("x") == true)
				{
					pos.x = xmlTag.getAttributeFloat ("x", 0);
					obj._transform.setPosition (pos);
				}

				if (xmlTag.hasAttribute ("y") == true)
				{
					pos.y = xmlTag.getAttributeFloat ("y", 0);
					obj._transform.setPosition (pos);
				}

				if (xmlTag.hasAttribute ("width") == true)
				{
					size.x = xmlTag.getAttributeFloat ("width", 0);
					obj._transform.setSize (size);
				}

				if (xmlTag.hasAttribute ("height") == true)
				{
					size.y = xmlTag.getAttributeFloat ("height", 0);
					obj._transform.setSize (size);
				}

				return (obj);
			}

			/** 
			* Set this object's font.
			* Theme Event: setFont
			* Is Theme Event Asynchronous: No
			* @param {RadJav.Font} font The font to set.
			*/
			setFont(font: Font): void
			{
				this._font = font;
				RadJav.currentTheme.eventSync(this.type, "setFont", this, font);
			}

			/** 
			* Get this object's font.
			* Theme Event: getFont
			* Is Theme Event Asynchronous: No
			* @return {RadJav.Font} The font.
			*/
			getFont(): Font
			{
				return RadJav.currentTheme.eventSync(this.type, "getFont", this);
			}

			/** 
			* Set the position of this object.
			* Theme Event: None
			* Is Theme Event Asynchronous: No
			* @param {Number/RadJav.Vector2} x The new position, or the new x coordinate of the new position.
			* @param {Number} [y=null] The new y coordinate.
			*/
			setPosition(x: number | RadJav.Vector2, y: number = 0): void
			{
				let pos: RadJav.Vector2 = new RadJav.Vector2 ();

				if (typeof (x) == "object")
				{
					pos.x = (<RadJav.Vector2>x).x;
					pos.y = (<RadJav.Vector2>x).y;
				}
				else
				{
					pos.x = x;
					pos.y = y;
				}

				RadJav.currentTheme.eventSync(this.type, "setPosition", this, pos);
				this._transform.setPosition(pos);
			}

			/** 
			* Get the position of this object.
			* Theme Event: None
			* Is Theme Event Asynchronous: No
			* @return {RadJav.Vector2} The position of this object.
			*/
			getPosition(): RadJav.Vector2
			{
				let pos: RadJav.Vector2 = this._transform.getPosition ();

				if (this._html != null)
					pos = RadJav.currentTheme.eventSync(this.type, "getPosition", this);

				return pos;
			}

			/** 
			 * Get the X position of this object.
			 * Theme Event: None
			 * Is Theme Event Asynchronous: No
			 * @return {RadJav.Vector2} The position of this object.
			 */
			getX(): number
			{
				let pos: number = this._transform.x;

				if (this._html != null)
					pos = RadJav.currentTheme.eventSync(this.type, "getPosition", this).x;

				return pos;
			}

			/** 
			 * Get the Y position of this object.
			 * Theme Event: None
			 * Is Theme Event Asynchronous: No
			 * @return {RadJav.Vector2} The position of this object.
			 */
			getY(): number 
			{
				let pos: number = this._transform.y;

				if (this._html != null)
					pos = RadJav.currentTheme.eventSync(this.type, "getPosition", this).y;

				return pos;
			}

			/** 
			 * Set the size of this object.
			 * Theme Event: None
			 * Is Theme Event Asynchronous: No
			 * @param {number/RadJav.Vector2} width The object's new size, or new width.
			 * @param {number} [height=null] The object's new height.
			 */
			setSize(width: number, height: number): void
			{
				RadJav.currentTheme.eventSync(this.type, "setSize", this, new RadJav.Vector2 (width, height));
				this._transform.setSize(width, height);
			}

			/** 
			 * Get the size of this object.
			 * Theme Event: None
			 * Is Theme Event Asynchronous: No
			 * @return {RadJav.Vector2} The size of this object.
			 */
			getSize(): RadJav.Vector2
			{
				let size: RadJav.Vector2 = this._transform.getSize ();

				if (this._html != null)
					size = RadJav.currentTheme.eventSync(this.type, "getSize", this);

				return size;
			}

			/** 
			 * Get the width of this object.
			 * Theme Event: None
			 * Is Theme Event Asynchronous: No
			 * @return {number} The width of this object.
			 */
			getWidth(): number
			{
				let size: number = this._transform.width;

				if (this._html != null)
					size = RadJav.currentTheme.eventSync(this.type, "getSize", this).x;

				return size;
			}

			/** 
			* Get the height of this object.
			* Theme Event: None
			* Is Theme Event Asynchronous: No
			* @return {number} The height of this object.
			*/
			getHeight(): number
			{
				let size: number = this._transform.height;

				if (this._html != null)
					size = RadJav.currentTheme.eventSync(this.type, "getSize", this).y;

				return size;
			}

			/** 
			* Set the object's text.
			* Theme Event: setText
			* Is Theme Event Asynchronous: Yes
			* @param {string} text The text to set.
			* @return {string} The text associated with this object.
			*/
			setText(text: string): void
			{
				RadJav.currentTheme.event(this.type, "setText", this, text);
			}

			/** 
			* Get the object's text.
			* Theme Event: getText
			* Is Theme Event Asynchronous: No
			* @return {string} The text associated with this object.
			*/
			getText(): string
			{
				return RadJav.currentTheme.eventSync(this.type, "getText", this);
			}

			/** 
			 * Get the parent.
			 * Theme Event: None
			 * Is Theme Event Asynchronous: No
			 * @return {RadJav.GUI.GObject} The parent of this object.
			 */
			getParent(): GObject
			{
				return this._parent;
			}

			/** 
			 * Get the HTML from this object.
			 * Theme Event: None
			 * Is Theme Event Asynchronous: No
			 * @return {Mixed} The HTML object associated with this object.
			 */
			getHTML(): any
			{
				return this._html;
			}

			/** 
			 * Set the visibility of this object.
			 * Theme Event: setVisibility
			 * Is Theme Event Asynchronous: Yes
			 * Parameters Passed to Theme Event: RadJav.GUI.GObject, Boolean
			 * @param {Boolean} visible The visibility of the object
			 */
			setVisibility(visible: boolean): void
			{
				RadJav.currentTheme.event(this.type, "setVisibility", this, visible);
			}

			/** 
			 * Get the visibility of this object.
			 * Theme Event: setVisibility
			 * Is Theme Event Asynchronous: No
			 * Parameters Passed to Theme Event: RadJav.GUI.GObject
			 * @return {Boolean} The visibility of this object
			 */
			getVisibility(): boolean
			{
				return RadJav.currentTheme.eventSync(this.type, "getVisibility", this);
			}

			/** 
			 * Show this object.
			 * Theme Event: setVisibility
			 * Is Theme Event Asynchronous: Yes
			 * Parameters Passed to Theme Event: RadJav.GUI.GObject, Boolean
			 */
			show(): void
			{
				this.setVisibility(true);
			}

			/** 
			 * Show this object.
			 * Theme Event: setVisibility
			 * Is Theme Event Asynchronous: Yes
			 * Parameters Passed to Theme Event: RadJav.GUI.GObject, Boolean
			 */
			hide(): void
			{
				this.setVisibility(false);
			}

			/** 
			 * Enable or disable this object.
			 * Theme Event: setEnabled
			 * Is Theme Event Asynchronous: Yes
			 * Parameters Passed to Theme Event: RadJav.GUI.GObject, Boolean
			 */
			setEnabled(enabled: boolean): void
			{
				RadJav.currentTheme.event(this.type, "setEnabled", this, enabled);
			}

			/** 
			 * Get whether or not this object is enabled.
			 * Theme Event: getEnabled
			 * Is Theme Event Asynchronous: No
			 * Parameters Passed to Theme Event: RadJav.GUI.GObject
			 * @return {Boolean} The enabled status of this object
			 */
			getEnabled(): boolean
			{
				return RadJav.currentTheme.eventSync(this.type, "getEnabled", this);
			}

			/** 
			 * Calls a function when an event is triggered.
			 * Theme Event: on
			 * Is Theme Event Asynchronous: No
			 * Parameters Passed to Theme Event: RadJav.GUI.GObject, string, Function
			 * @param {string} eventName The name of the event to trigger.
			 * @param {Function} func The function to execute.
			 * @return {Mixed} The result.
			 */
			on(eventName: string, func: Function): any
			{
				return RadJav.currentTheme.event(this.type, "on", this, eventName, func);
			}

			/** 
			 * Get the HTML dom object.
			 * Theme Event: getHTMLDOM
			 * Is Theme Event Asynchronous: No
			 * Parameters Passed to Theme Event: RadJav.GUI.GObject
			 * Available on platforms: HTML5
			 * @return {Mixed} The html dom object.
			 */
			getHTMLDOM(): any
			{
				return RadJav.currentTheme.eventSync(this.type, "getHTMLDOM", this);
			}
		}
	}
}
