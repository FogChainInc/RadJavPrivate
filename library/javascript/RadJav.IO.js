var RadJav;
(function (RadJav) {
    var IO = (function () {
        function IO() {
        }
        IO.isDir = function (path) { return; };
        IO.isFile = function (path) { return; };
        IO.isSymLink = function (path) { return; };
        IO.currentPath = function () { return; };
        IO.changePath = function (path) { return; };
        IO.exists = function (path) { return; };
        IO.createDir = function (path) { };
        IO.copyDir = function (src, dest, recursive) {
            if (recursive === void 0) { recursive = true; }
        };
        IO.renameDir = function (src, dest) { };
        IO.deleteDir = function (path) { };
        IO.isEmpty = function (path) { return; };
        IO.createSymLink = function (path, link) { };
        IO.copySymLink = function (src, dest) { };
        IO.renameSymLink = function (src, dest) { };
        IO.deleteSymLink = function (path) { };
        IO.copyFile = function (src, dest) { };
        IO.renameFile = function (src, dest) { };
        IO.deleteFile = function (path) { };
        IO.listFiles = function (path, recursive) {
            if (recursive === void 0) { recursive = true; }
            return;
        };
        IO.listFilesAsync = function (asyncCallback, path, recursive) {
            if (recursive === void 0) { recursive = true; }
            return;
        };
        IO.normalizePath = function (path, basePath) {
            if (basePath === void 0) { basePath = ""; }
            return;
        };
        IO.normalizeAndVerifyPath = function (path, basePath) {
            if (basePath === void 0) { basePath = ""; }
            return;
        };
        IO.normalizeCurrentPath = function (path) { return; };
        
        IO.watchFile = function (asyncCallback, file) { };
		IO.watchFolder = function (asyncCallback, path) { };
		IO.unwatch = function (file) { };

        return IO;
    }());
    RadJav.IO = IO;
    (function (IO) {
        var SerialComm = (function () {
            function SerialComm() {
            }
            SerialComm.prototype.getPort = function () { return; };
            SerialComm.prototype.getBaud = function () { return; };
            SerialComm.prototype.getByteSize = function () { return; };
            SerialComm.prototype.getStopBits = function () { return; };
            SerialComm.prototype.getParity = function () { return; };
            SerialComm.prototype.open = function () { return; };
            SerialComm.prototype.isOpen = function () { return; };
            SerialComm.prototype.read = function (bufferSize) { return; };
            SerialComm.prototype.write = function (buffer, bufferSize) { return; };
            SerialComm.prototype.close = function () { };
            return SerialComm;
        }());
        IO.SerialComm = SerialComm;
        
        var TextFile = (function () {
            function TextFile() {
            }
            TextFile.writeFile = function (path, content) { };
            TextFile.writeFileAsync = function (path, content) { };
            TextFile.readFile = function (path) { return; };
            TextFile.readFileAsync = function (path) { return; };
            return TextFile;
        }());
        IO.TextFile = TextFile;
        
        var StreamFile = (function () {
            function StreamFile() {
            }
            StreamFile.writeStream = function (path, content) { };
            StreamFile.writeStreamAsync = function (path, content) { };
            StreamFile.readStream = function (path) { return; };
            StreamFile.readStreamAsync = function (path) { return; };
            return StreamFile;
        }());
        IO.StreamFile = StreamFile;
    
    })(IO = RadJav.IO || (RadJav.IO = {}));
    var XML;
    (function (XML) {
        var XMLFile = (function () {
            function XMLFile() {
                this.parser = null;
                this.root = null;
                this.xmlFile = null;
                if (typeof this["_init"] == "function")
                    this["_init"]();
            }
            XMLFile.prototype.loadXMLFile = function (filePath) {
                var promise = new Promise(RadJav.keepContext(function (resolve, reject, file) {
                    var path = file[0];
                    if (RadJav.OS.HTML5 != null) {
                        return (RadJav.Net.httpRequest(path).then(function (data) {
                            this.loadXML(data);
                            resolve(data);
                        }));
                    }
                    else {
                        var data = RadJav.IO.TextFile.readFile(path);
                        this.loadXML(data);
                        resolve(data);
                    }
                }, this, [filePath]));
                return (promise);
            };
            XMLFile.prototype.loadXML = function (xmlString) {
                if (RadJav.OS.HTML5 != null) {
                    this.parser = new DOMParser();
                    this.xmlFile = this.parser.parseFromString(xmlString, "text/xml");
                    this.root = new XMLTag(this.xmlFile.firstChild);
                }
            };
            return XMLFile;
        }());
        XML.XMLFile = XMLFile;
        var XMLTag = (function () {
            function XMLTag(tag) {
                if (RadJav.OS.HTML5 != null) {
                    if (typeof (tag) == "string") {
                        this.tag = tag;
                        this.attributes = {};
                        this.value = "";
                        this.children = [];
                    }
                    else {
                        var domTag = tag;
                        this.tag = domTag.tagName;
                        this.attributes = {};
                        for (var iIdx = 0; iIdx < domTag.attributes.length; iIdx++) {
                            var attribute = domTag.attributes[iIdx];
                            var attr = new XMLAttribute(attribute.name, attribute.value);
                            this.attributes[attribute.name] = attr;
                        }
                        this.value = domTag.textContent;
                        this.children = [];
                        for (var iIdx = 0; iIdx < domTag.childNodes.length; iIdx++) {
                            var domChild = domTag.childNodes[iIdx];
                            var child = new XMLTag(domChild);
                            this.children.push(child);
                        }
                    }
                }
                else {
                    if (typeof this["_init"] == "function")
                        this["_init"](tag);
                }
            }
            XMLTag.prototype.getTags = function (tag) {
                var tags = [];
                for (var iIdx = 0; iIdx < this.children.length; iIdx++) {
                    var child = this.children[iIdx];
                    if (child.tag == tag)
                        tags.push(child);
                }
                return (tags);
            };
            XMLTag.prototype.setAttribute = function (attribute, value) {
                if (this.attributes[attribute] == undefined)
                    this.attributes[attribute] = new XMLAttribute(attribute, value);
                else
                    this.attributes[attribute].value = value;
            };
            XMLTag.prototype.getAttribute = function (attribute) {
                if (this.attributes[attribute] == undefined)
                    throw new Error("Attribute does not exist!");
                return (this.attributes[attribute]);
            };
            XMLTag.prototype.getAttributeString = function (attribute) {
                return (this.getAttribute(attribute).getValue());
            };
            XMLTag.prototype.getAttributeInt = function (attribute) {
                return (this.getAttribute(attribute).toInt());
            };
            XMLTag.prototype.getAttributeFloat = function (attribute) {
                return (this.getAttribute(attribute).toFloat());
            };
            XMLTag.prototype.getAttributeBoolean = function (attribute) {
                return (this.getAttribute(attribute).toBoolean());
            };
            XMLTag.prototype.toString = function () {
                var result = "<" + this.tag + " ";
                for (var attr in this.attributes) {
                    var attribute = this.attributes[attr];
                    result += attribute.toString() + " ";
                }
                if ((this.value != "") || (this.children.length > 0)) {
                    result += ">" + this.value;
                    if (this.value != "")
                        result += " ";
                    for (var iIdx = 0; iIdx < this.children.length; iIdx++) {
                        var child = this.children[iIdx];
                        result += child.toString() + " ";
                    }
                    result += "</" + this.tag + ">";
                }
                else
                    result += "/>";
                return (result);
            };
            return XMLTag;
        }());
        XML.XMLTag = XMLTag;
        var XMLAttribute = (function () {
            function XMLAttribute(name, value) {
                this.name = name;
                this.value = value;
            }
            XMLAttribute.prototype.getValue = function () {
                return (this.value);
            };
            XMLAttribute.prototype.toInt = function () {
                return (parseInt(this.value));
            };
            XMLAttribute.prototype.toFloat = function () {
                return (parseFloat(this.value));
            };
            XMLAttribute.prototype.toBoolean = function () {
                return (parseBoolean(this.value));
            };
            XMLAttribute.prototype.toString = function () {
                return (this.name + " = \"" + this.value + "\"");
            };
            return XMLAttribute;
        }());
        XML.XMLAttribute = XMLAttribute;
    })(XML = RadJav.XML || (RadJav.XML = {}));
})(RadJav || (RadJav = {}));
