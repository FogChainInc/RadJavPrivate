var RadJav;
(function (RadJav) {
    var Interact;
    (function (Interact) {
        var App = (function () {
            function App(name) {
                this.name = name;
                this._views = {};
                RadJav.Interact.App.defaultHeader = "<!DOCTYPE html>\n";
                RadJav.Interact.App.defaultHeader += "<html>\n";
                RadJav.Interact.App.defaultHeader += "<head>\n";
                RadJav.Interact.App.defaultHeader += "\t<title>{{title}}</title>\n";
                RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojo/themes/claro/claro.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/Grid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/claroGrid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/Calendar.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/MonthColumnView.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojo/resources/FloatingPane.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                RadJav.Interact.App.defaultHeader += "\t<link href = \"./RadJav/themes/default/dojo/resources/ResizeHandle.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                RadJav.Interact.App.defaultHeader += "{{cssFiles}}";
                RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\" src = \"./dependencies/three.js/three.min.js\"></script>\n";
                RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\" src = \"./dependencies/dojo/dojo/dojo.js\"></script>\n";
                RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.js\"></script>\n";
                RadJav.Interact.App.defaultHeader += "\n";
                RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\">\n";
                RadJav.Interact.App.defaultHeader += "\t\tRadJav.baseUrl = \"./RadJav\";\n";
                RadJav.Interact.App.defaultHeader += "\t\tRadJav.themeUrl = \"./RadJav/themes/default\";\n";
                RadJav.Interact.App.defaultHeader += "\t\tRadJav.useAjax = true;\n";
                RadJav.Interact.App.defaultHeader += "\n";
                RadJav.Interact.App.defaultHeader += "\t\tfunction setup ()\n";
                RadJav.Interact.App.defaultHeader += "\t\t{\n";
                RadJav.Interact.App.defaultHeader += "{{events}}";
                RadJav.Interact.App.defaultHeader += "\t\t}\n";
                RadJav.Interact.App.defaultHeader += "\t</script>\n";
                RadJav.Interact.App.defaultHeader += "\n";
                RadJav.Interact.App.defaultHeader += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.Interact.js\"></script>\n";
                RadJav.Interact.App.defaultHeader += "{{jsFiles}}";
                RadJav.Interact.App.defaultHeader += "</head>\n";
                RadJav.Interact.App.defaultHeader += "\n";
                RadJav.Interact.App.defaultHeader += "<body onload = \"setup ();\">\n";
                RadJav.Interact.App.defaultFooter = "</body>\n";
                RadJav.Interact.App.defaultFooter += "</html>\n\n";
            }
            App.prototype.addView = function (view) {
                if (view.name == "")
                    throw new Error("View must have a name!");
                if (view.name == null)
                    throw new Error("View must have a name!");
                this._views[view.name] = view;
            };
            App.prototype.setView = function (root, view) {
                this.addView(view);
                if (root.constructor["name"] == "GObject") {
                    return;
                }
                if (root.constructor["name"] == "Object3D") {
                    return;
                }
                if (typeof (root) == "string")
                    root = document.querySelector(root);
                for (var comp in view._components)
                    root.appendChild(view._components[comp].display._html);
            };
            App.prototype.setComponentEvent = function (name, eventName, eventFunction) {
                var elm = document.getElementById(name);
                for (var iIdx = 0; iIdx < elm.childNodes.length; iIdx++) {
                    var child = elm.childNodes[iIdx];
                    child.addEventListener(eventName, eventFunction);
                }
            };
            App.prototype.buildHTMLApp = function () {
                var app = new HTMLApp();
                for (var view in this._views) {
                    var html = this._views[view].buildHTML();
                    html.header = app.header;
                    html.footer = app.footer;
                    html.displays = app.displays;
                    html.events = app.events;
                    html.cssFiles = app.cssFiles;
                    html.jsFiles = app.jsFiles;
                    html._fileCopyFunc = app._fileCopyFunc;
                    app._outputs.push(html);
                }
                return (app);
            };
            return App;
        }());
        Interact.App = App;
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
            View.prototype.setMainComponent = function (component) {
                if (typeof component == "string") {
                    component = this._components[component];
                    if (component == null)
                        throw new Error("Component could not be found!");
                }
                this._mainComponent = component;
            };
            View.prototype.getMainComponent = function () {
                return (this._mainComponent);
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
        var HTMLApp = (function () {
            function HTMLApp() {
                this.name = "";
                this.title = "";
                this.displays = [];
                this.events = [];
                this.cssFiles = [];
                this.jsFiles = [];
                this._assetsPath = "";
                this._fileCopyFunc = null;
                this._outputs = [];
            }
            HTMLApp.prototype.assetsDir = function (path, fileCopyFunc) {
                this._assetsPath = path;
                this._fileCopyFunc = fileCopyFunc;
            };
            HTMLApp.prototype.writeFiles = function (path) {
                var files = RadJav.IO.listFiles(this._assetsPath);
                for (var iIdx = 0; iIdx < files.length; iIdx++) {
                    var file = files[iIdx];
                    var oldPath = RadJav.IO.normalizePath(this._assetsPath + "/" + file);
                    var newPath = RadJav.IO.normalizePath(path + "/" + file);
                    var result = true;
                    if (this._fileCopyFunc != null) {
                        result = this._fileCopyFunc(oldPath, newPath);
                        if (result == null)
                            result = true;
                    }
                    if (result == true) {
                        if (RadJav.IO.isDir(oldPath) == true)
                            RadJav.IO.copyDir(oldPath, newPath);
                        else
                            RadJav.IO.copyFile(oldPath, newPath);
                    }
                }
                for (var iIdx = 0; iIdx < this._outputs.length; iIdx++) {
                    var output = this._outputs[iIdx];
                    output.writeToFile(path + "/" + output.name + ".htm");
                }
            };
            return HTMLApp;
        }());
        Interact.HTMLApp = HTMLApp;
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
                this.header = RadJav.Interact.App.defaultHeader;
                this.footer = RadJav.Interact.App.defaultFooter;
            }
            HTMLOutput.prototype.assetsDir = function (path, fileCopyFunc) {
                this._assetsPath = path;
                this._fileCopyFunc = fileCopyFunc;
            };
            HTMLOutput.prototype.writeToFile = function (path, includeDependencies) {
                if (includeDependencies === void 0) { includeDependencies = true; }
                var output = "";
                var html = "";
                output += this.header;
                RadJav.IO.TextFile.writeTextToFile(path, output);
            };
            return HTMLOutput;
        }());
        Interact.HTMLOutput = HTMLOutput;
    })(Interact = RadJav.Interact || (RadJav.Interact = {}));
})(RadJav || (RadJav = {}));
