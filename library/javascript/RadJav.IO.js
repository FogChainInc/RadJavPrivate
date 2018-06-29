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
var RadJav;
(function (RadJav) {
    var IO;
    (function (IO) {
        var FileIO = /** @class */ (function () {
            function FileIO() {
				this.fileListEvent = null;
                this._fileio = null;
				
                if (typeof this["_init"] == "function")
                    this["_init"]();
			}
            FileIO.prototype.isDir = function (path) {
			};
            FileIO.prototype.isFile = function (path) {
			};
            FileIO.prototype.isSymLink = function (path) {
			};
            FileIO.prototype.currentPath = function () {
			};
			FileIO.prototype.changePath = function (path) {
            };
            FileIO.prototype.exists = function (path) {
            };
            FileIO.prototype.createDir = function (path) {
            };
            FileIO.prototype.copyDir = function (src, dest, recursive) {
            };
            FileIO.prototype.renameDir = function (src, dest) {
            };
            FileIO.prototype.deleteDir = function (path) {
            };
            FileIO.prototype.isEmpty = function (path) {
            };
            FileIO.prototype.createSymLink = function (path, link) {
            };
            FileIO.prototype.copySymLink = function (src, dest) {
            };
            FileIO.prototype.renameSymLink = function (src, dest) {
            };
            FileIO.prototype.deleteSymLink = function (path) {
            };
            FileIO.prototype.copyFile = function (src, dest, overwrite) {
            };
            FileIO.prototype.renameFile = function (src, dest) {
            };
            FileIO.prototype.deleteFile = function (path) {
            };
			FileIO.prototype.listFiles = function (path, recursive) {
			};
			FileIO.prototype.listFilesAsync = function (path, recursive) {
			};
			FileIO.prototype.onFileList = function (evt) {
				this.fileListEvent = evt;
			};
            FileIO.prototype.normalizePath = function (path, basePath) {
			};

            return FileIO;
        }());
        IO.FileIO = FileIO;
    })(IO = RadJav.IO || (RadJav.IO = {}));
})(RadJav || (RadJav = {}));

var RadJav;
(function (RadJav) {
    var IO;
    (function (SerialComm) {
        var SerialComm = /** @class */ (function () {
            function SerialComm() {
               this._serialcomm = null;
				
                if (typeof this["_init"] == "function")
                    this["_init"]();
			}
            SerialComm.prototype.open = function (port, baud) {
            };
            SerialComm.prototype.isOpen = function () {
            };
            SerialComm.prototype.close = function () {
            };
            SerialComm.prototype.read = function () {
            };
            SerialComm.prototype.write = function (buffer, size) {
            };
        
            return SerialComm;
        }());
        IO.SerialComm = SerialComm;
    })(IO = RadJav.IO || (RadJav.IO = {}));
})(RadJav || (RadJav = {}));

var RadJav;
(function (RadJav) {
    var IO;
    (function (TextFile) {
        var TextFile = /** @class */ (function () {
            function TextFile() {
                this._testfile = null;
                    
                if (typeof this["_init"] == "function")
                    this["_init"]();
            }
            TextFile.prototype.writeFile = function (path, contents, type) {
            };
            TextFile.prototype.writeFileAsync = function (path, contents, type) {
            };
            TextFile.prototype.readFile = function (path) {
            };
            TextFile.prototype.readFileAsync = function (path) {
            };

            return TextFile;
        }());
        IO.TextFile = TextFile;
    })(IO = RadJav.IO || (RadJav.IO = {}));
})(RadJav || (RadJav = {}));

RadJav.IO.TextFile.read = 0;
RadJav.IO.TextFile.write = 1;
RadJav.IO.TextFile.append = 2;
