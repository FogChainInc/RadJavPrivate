var RadJav;
(function (RadJav) {
    var IO = (function () {
        function IO() {
        }
        IO.isDir = null;
        IO.isFile = null;
        IO.isSymLink = null;
        IO.currentPath = null;
        IO.changePath = null;
        IO.exists = null;
        IO.createDir = null;
        IO.copyDir = null;
        IO.renameDir = null;
        IO.deleteDir = null;
        IO.isEmpty = null;
        IO.createSymLink = null;
        IO.copySymLink = null;
        IO.renameSymLink = null;
        IO.deleteSymLink = null;
        IO.copyFile = null;
        IO.renameFile = null;
        IO.deleteFile = null;
        IO.listFiles = null;
        IO.listFilesAsync = null;
        IO.normalizePath = null;
        IO.normalizeAndVerifyPath = null;
        IO.normalizeCurrentPath = null;
        return IO;
    }());
    RadJav.IO = IO;
    (function (IO) {
        var SerialComm = (function () {
            function SerialComm() {
            }
            return SerialComm;
        }());
        IO.SerialComm = SerialComm;
        var AccessType;
        (function (AccessType) {
            AccessType[AccessType["read"] = 0] = "read";
            AccessType[AccessType["write"] = 1] = "write";
            AccessType[AccessType["append"] = 2] = "append";
        })(AccessType = IO.AccessType || (IO.AccessType = {}));
        var TextFile = (function () {
            function TextFile(filePath, writeType) {
                if (writeType === void 0) { writeType = RadJav.IO.AccessType.write; }
                this.filePath = filePath;
                this.writeType = writeType;
            }
            TextFile.writeToFile = function (path, content, writeType) {
                if (writeType === void 0) { writeType = RadJav.IO.AccessType.write; }
                var file = new TextFile(path, writeType);
                file.writeFile(content);
            };
            TextFile.readFromFile = function (path) {
                var file = new TextFile(path);
                return (file.readFile());
            };
            return TextFile;
        }());
        IO.TextFile = TextFile;
        var StreamFile = (function () {
            function StreamFile() {
            }
            StreamFile.read = 1;
            StreamFile.write = 2;
            StreamFile.append = 3;
            return StreamFile;
        }());
        IO.StreamFile = StreamFile;
    })(IO = RadJav.IO || (RadJav.IO = {}));
    var XML;
    (function (XML) {
        var XMLFile = (function () {
            function XMLFile() {
                this._parser = null;
                this._root = null;
                this._loadedFile = null;
                if (typeof this["_init"] == "function")
                    this["_init"]();
            }
            XMLFile.prototype.parseXMLFile = function (filePath) {
                var promise = new Promise(RadJav.keepContext(function (resolve, reject, file) {
                    var path = file[0];
                    if (RadJav.OS.HTML5 != null) {
                        return (RadJav.Net.httpRequest(path).then(RadJav.keepContext(function (data) {
                            this._parseXML(data);
                            resolve(data);
                        }, this)));
                    }
                    else {
                        var data = new RadJav.IO.TextFile().readFile(path);
                        this._parseXML(data);
                        resolve(data);
                    }
                }, this, [filePath]));
                return (promise);
            };
            XMLFile.prototype.parseXML = function (xmlString) {
                if (RadJav.OS.HTML5 != null) {
                    this._parser = new DOMParser();
                    this._loadedFile = new XMLParser();
                    this._loadedFile.xmlFile = this._parser.parseFromString(xmlString, "text/xml");
                    this._root = new XMLTag(this._loadedFile);
                }
            };
            XMLFile.prototype.getRoot = function () {
                return (this._root);
            };
            XMLFile.loadFile = function (filePath) {
                var promise = new Promise(RadJav.keepContext(function (resolve, reject, file) {
                    var path = file[0];
                    var xmlFile = new XMLFile();
                    var promise2 = xmlFile.parseXMLFile(path).then(function () {
                        resolve(this);
                    });
                    return (promise2);
                }, this, [filePath]));
                return (promise);
            };
            XMLFile.loadString = function (xmlString) {
                var xmlFile = new XMLFile();
                xmlFile.parseXML(xmlString);
                return (xmlFile);
            };
            return XMLFile;
        }());
        XML.XMLFile = XMLFile;
        var XMLParser = (function () {
            function XMLParser(xmlFile) {
                if (xmlFile === void 0) { xmlFile = null; }
                this.xmlFile = xmlFile;
            }
            return XMLParser;
        }());
        XML.XMLParser = XMLParser;
        var XMLTag = (function () {
            function XMLTag(tag) {
                if (RadJav.OS.HTML5 != null) {
                    if (typeof (tag) == "string") {
                        this._tag = tag;
                        this._attributes = {};
                        this._value = "";
                        this._children = [];
                        this._loadedFile = null;
                    }
                    else {
                        var domTag = tag.xmlFile.firstChild;
                        this._loadedFile = tag;
                        this._tag = domTag.tagName;
                        this._attributes = {};
                        for (var iIdx = 0; iIdx < domTag.attributes.length; iIdx++) {
                            var attribute = domTag.attributes[iIdx];
                            var attr = new XMLAttribute(attribute.name, attribute.value);
                            this._attributes[attribute.name] = attr;
                        }
                        this._value = domTag.textContent;
                        this._children = [];
                        for (var iIdx = 0; iIdx < domTag.childNodes.length; iIdx++) {
                            var domChild = domTag.childNodes[iIdx];
                            var child = new XMLTag(new XMLParser(domChild));
                            this._children.push(child);
                        }
                    }
                }
                else {
                    if (typeof this["_init"] == "function")
                        this["_init"](tag);
                }
            }
            XMLTag.prototype.getChildren = function () {
                return (this._children);
            };
            XMLTag.prototype.getAttributes = function () {
                return (this._attributes);
            };
            XMLTag.prototype.setTag = function (name) {
                this._tag = name;
            };
            XMLTag.prototype.getTag = function () {
                return (this._tag);
            };
            XMLTag.prototype.setValue = function (value) {
                this._value = value;
            };
            XMLTag.prototype.getValue = function () {
                return (this._value);
            };
            XMLTag.prototype.getTags = function (tag) {
                var tags = [];
                for (var iIdx = 0; iIdx < this._children.length; iIdx++) {
                    var child = this._children[iIdx];
                    if (child._tag == tag)
                        tags.push(child);
                }
                return (tags);
            };
            XMLTag.prototype.setAttribute = function (attribute, value) {
                if (this._attributes[attribute] == undefined)
                    this._attributes[attribute] = new XMLAttribute(attribute, value);
                else
                    this._attributes[attribute].setValue(value);
            };
            XMLTag.prototype.hasAttribute = function (attribute) {
                if (this._attributes[attribute] == undefined)
                    return (false);
                return (true);
            };
            XMLTag.prototype.getAttribute = function (attribute) {
                if (this._attributes[attribute] == undefined)
                    throw new Error("Attribute does not exist!");
                return (this._attributes[attribute]);
            };
            XMLTag.prototype.getAttributeString = function (attribute, defaultValue) {
                if (defaultValue === void 0) { defaultValue = ""; }
                var value = defaultValue;
                try {
                    var attrValue = this.getAttribute(attribute);
                    value = attrValue.getValue();
                }
                catch (ex) {
                }
                return (value);
            };
            XMLTag.prototype.getAttributeInt = function (attribute, defaultValue) {
                if (defaultValue === void 0) { defaultValue = 0; }
                var value = defaultValue;
                try {
                    var attrValue = this.getAttribute(attribute);
                    value = attrValue.toInt();
                }
                catch (ex) {
                }
                return (value);
            };
            XMLTag.prototype.getAttributeFloat = function (attribute, defaultValue) {
                if (defaultValue === void 0) { defaultValue = 0.0; }
                var value = defaultValue;
                try {
                    var attrValue = this.getAttribute(attribute);
                    value = attrValue.toFloat();
                }
                catch (ex) {
                }
                return (value);
            };
            XMLTag.prototype.getAttributeBoolean = function (attribute, defaultValue) {
                if (defaultValue === void 0) { defaultValue = false; }
                var value = defaultValue;
                try {
                    var attrValue = this.getAttribute(attribute);
                    value = attrValue.toBoolean();
                }
                catch (ex) {
                }
                return (value);
            };
            XMLTag.prototype.toString = function () {
                var result = "<" + this._tag + " ";
                for (var attr in this._attributes) {
                    var attribute = this._attributes[attr];
                    result += attribute.toString() + " ";
                }
                if ((this._value != "") || (this._children.length > 0)) {
                    result += ">" + this._value;
                    if (this._value != "")
                        result += " ";
                    for (var iIdx = 0; iIdx < this._children.length; iIdx++) {
                        var child = this._children[iIdx];
                        result += child.toString() + " ";
                    }
                    result += "</" + this._tag + ">";
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
                this._name = name;
                this._value = value;
            }
            XMLAttribute.prototype.setValue = function (value) {
                this._value = value;
            };
            XMLAttribute.prototype.getValue = function () {
                return (this._value);
            };
            XMLAttribute.prototype.toInt = function () {
                return (parseInt(this._value));
            };
            XMLAttribute.prototype.toFloat = function () {
                return (parseFloat(this._value));
            };
            XMLAttribute.prototype.toBoolean = function () {
                return (parseBoolean(this._value));
            };
            XMLAttribute.prototype.toString = function () {
                return (this._name + " = \"" + this._value + "\"");
            };
            return XMLAttribute;
        }());
        XML.XMLAttribute = XMLAttribute;
    })(XML = RadJav.XML || (RadJav.XML = {}));
})(RadJav || (RadJav = {}));
//# sourceMappingURL=RadJav.IO.js.map