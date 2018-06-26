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
}
