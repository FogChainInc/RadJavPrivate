var RadJav;
(function (RadJav) {
    var Interact;
    (function (Interact) {
        function setView(root, view) {
            if (root.constructor["name"] == "GObject") {
                return;
            }
            if (root.constructor["name"] == "Object3D") {
                return;
            }
            for (var comp in view._components)
                root.appendChild(view._components[comp].display._html);
        }
        Interact.setView = setView;
        function setComponentEvent(name, eventName, eventFunction) {
            var elm = document.getElementById(name);
            for (var iIdx = 0; iIdx < elm.childNodes.length; iIdx++) {
                var child = elm.childNodes[iIdx];
                child.addEventListener(eventName, eventFunction);
            }
        }
        Interact.setComponentEvent = setComponentEvent;
        var View = (function () {
            function View(name) {
                if (name == null)
                    throw new Error("View must have a name!");
                if (name == "")
                    throw new Error("View must have a name!");
                this.name = name;
                this._components = {};
                this._refCount = 0;
                this._events = {};
            }
            View.prototype.createComponent = function (obj) {
                if (obj.name == null)
                    throw new Error("Component must have a name!");
                if (obj.name == "")
                    throw new Error("Component must have a name!");
                if (this._components[obj.name] != null)
                    throw new Error("Component cannot have the same name as an existing component!");
                if (typeof obj.display == "string") {
                    if (RadJav.OS.HTML5 != null)
                        obj.display = new RadJav.GUI.HTMLElement(obj.name, obj.display);
                }
                if (obj["on"] == null)
                    obj = new RadJav.Interact.Component(obj, this);
                this._components[obj.name] = obj;
                return (obj);
            };
            View.prototype.on = function (eventName, eventFunction) {
                this._events[eventName] = eventFunction;
                if (RadJav.OS.HTML5 != null) {
                    if (eventName == "load")
                        window.onload = eventFunction;
                    else
                        document.addEventListener(eventName, eventFunction);
                }
                return (null);
            };
            View.prototype.buildHTML = function () {
                var output = new HTMLOutput();
                output.name = this.name;
                output.title = this.name;
                for (var evt in this._events) {
                    var viewEvent = "";
                    if (evt == "load")
                        viewEvent = "(" + this._events[evt].toString() + ")()";
                    else
                        viewEvent = "document.addEventListener (\"" + evt + "\", " + this._events[evt].toString() + ");";
                    output.events.push(viewEvent);
                }
                for (var comp in this._components) {
                    var component = this._components[comp];
                    var icomp = null;
                    if (RadJav.OS.HTML5 != null)
                        icomp = component.display._text;
                    else
                        icomp = "<div id = \"" + component.name + "\">" + component.display + "</div>";
                    for (var evt in component._events) {
                        var compEvent = "RadJav.Interact.setComponentEvent (\"" + component.name + "\", \"" + evt + "\", " + component._events[evt].toString() + ");";
                        output.events.push(compEvent);
                    }
                    output.displays.push(icomp);
                }
                return (output);
            };
            return View;
        }());
        Interact.View = View;
        var Component = (function () {
            function Component(obj, parentView) {
                if (obj.name == null)
                    throw new Error("Component must have a name!");
                if (obj.name == "")
                    throw new Error("Component must have a name!");
                this._data = {};
                this._events = {};
                if (obj["display"] != null) {
                    if (typeof obj.display == "string") {
                        this.parse();
                        if (RadJav.OS.HTML5 != null)
                            obj.display = new RadJav.GUI.HTMLElement(obj.name, obj.display);
                        parentView._refCount++;
                    }
                }
                this.name = obj.name;
                this.tag = RadJav.setDefaultValue(obj.tag, "");
                this.display = RadJav.setDefaultValue(obj.display, null);
                var ignoreList = ["name", "tag", "display"];
                var _loop_1 = function (key) {
                    var skip = false;
                    for (var iIdx = 0; iIdx < ignoreList.length; iIdx++) {
                        if (key == ignoreList[iIdx]) {
                            skip = true;
                            break;
                        }
                    }
                    if (skip == true)
                        return "break";
                    this_1._data[key] = Object.defineProperty(obj, key, {
                        get: function () {
                            return (this[key]);
                        },
                        set: RadJav.keepContext(function (value, obj2) {
                            var obj3 = obj2[0];
                            var key2 = obj2[1];
                            obj3[key2] = value;
                        }, this_1, [obj, key])
                    });
                };
                var this_1 = this;
                for (var key in obj) {
                    var state_1 = _loop_1(key);
                    if (state_1 === "break")
                        break;
                }
            }
            Component.prototype.parse = function () {
            };
            Component.prototype.on = function (eventName, eventFunction) {
                this._events[eventName] = eventFunction;
                if (RadJav.OS.HTML5 != null)
                    return (this.display.on(eventName, eventFunction));
                return (null);
            };
            return Component;
        }());
        Interact.Component = Component;
        var HTMLOutput = (function () {
            function HTMLOutput() {
                this.name = "";
                this.title = "";
                this.displays = [];
                this.events = [];
                this.cssFiles = [];
                this.jsFiles = [];
                this._assetsPath = "";
                this._fileCopyFunc = null;
                this.header = "<!DOCTYPE html>\n";
                this.header += "<html>\n";
                this.header += "<head>\n";
                this.header += "\t<title>" + this.title + "</title>\n";
                this.header += "\t<link href = \"./RadJav/themes/default/dojo/themes/claro/claro.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.header += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/Grid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.header += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/claroGrid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.header += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/Calendar.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.header += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/MonthColumnView.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.header += "\t<link href = \"./RadJav/themes/default/dojo/resources/FloatingPane.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.header += "\t<link href = \"./RadJav/themes/default/dojo/resources/ResizeHandle.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.header += "{{cssFiles}}";
                this.header += "\t<script type = \"text/javascript\" src = \"./dependencies/three.js/three.min.js\"></script>\n";
                this.header += "\t<script type = \"text/javascript\" src = \"./dependencies/dojo/dojo/dojo.js\"></script>\n";
                this.header += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.js\"></script>\n";
                this.header += "\n";
                this.header += "\t<script type = \"text/javascript\">\n";
                this.header += "\t\tRadJav.baseUrl = \"./RadJav\";\n";
                this.header += "\t\tRadJav.themeUrl = \"./RadJav/themes/default\";\n";
                this.header += "\t\tRadJav.useAjax = true;\n";
                this.header += "\n";
                this.header += "\t\tfunction setup ()\n";
                this.header += "\t\t{\n";
                this.header += "{{events}}";
                this.header += "\t\t}\n";
                this.header += "\t</script>\n";
                this.header += "\n";
                this.header += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.Interact.js\"></script>\n";
                this.header += "{{jsFiles}}";
                this.header += "</head>\n";
                this.header += "\n";
                this.header += "<body onload = \"setup ();\">\n";
                this.footer = "</body>\n";
                this.footer += "</html>\n\n";
            }
            HTMLOutput.prototype.assetsDir = function (path, fileCopyFunc) {
                this._assetsPath = path;
                this._fileCopyFunc = fileCopyFunc;
            };
            HTMLOutput.prototype.writeFiles = function (path) {
                var files = RadJav.IO.listFiles(this._assetsPath);
                for (var iIdx = 0; iIdx < files.length; iIdx++) {
                    var file = files[iIdx];
                    var oldPath = this._assetsPath + "/" + file;
                    var newPath = path + "/" + file;
                    if (this._fileCopyFunc(oldPath, newPath) == true) {
                        if (RadJav.IO.isDir(newPath) == true)
                            RadJav.IO.copyDir(oldPath, newPath);
                        else
                            RadJav.IO.copyFile(oldPath, newPath);
                    }
                }
                this.writeToFile(path + "/" + this.name + ".htm");
            };
            HTMLOutput.prototype.writeToFile = function (path, includeDependencies) {
                if (includeDependencies === void 0) { includeDependencies = true; }
                var output = "";
                var html = "";
                output += this.header;
                for (var iIdx = 0; iIdx < this.displays.length; iIdx++)
                    html += "\t" + this.displays[iIdx] + "\n";
                var additions = ["events", "cssFiles", "jsFiles"];
                for (var iIdx = 0; iIdx < additions.length; iIdx++) {
                    var additionType = additions[iIdx];
                    var additionOutput = "";
                    for (var iJdx = 0; iJdx < this[additionType].length; iJdx++) {
                        if (additionType == "events")
                            additionOutput += "\t\t\t" + this.events[iJdx] + "\n";
                        if (additionType == "cssFiles")
                            additionOutput += "\t<link href = \"" + this.cssFiles[iJdx] + "\" rel = \"stylesheet\" type = \"text/css\" />\n";
                        if (additionType == "jsFiles")
                            additionOutput += "\t<script type = \"text/javascript\" src = \"" + this.jsFiles[iJdx] + "\"></script>\n";
                    }
                    output = output.replaceAll("{{" + additionType + "}}", additionOutput);
                }
                output += html;
                output += this.footer;
                RadJav.IO.TextFile.writeTextToFile(path, output);
            };
            return HTMLOutput;
        }());
        Interact.HTMLOutput = HTMLOutput;
    })(Interact = RadJav.Interact || (RadJav.Interact = {}));
})(RadJav || (RadJav = {}));
