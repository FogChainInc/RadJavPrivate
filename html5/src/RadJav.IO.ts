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

namespace RadJav {
    /** @class RadJav.IO
     * The IO class.
     * Available on platforms: Windows,Linux,OSX
     */
    export class IO {
      /** @method isDir
       * Check to see if a directory exists.
       * Available on platforms: Windows,Linux,OSX
       * @param {string} path The path to check.
       * @return {Boolean} Returns true if the directory exists.
       */
      static isDir(path: string): boolean { return; }

      /** @method isFile
       * Check to see if a file exists.
       * Available on platforms: Windows,Linux,OSX
       * @param {string} path The path to check.
       * @return {Boolean} Returns true if the file exists.
       */
      static isFile(path: string): boolean { return; }

      /** @method mkdir
       * Make a directory.
       * Available on platforms: Windows,Linux,OSX
       * @param {string} path The path to the directory to create.
       */
      static mkdir(path: string): void {}

      /** @method deleteFile
       * Delete a file.
       * Available on platforms: Windows,Linux,OSX
       * @param {string} path The path to the file to delete.
       */
      static deleteFile(path: string): void {}

      /** @method listFiles
       * List files in a directory.
       * Available on platforms: Windows,Linux,OSX
       * @param {string} path The path to list files.
	   * @return {string[]} The files and directories within that path.
       */
      static listFiles(path: string): string[] { return; }

      /** @method normalizePath
       * Normalize a file/directory path.
       * Available on platforms: Windows,Linux,OSX
       * @param {string} path The path to normalize.
	   * @return {string} The normalized path.
       */
      static normalizePath(path: string): string { return; }

      /** @method copyDir
       * Copy a directory.
       * Available on platforms: Windows,Linux,OSX
       * @param {string} src The directory to copy.
       * @param {string} dest The destination to copy the directory to.
       */
      static copyDir(src: string, dest: string): void {}

      /** @method copyFile
       * Copy a file.
       * Available on platforms: Windows,Linux,OSX
       * @param {string} src The directory to copy.
       * @param {string} dest The destination to copy the directory to.
       */
      static copyFile(src: string, dest: string): void {}
    }

    export namespace IO {
      /** @class RadJav.IO.SerialComm
       * Handles serial communications.
       * Available on platforms: Windows,Linux,OSX
       */
      export class SerialComm {
        /** @method getPort
         * Get the serial port.
         * Available on platforms: Windows,Linux,OSX
         * @return {string} The port being used.
         */
        getPort(): string {
          return;
        }

        /** @method getBaud
         * Get the baud.
         * Available on platforms: Windows,Linux,OSX
         * @return {string} The baud being used.
         */
        getBaud(): Number { return; }

        /** @method getByteSize
         * Get the byte size being used.
         * Available on platforms: Windows,Linux,OSX
         * @return {string} The byte size being used.
         */
        getByteSize(): Number {return;}

        /** @method getStopBits
         * Get the stop bits being used.
         * Available on platforms: Windows,Linux,OSX
         * @return {string} The stop bits being used.
         */
        getStopBits(): Number { return; }

        /** @method getParity
         * Get the parity being used.
         * Available on platforms: Windows,Linux,OSX
         * @return {string} The parity being used.
         */
        getParity(): Number { return; }

        /** @method open
         * Open the serial communications.
         * Available on platforms: Windows,Linux,OSX
         * @return {Boolean} Whether or not communications were able to be established.
         */
        open(): boolean { return; }

        /** @method isOpen
         * Check if serial communications were opened.
         * Available on platforms: Windows,Linux,OSX
         * @return {Boolean} Whether or not communications were able to be established.
         */
        isOpen(): boolean { return; }

        /** @method read
         * Read from the opened port.
         * Available on platforms: Windows,Linux,OSX
         * @param {Number} bufferSize The size of the buffer to read in bytes.
         * @return {string} The string buffer from the opened port.
         */
        read(bufferSize: Number): string { return; }

        /** @method write
         * Write to the opened port.
         * Available on platforms: Windows,Linux,OSX
         * @param {Number} buffer The string buffer to write.
         * @param {Number} [bufferSize=buffer.length] The number of bytes to write from the buffer.
         * @return {Number} The number of bytes written.
         */
        write(buffer: Number, bufferSize?: Number): Number { return; }

        /** @method close
         * Close the opened port.
         * Available on platforms: Windows,Linux,OSX
         */
        close(): void {}
      }

      /** @class RadJav.IO.TextFile
       * Handles text files.
       * Available on platforms: Windows,Linux,OSX
       */
      export class TextFile {
        /** @method writeTextToFile
         * Write to a text file.
         * Available on platforms: Windows,Linux,OSX
         * @param {string} path The path to the file to write to.
         * @param {string} content The content to write.
         */
        static writeTextToFile(path: string, content: string): void {}

        /** @method readEntireFile
         * Read from a text file.
         * Available on platforms: Windows,Linux,OSX
         * @param {string} path The path to the file to read from.
         * @return {string} The content read from the text file.
         */
        static readEntireFile(path: string): string { return; }

        /** @property {Number} [read=1]
         * @static
         * Read from a file.
         */
        static read: Number;
        /** @property {Number} [write=2]
         * @static
         * Write to a file.
         */
        static write: Number;
        /** @property {Number} [append=3]
         * @static
         * Append to a file.
         */
        static append: Number;
      }
    }

	/// Handles XML.
	export namespace XML
	{
		/** @class RadJav.IO.XML.XMLFile
		* Opens XML files.
		*/
		export class XMLFile
		{
			/// The XML parser.
			public parser: DOMParser;
			/// The root node.
			public root: XMLTag;

			/// The XML file that has been loaded.
			protected xmlFile: XMLDocument;

			/// Load a XML file.
			loadXMLFile (filePath: string): Promise<string>
			{
				let promise: Promise<string> = new Promise<string> (RadJav.keepContext (function (resolve, reject, file)
					{
						let path: string = file[0];

						if (RadJav.OS.HTML5 != null)
						{
							return (RadJav.Net.httpRequest (path).then (function (data)
								{
									this.loadXML (data);
									resolve (data);
								}));
						}
						else
						{
							let data = RadJav.IO.TextFile.readEntireFile (path);
							this.loadXML (data);

							resolve (data);
						}
					}, this, [filePath]));

				return (promise);
			}

			/// Load XML from a string.
			loadXML (xmlString: string): void
			{
				if (RadJav.OS.HTML5 != null)
				{
					this.parser = new DOMParser ();
					this.xmlFile = this.parser.parseFromString (xmlString, "text/xml");
					this.root = new XMLTag ((<DOMElement>this.xmlFile.firstChild));
				}
			}
		}

		/// XML tag.
		export class XMLTag
		{
			/// The tag name.
			public tag: string;
			/// The attributes associated with this tag.
			public attributes: { [name: string]: XMLAttribute };
			/// The value in this tag.
			public value: string;
			/// This tag's children.
			public children: XMLTag[];

			constructor (tag: string | DOMElement)
			{
				if (typeof (tag) == "string")
				{
					this.tag = (<string>tag);
					this.attributes = {};
					this.value = "";
					this.children = [];
				}
				else
				{
					let domTag: DOMElement = (<DOMElement>tag);
					this.tag = domTag.tagName;
					this.attributes = {};

					for (let iIdx = 0; iIdx < domTag.attributes.length; iIdx++)
					{
						let attribute = domTag.attributes[iIdx];
						let attr: XMLAttribute = new XMLAttribute (attribute.name, attribute.value);
						this.attributes[attribute.name] = attr;
					}

					this.value = domTag.textContent;
					this.children = [];

					for (let iIdx = 0; iIdx < domTag.childNodes.length; iIdx++)
					{
						let domChild: DOMElement = (<DOMElement>domTag.childNodes[iIdx]);
						let child: XMLTag = new XMLTag (domChild);

						this.children.push (child);
					}
				}
			}

			/// Get the child tags in this tag.
			getTags (tag: string): XMLTag[]
			{
				let tags: XMLTag[] = [];

				for (let iIdx = 0; iIdx < this.children.length; iIdx++)
				{
					let child: XMLTag = this.children[iIdx];

					if (child.tag == tag)
						tags.push (child);
				}

				return (tags);
			}

			/// Set an attribute for this tag.
			setAttribute (attribute: string, value: string): void
			{
				if (this.attributes[attribute] == undefined)
					this.attributes[attribute] = new XMLAttribute (attribute, value);
				else
					this.attributes[attribute].value = value;
			}

			/// Get an attribute from this tag.
			getAttribute (attribute: string): XMLAttribute
			{
				if (this.attributes[attribute] == undefined)
					throw new Error ("Attribute does not exist!");

				return (this.attributes[attribute]);
			}

			/// Get an attribute from this tag.
			getAttributeString (attribute: string): string
			{
				return (this.getAttribute (attribute).getValue ());
			}

			/// Get an attribute integer value from this tag.
			getAttributeInt (attribute: string): number
			{
				return (this.getAttribute (attribute).toInt ());
			}

			/// Get an attribute float value from this tag.
			getAttributeFloat (attribute: string): number
			{
				return (this.getAttribute (attribute).toFloat ());
			}

			/// Get a boolean result from an attribute.
			getAttributeBoolean (attribute: string): boolean
			{
				return (this.getAttribute (attribute).toBoolean ());
			}

			/// Convert this tag to a string.
			toString (): string
			{
				let result: string = "<" + this.tag + " ";

				for (let attr in this.attributes)
				{
					let attribute: XMLAttribute = this.attributes[attr];
					result += attribute.toString () + " ";
				}

				if ((this.value != "") || (this.children.length > 0))
				{
					result += ">" + this.value;

					if (this.value != "")
						result += " ";

					for (let iIdx = 0; iIdx < this.children.length; iIdx++)
					{
						let child: XMLTag = this.children[iIdx];
						result += this.child.toString () + " ";
					}

					result += "</" + this.tag + ">";
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
			public name: string;
			/// The value of the attribute.
			public value: string;

			constructor (name: string, value: string)
			{
				this.name = name;
				this.value = value;
			}

			/// Get the value of the attribute.
			getValue (): string
			{
				return (this.value);
			}

			/// Get the integer value of the attribute.
			toInt (): number
			{
				return (parseInt (this.value));
			}

			/// Get the float value of the attribute.
			toFloat (): number
			{
				return (parseFloat (this.value));
			}

			/// Get the boolean value of the attribute.
			toBoolean (): boolean
			{
				return (parseBoolean (this.value));
			}

			/// Convert attribute to a string.
			toString (): string
			{
				return (this.name + " = \"" + this.value + "\"");
			}
		}
	}
}
