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

interface DOMElement extends HTMLElement
{
}

namespace RadJav
{
	/** 
	* The IO class.
	* Available on platforms: Windows,Linux,OSX
	*/
	export class IO
	{
		/** 
		* Check to see if a directory exists.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to check.
		* @return Returns true if the directory exists.
		*/
		static isDir: (path: string) => boolean = null;

		/** 
		* Check to see if a file exists.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to check.
		* @return Returns true if the file exists.
		*/
		static isFile: (path: string) => boolean = null;

		/** 
		* Check to see if the file/directory is a symbolic link.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to check.
		* @return Returns true if its a symbolic link.
		*/
		static isSymLink: (path: string) => boolean = null;

		/** @method currentPath
		* Get the current directory path.
		* Available on platforms: Windows,Linux,OSX
		* @return {string} The current directory path.
		*/
		static currentPath: () => string = null;

		/** 
		* Change the current directory path.
		* Available on platforms: Windows,Linux,OSX
		* @param path Change the current directory path.
		*/
		static changePath: (path: string) => void = null;

		/** 
		* Checks if the file/directory exists.
		* Available on platforms: Windows,Linux,OSX
		* @param path Change the current directory path.
		* @return Returns true if its the file/directory exists.
		*/
		static exists: (path: string) => boolean = null;

		/** 
		* Create a directory.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to the directory to create.
		*/
		static createDir: (path: string) => void = null;

		/** 
		* Copy a directory.
		* Available on platforms: Windows,Linux,OSX
		* @param src The source directory to copy.
		* @param dest The destination to copy the directory to.
		* @param recursive Recursively copy if set to true.
		*/
		static copyDir: (src: string, dest: string, recursive?: boolean) => void = null;

		/** 
		* Rename a directory.
		* Available on platforms: Windows,Linux,OSX
		* @param src The directory to rename.
		* @param dest The new name of the directory.
		*/
		static renameDir: (src: string, dest: string) => void = null;

		/** 
		* Delete a directory.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to the directory to delete.
		*/
		static deleteDir: (path: string) => void = null;

		/** 
		* Check if a directory is empty.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to the directory to check.
		*/
		static isEmpty: (path: string) => boolean = null;

		/** 
		* Create a symbolic link.
		* Available on platforms: Windows,Linux,OSX
		* @param src The path to create a symbolic link to.
		* @param link The path to where the symbolic link can be found.
		*/
		static createSymLink: (path: string, link: string) => void = null;

		/** 
		* Copy a symbolic link.
		* Available on platforms: Windows,Linux,OSX
		* @param src The path to copy.
		* @param dest The destination to copy to.
		*/
		static copySymLink: (src: string, dest: string) => void = null;

		/** 
		* Rename a symbolic link.
		* Available on platforms: Windows,Linux,OSX
		* @param src The path to rename.
		* @param dest The new name.
		*/
		static renameSymLink: (src: string, dest: string) => void = null;

		/** 
		* Delete a symbolic link.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to the symbolic link to delete.
		*/
		static deleteSymLink: (path: string) => void = null;

		/** 
		* Copy a file.
		* Available on platforms: Windows,Linux,OSX
		* @param src The directory to copy.
		* @param dest The destination to copy the directory to.
		*/
		static copyFile: (src: string, dest: string) => void = null;

		/** 
		* Rename a file.
		* Available on platforms: Windows,Linux,OSX
		* @param src The file to rename.
		* @param dest The new name of the file.
		*/
		static renameFile: (src: string, dest: string) => void = null;

		/** 
		* Delete a file.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to the file to delete.
		*/
		static deleteFile: (path: string) => void = null;

		/** 
		* List files in a directory.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to list files.
		* @return The files and directories within that path.
		*/
		static listFiles: (path: string, recursive?: boolean) => string[] = null;

		/** 
		* Asynchronously list files in a directory.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to list files.
		* @return The files and directories within that path.
		*/
		static listFilesAsync: (asyncCallback: (newFileOrDir: string) => boolean, path: string, recursive?: boolean) => string[] = null;

		/** 
		* Normalize a file/directory path.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to normalize.
		* @param basePath The base path to normalize from.
		* @return The normalized path.
		*/
		static normalizePath: (path: string, basePath?: string) => string = null;

		/** 
		* Normalize an verify the file/directory path.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to normalize.
		* @param basePath The base path to normalize from.
		* @return The normalized path.
		*/
		static normalizeAndVerifyPath: (path: string, basePath?: string) => string = null;

		/** 
		* Normalize a file/directory path relative to the current directory path.
		* Available on platforms: Windows,Linux,OSX
		* @param path The path to normalize.
		* @return The normalized path.
		*/
		static normalizeCurrentPath: (path: string) => string = null;
	}

	export namespace IO
	{
		/** 
		* Handles serial communications.
		* Available on platforms: Windows,Linux,OSX
		*/
		export class SerialComm
		{
			/** 
			* Get the serial port.
			* Available on platforms: Windows,Linux,OSX
			* @return The port being used.
			*/
			getPort: () => string;

			/** 
			* Get the baud.
			* Available on platforms: Windows,Linux,OSX
			* @return The baud being used.
			*/
			getBaud: () => number;

			/** 
			* Get the byte size being used.
			* Available on platforms: Windows,Linux,OSX
			* @return The byte size being used.
			*/
			getByteSize: () => number;

			/** 
			* Get the stop bits being used.
			* Available on platforms: Windows,Linux,OSX
			* @return The stop bits being used.
			*/
			getStopBits: () => number;

			/** 
			* Get the parity being used.
			* Available on platforms: Windows,Linux,OSX
			* @return The parity being used.
			*/
			getParity: () => number;

			/** 
			* Open the serial communications.
			* Available on platforms: Windows,Linux,OSX
			* @return Whether or not communications were able to be established.
			*/
			open: () => boolean;

			/** 
			* Check if serial communications were opened.
			* Available on platforms: Windows,Linux,OSX
			* @return Whether or not communications were able to be established.
			*/
			isOpen: () => boolean;

			/** 
			* Read from the opened port.
			* Available on platforms: Windows,Linux,OSX
			* @param bufferSize The size of the buffer to read in bytes.
			* @return The string buffer from the opened port.
			*/
			read: (bufferSize: number) => string;

			/** 
			* Write to the opened port.
			* Available on platforms: Windows,Linux,OSX
			* @param buffer The string buffer to write.
			* @param bufferSize The number of bytes to write from the buffer.
			* @return The number of bytes written.
			*/
			write: (buffer: number, bufferSize?: number) => number;

			/** 
			* Close the opened port.
			* Available on platforms: Windows,Linux,OSX
			*/
			close: () => void;
		}

		/// The file or stream access type.
		export enum AccessType
		{
			read = 0,
			write = 1,
			append = 2
		}

		/** 
		* Handles text files.
		* Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class TextFile
		{
			/// The path to the text file.
			filePath: string;
			/// How the file is open.
			writeType: RadJav.IO.AccessType;

			constructor (filePath: string, writeType: RadJav.IO.AccessType = RadJav.IO.AccessType.write)
			{
				this.filePath = filePath;
				this.writeType = writeType;
			}

			/** 
			* Write to a text file.
			* @param content The content to write.
			*/
			writeFile: (content: string) => void;

			/** 
			* Write to a text file.
			* @param path The path to the file to write to.
			* @param content The content to write.
			*/
			static writeToFile (path: string, content: string, 
					writeType: RadJav.IO.AccessType = RadJav.IO.AccessType.write): void
			{
				let file: TextFile = new TextFile (path, writeType);

				file.writeFile (content);
			}

			/** 
			* Write to a text file asynchronously.
			* @param content The content to write.
			*/
			writeFileAsync: (content: string) => void;

			/** 
			* Read from a text file.
			* @param path The path to the file to read from.
			* @return The content read from the text file.
			*/
			readFile: () => string;

			/** 
			* Read from a text file.
			* @param path The path to the file to read from.
			* @return The content read from the text file.
			*/
			static readFromFile (path: string): string
			{
				let file: TextFile = new TextFile (path);

				return (file.readFile ());
			}

			/** 
			* Read from a text file asynchronously.
			* @return The content read from the text file.
			*/
			readFileAsync: () => string;
		}

		/** 
		* Handles stream files.
		* Available on platforms: Windows,Linux,OSX,iOS,Android
		*/
		export class StreamFile
		{
			/** 
			* Write to a stream file.
			* @param path The path to the file to write to.
			* @param content The content to write.
			*/
			writeStream: (path: string, content: string) => void;
			
			/** 
			* Write to a stream file asynchronously.
			* @param path The path to the file to write to.
			* @param content The content to write.
			*/
			writeStreamAsync: (path: string, content: string) => void;

			/** 
			* Read from a stream file.
			* @param path The path to the file to read from.
			* @return The content read from the text file.
			*/
			readStream: (path: string) => string;
			
			/** 
			* Read from a stream file asynchronously.
			* @param path The path to the file to read from.
			* @return The content read from the text file.
			*/
			readStreamAsync: (path: string) => string;

			/// Read from a file.
			static read: number = 1;
			/// Write to a file.
			static write: number = 2;
			/// Append to a file.
			static append: number = 3;
		}
	}

	/// Handles XML.
	export namespace XML
	{
		/** 
		* Opens XML files.
		* Available on platforms: Windows,Linux,OSX,iOS,Android,HTML5
		*/
		export class XMLFile
		{
			/// The XML parser.
			public _parser: DOMParser;
			/// The root node.
			public _root: XMLTag;

			/// The XML file that has been loaded.
			public _loadedFile: XMLParser;

			constructor ()
			{
				this._parser = null;
				this._root = null;
				this._loadedFile = null;

				if (typeof this["_init"] == "function")
					this["_init"] ();
			}

			/// Load a XML file.
			parseXMLFile (filePath: string): Promise<string>
			{
				let promise: Promise<string> = new Promise<string> (RadJav.keepContext (function (resolve, reject, file)
					{
						let path: string = file[0];

						if (RadJav.OS.HTML5 != null)
						{
							return (RadJav.Net.httpRequest (path).then (RadJav.keepContext (function (data)
								{
									this._parseXML (data);
									resolve (data);
								}, this)));
						}
						else
						{
							let data = new RadJav.IO.TextFile ().readFile (path);
							this._parseXML (data);

							resolve (data);
						}
					}, this, [filePath]));

				return (promise);
			}

			/// Load XML from a string.
			parseXML (xmlString: string): void
			{
				if (RadJav.OS.HTML5 != null)
				{
					this._parser = new DOMParser ();
					this._loadedFile = new XMLParser ();
					this._loadedFile.xmlFile = this._parser.parseFromString (xmlString, "text/xml");
					this._root = new XMLTag (this._loadedFile);
				}
			}

			/// Get the root tag.
			getRoot (): XMLTag
			{
				return (this._root);
			}

			/// Load a XML file.
			static loadFile (filePath: string): Promise<XMLFile>
			{
				let promise: Promise<XMLFile> = new Promise<XMLFile> (RadJav.keepContext (function (resolve, reject, file)
					{
						let path: string = file[0];
						let xmlFile: XMLFile = new XMLFile ();
						let promise2: Promise<void> = xmlFile.parseXMLFile (path).then (function ()
							{
								resolve (this);
							});

						return (promise2);
					}, this, [filePath]));

				return (promise);
			}

			/// Load a XML string.
			static loadString (xmlString: string): XMLFile
			{
				let xmlFile: XMLFile = new XMLFile ();
				xmlFile.parseXML (xmlString);

				return (xmlFile);
			}
		}

		/// This is mostly for maintaining consistency across
		/// HTML5, desktop, and mobile platforms.
		export class XMLParser
		{
			/// The XML file that has been loaded.
			public xmlFile: any;

			constructor (xmlFile: any = null)
			{
				this.xmlFile = xmlFile;
			}
		}

		/// XML tag.
		export class XMLTag
		{
			/// The tag name.
			public _tag: string;
			/// The attributes associated with this tag.
			public _attributes: { [name: string]: XMLAttribute };
			/// The value in this tag.
			public _value: string;
			/// This tag's children.
			public _children: XMLTag[];

			/// The XML file that has been loaded.
			public _loadedFile: XMLParser;

			constructor (tag: string | XMLParser)
			{
				if (RadJav.OS.HTML5 != null)
				{
					if (typeof (tag) == "string")
					{
						this._tag = (<string>tag);
						this._attributes = {};
						this._value = "";
						this._children = [];
						this._loadedFile = null;
					}
					else
					{
						let domTag: DOMElement =  (<DOMElement>(<XMLParser>tag).xmlFile.firstChild);
						this._loadedFile = (<XMLParser>tag);
						this._tag = domTag.tagName;
						this._attributes = {};

						for (let iIdx = 0; iIdx < domTag.attributes.length; iIdx++)
						{
							let attribute = domTag.attributes[iIdx];
							let attr: XMLAttribute = new XMLAttribute (attribute.name, attribute.value);
							this._attributes[attribute.name] = attr;
						}

						this._value = domTag.textContent;
						this._children = [];

						for (let iIdx = 0; iIdx < domTag.childNodes.length; iIdx++)
						{
							let domChild: DOMElement = (<DOMElement>domTag.childNodes[iIdx]);
							let child: XMLTag = new XMLTag (new XMLParser (domChild));

							this._children.push (child);
						}
					}
				}
				else
				{
					if (typeof this["_init"] == "function")
						this["_init"] (tag);
				}
			}

			/// Get the children.
			getChildren (): XMLTag[]
			{
				return (this._children);
			}

			/// Get the children.
			getAttributes (): { [name: string]: XMLAttribute }
			{
				return (this._attributes);
			}

			/// Set the current tag name.
			setTag (name: string): void
			{
				this._tag = name;
			}

			/// Get the current tag name.
			getTag (): string
			{
				return (this._tag);
			}

			/// Get the current value.
			setValue (value: string): void
			{
				this._value = value;
			}

			/// Get the current value.
			getValue (): string
			{
				return (this._value);
			}

			/// Get the child tags in this tag.
			getTags (tag: string): XMLTag[]
			{
				let tags: XMLTag[] = [];

				for (let iIdx = 0; iIdx < this._children.length; iIdx++)
				{
					let child: XMLTag = this._children[iIdx];

					if (child._tag == tag)
						tags.push (child);
				}

				return (tags);
			}

			/// Set an attribute for this tag.
			setAttribute (attribute: string, value: string): void
			{
				if (this._attributes[attribute] == undefined)
					this._attributes[attribute] = new XMLAttribute (attribute, value);
				else
					this._attributes[attribute].setValue (value);
			}

			/// Checks if an attribute has been set.
			hasAttribute (attribute: string): boolean
			{
				if (this._attributes[attribute] == undefined)
					return (false);

				return (true);
			}

			/// Get an attribute from this tag.
			getAttribute (attribute: string): XMLAttribute
			{
				if (this._attributes[attribute] == undefined)
					throw new Error ("Attribute does not exist!");

				return (this._attributes[attribute]);
			}

			/// Get an attribute from this tag.
			getAttributeString (attribute: string, defaultValue: string = ""): string
			{
				let value: string = defaultValue;

				try
				{
					let attrValue: XML.XMLAttribute = this.getAttribute (attribute);
					value = attrValue.getValue ();
				}
				catch (ex)
				{
				}

				return (value);
			}

			/// Get an attribute integer value from this tag.
			getAttributeInt (attribute: string, defaultValue: number = 0): number
			{
				let value: number = defaultValue;

				try
				{
					let attrValue: XML.XMLAttribute = this.getAttribute (attribute);
					value = attrValue.toInt ();
				}
				catch (ex)
				{
				}

				return (value);
			}

			/// Get an attribute float value from this tag.
			getAttributeFloat (attribute: string, defaultValue: number = 0.0): number
			{
				let value: number = defaultValue;

				try
				{
					let attrValue: XML.XMLAttribute = this.getAttribute (attribute);
					value = attrValue.toFloat ();
				}
				catch (ex)
				{
				}

				return (value);
			}

			/// Get a boolean result from an attribute.
			getAttributeBoolean (attribute: string, defaultValue: boolean = false): boolean
			{
				let value: boolean = defaultValue;

				try
				{
					let attrValue: XML.XMLAttribute = this.getAttribute (attribute);
					value = attrValue.toBoolean ();
				}
				catch (ex)
				{
				}

				return (value);
			}

			/// Convert this tag to a string.
			toString (): string
			{
				let result: string = "<" + this._tag + " ";

				for (let attr in this._attributes)
				{
					let attribute: XMLAttribute = this._attributes[attr];
					result += attribute.toString () + " ";
				}

				if ((this._value != "") || (this._children.length > 0))
				{
					result += ">" + this._value;

					if (this._value != "")
						result += " ";

					for (let iIdx = 0; iIdx < this._children.length; iIdx++)
					{
						let child: XMLTag = this._children[iIdx];
						result += child.toString () + " ";
					}

					result += "</" + this._tag + ">";
				}
				else
					result += "/>";

				return (result);
			}
		}

		/// XML attribute.
		export class XMLAttribute
		{
			/// The attribute's name.
			public _name: string;
			/// The value of the attribute.
			public _value: string;

			constructor (name: string, value: string)
			{
				this._name = name;
				this._value = value;
			}

			/// Set the value of this attribute.
			setValue (value: string): void
			{
				this._value = value;
			}

			/// Get the value of the attribute.
			getValue (): string
			{
				return (this._value);
			}

			/// Get the integer value of the attribute.
			toInt (): number
			{
				return (parseInt (this._value));
			}

			/// Get the float value of the attribute.
			toFloat (): number
			{
				return (parseFloat (this._value));
			}

			/// Get the boolean value of the attribute.
			toBoolean (): boolean
			{
				return (parseBoolean (this._value));
			}

			/// Convert attribute to a string.
			toString (): string
			{
				return (this._name + " = \"" + this._value + "\"");
			}
		}
		}
}
