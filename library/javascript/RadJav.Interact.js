var RadJav;
(function (RadJav) {
    var Interact;
    (function (Interact) {
        function parseExpressions(expression) {
            var expressions = [];
            var expBegin = expression.indexOf("{{");
            var expEnd = expression.indexOf("}}");
            while (expBegin > -1) {
                if (expEnd < 0)
                    throw new Error("Expression does not end properly!");
                var foundExp = expression.substr((expBegin + 2), (expEnd - (expBegin + 2)));
                expressions.push(foundExp);
                expBegin = expression.indexOf("{{", expBegin + 2);
                expEnd = expression.indexOf("}}", expEnd + 2);
            }
            return (expressions);
        }
        Interact.parseExpressions = parseExpressions;
        var App = (function () {
            function App(name) {
                this.name = name;
                this._views = {};
                this.html = null;
                this.xrjApp = null;
                this.root = null;
            }
            App.prototype.addView = function (view) {
                if (view.name == "")
                    throw new Error("View must have a name!");
                if (view.name == null)
                    throw new Error("View must have a name!");
                view._app = this;
                this._views[view.name] = view;
            };
            App.prototype.setView = function (root, view) {
                if (typeof (root) == "object") {
                    if (root.constructor["name"] == "View")
                        view = root;
                }
                this.addView(view);
                this.currentView = view;
                if (root.constructor["name"] == "GObject") {
                    return;
                }
                if (root.constructor["name"] == "Object3D") {
                    return;
                }
                if (root.constructor["name"] != "View")
                    this.root = root;
                debugger;
                if (RadJav.OS.HTML5 != null) {
                    if (typeof (root) == "string")
                        root = document.querySelector(root);
                    for (var comp in view._components) {
                        view._components[comp].display.create();
                    }
                }
            };
            App.prototype.setComponentEvent = function (name, eventName, eventFunction) {
                var elm = document.getElementById(name);
                for (var iIdx = 0; iIdx < elm.childNodes.length; iIdx++) {
                    var child = elm.childNodes[iIdx];
                    child.addEventListener(eventName, eventFunction);
                }
            };
            App.prototype.build = function (newApp) {
                if (newApp === void 0) { newApp = null; }
                var app = newApp;
                if (app == null)
                    app = new CompiledApp(this.name);
                app.mainView = this.currentView.name;
                if (this.root != null)
                    app.root = this.root.toString();
                for (var key in this._views) {
                    var view = this._views[key].build(newApp);
                    app.views.push(view);
                }
                return (app);
            };
            App.loadApp = function (file) {
                if (typeof (file) == "string")
                    RadJav.include(file);
                else
                    file();
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
                this._mainComponent = null;
                this._app = null;
            }
            View.prototype.createComponent = function (obj) {
                if (obj.name == null)
                    throw new Error("Component must have a name!");
                if (obj.name == "")
                    throw new Error("Component must have a name!");
                if (this._components[obj.name] != null)
                    throw new Error("Component cannot have the same name as an existing component!");
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
            View.prototype.build = function (newApp) {
                var view = new CompiledView(this.name);
                for (var compName in this._components) {
                    var component = this._components[compName];
                    var componentStr = "";
                    var css = "";
                    var display = "";
                    if (typeof (component.display) == "string")
                        display = JSON.stringify(component.display);
                    componentStr += "var " + compName + " = " + view.name + ".createComponent ({\n";
                    componentStr += "\t\tname: \"" + component.name + "\", \n";
                    componentStr += "\t\ttag: \"" + component.tag + "\", \n";
                    componentStr += "\t\tdisplay: " + display + "\n";
                    componentStr += "\t});\n";
                    for (var eventName in component._events)
                        componentStr += compName + ".on (\"" + eventName + "\", " + component._events[eventName].toString() + ");\n";
                    view.components[compName] = componentStr;
                    if (component.css != null) {
                        for (var iIdx = 0; iIdx < component.css.length; iIdx++) {
                            var compCSS = component.css[iIdx];
                            if (typeof (compCSS) == "string")
                                css += RadJav.IO.TextFile.readEntireFile(compCSS) + "\n";
                            if (typeof (compCSS) == "object")
                                css += compCSS.css + "\n";
                        }
                    }
                    if (css != "") {
                        view.compiledHTMLCSS += "/* --- View " + this.name + ", Component " + compName + " CSS Begin --- */\n";
                        view.compiledHTMLCSS += css;
                        view.compiledHTMLCSS += "/* --- View " + this.name + ", Component " + compName + " CSS End --- */\n";
                    }
                }
                if (this._mainComponent != null)
                    view.mainComponentName = this._mainComponent.name;
                return (view);
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
                if (obj["display"] != null) {
                    if (typeof obj.display == "string") {
                        if (RadJav.OS.HTML5 != null)
                            obj.display = new RadJav.GUI.HTMLElement(obj.name, obj.display);
                        parentView._refCount++;
                    }
                }
                this.name = obj.name;
                this.tag = RadJav.setDefaultValue(obj.tag, "");
                this.display = RadJav.setDefaultValue(obj.display, null);
                this.css = RadJav.setDefaultValue(obj.css, []);
                this._data = RadJav.setDefaultValue(obj._data, {});
                this._events = RadJav.setDefaultValue(obj._events, {});
                if (RadJav.OS.HTML5 != null) {
                    if (this.tag != "") {
                        var elm = document.createElement(this.tag);
                        elm.innerHTML = this.display._html;
                    }
                }
                var ignoreList = ["name", "tag", "display", "css"];
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
                            return (this[key].value);
                        },
                        set: RadJav.keepContext(function (value, obj2) {
                            var obj3 = obj2[0];
                            var key2 = obj2[1];
                            var parentView2 = obj2[2];
                            obj3[key2] = new ComponentData();
                            obj3[key2].name = parentView2._app.name + "-" + parentView2.name + "-" + parentView2._refCount;
                            obj3[key2].value = value;
                        }, this_1, [obj, key, parentView])
                    });
                };
                var this_1 = this;
                for (var key in obj) {
                    var state_1 = _loop_1(key);
                    if (state_1 === "break")
                        break;
                }
                this.parse();
            }
            Component.prototype.parse = function () {
                if (this.display == null)
                    return;
                var beginningBraces = "";
                var endBraces = "";
                var str = "";
                var compName = "";
                if (typeof this.display == "string") {
                    str = this.display;
                    beginningBraces = "<div id = \"" + compName + "\">";
                    endBraces = "</div>";
                }
                else {
                    if (this.display.constructor["name"] == "GObject")
                        str = this.display.getText();
                }
                var expressions = RadJav.Interact.parseExpressions(str);
                for (var iIdx = 0; iIdx < expressions.length; iIdx++) {
                }
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
        var ComponentData = (function () {
            function ComponentData() {
                this.name = "";
                this.value = null;
            }
            return ComponentData;
        }());
        Interact.ComponentData = ComponentData;
        var CompiledHTML = (function () {
            function CompiledHTML(name) {
                this.filename = name + ".htm";
                this.title = name;
                this.interactAppPath = "./" + name + ".js";
                this.htmlEvents = [];
                this.cssFiles = [];
                this.jsFiles = ["./" + name + ".xrj"];
                this.body = "";
                this.html = "<!DOCTYPE html>\n";
                this.html += "<html>\n";
                this.html += "<head>\n";
                this.html += "\t<title>{{title}}</title>\n";
                this.html += "\t<link href = \"./RadJav/themes/default/dojo/themes/claro/claro.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.html += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/Grid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.html += "\t<link href = \"./RadJav/themes/default/dojox/grid/resources/claroGrid.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.html += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/Calendar.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.html += "\t<link href = \"./RadJav/themes/default/dojox/calendar/themes/claro/MonthColumnView.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.html += "\t<link href = \"./RadJav/themes/default/dojo/resources/FloatingPane.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.html += "\t<link href = \"./RadJav/themes/default/dojo/resources/ResizeHandle.css\" rel = \"stylesheet\" type = \"text/css\" />\n";
                this.html += "{{cssFiles}}";
                this.html += "\t<script type = \"text/javascript\" src = \"./dependencies/three.js/three.min.js\"></script>\n";
                this.html += "\t<script type = \"text/javascript\" src = \"./dependencies/dojo/dojo/dojo.js\"></script>\n";
                this.html += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.js\"></script>\n";
                this.html += "\n";
                this.html += "\t<script type = \"text/javascript\">\n";
                this.html += "\t\tRadJav.baseUrl = \"./RadJav\";\n";
                this.html += "\t\tRadJav.themeUrl = \"./RadJav/themes/default\";\n";
                this.html += "\t\tRadJav.useAjax = true;\n";
                this.html += "\n";
                this.html += "\t\tfunction setup ()\n";
                this.html += "\t\t{\n";
                this.html += "{{htmlEvents}}";
                this.html += "\t\t}\n";
                this.html += "\t</script>\n";
                this.html += "\n";
                this.html += "\t<script type = \"text/javascript\" src = \"./RadJav/RadJav.Interact.js\"></script>\n";
                this.html += "{{jsFiles}}";
                this.html += "</head>\n";
                this.html += "\n";
                this.html += "<body onload = \"setup ();\">\n";
                this.html += "{{body}}\n";
                this.html += "</body>\n";
                this.html += "</html>\n\n";
            }
            CompiledHTML.prototype.build = function () {
                var output = this.html;
                var additions = ["title", "body", "interactAppPath", "htmlEvents", "cssFiles", "jsFiles"];
                for (var iIdx = 0; iIdx < additions.length; iIdx++) {
                    var additionType = additions[iIdx];
                    var additionOutput = "";
                    if (typeof (this[additionType]) != "string") {
                        for (var iJdx = 0; iJdx < this[additionType].length; iJdx++) {
                            if (additionType == "htmlEvents")
                                additionOutput += "\t\t\t" + this.htmlEvents[iJdx] + "\n";
                            if (additionType == "cssFiles")
                                additionOutput += "\t<link href = \"" + this.cssFiles[iJdx] + "\" rel = \"stylesheet\" type = \"text/css\" />\n";
                            if (additionType == "jsFiles")
                                additionOutput += "\t<script type = \"text/javascript\" src = \"" + this.jsFiles[iJdx] + "\"></script>\n";
                        }
                    }
                    else
                        additionOutput = this[additionType];
                    output = output.replaceAll("{{" + additionType + "}}", additionOutput);
                }
                return (output);
            };
            CompiledHTML.prototype.writeToFile = function (path) {
                var output = this.build();
                RadJav.IO.TextFile.writeTextToFile(path, output);
            };
            return CompiledHTML;
        }());
        Interact.CompiledHTML = CompiledHTML;
        var CompiledXRJApp = (function () {
            function CompiledXRJApp(name) {
                this.filename = name + ".xrj";
                this.name = name;
                this.version = "1.0";
                this.interactAppPath = "./" + name + ".js";
                this.guiEvents = [];
                this.generateAsHTML5App = true;
                this.xrjApp = "var appName = \"{{name}}\";\n";
                this.xrjApp += "var version = \"{{version}}\";\n";
                this.xrjApp += "\n";
                this.xrjApp += "function onReady ()\n";
                this.xrjApp += "{\n";
                this.xrjApp += "\tRadJav.initialize (RadJav.getStandardLibrary (), RadJav.getGUILibrary ()).then (function ()\n";
                this.xrjApp += "\t\t{\n";
                this.xrjApp += "\t\t\tRadJav.runApplication (function ()\n";
                this.xrjApp += "\t\t\t\t{\n";
                if (this.generateAsHTML5App == true) {
                }
                this.xrjApp += "\t\t\t\t\tRadJav.Interact.App.loadApp (\"{{interactAppPath}}\");\n";
                this.xrjApp += "\t\t\t\t});\n";
                this.xrjApp += "\t\t});\n";
                this.xrjApp += "}\n";
                this.xrjApp += "\n";
                this.xrjApp += "RadJav.OS.onReady (onReady);\n";
                this.xrjApp += "\n\n";
            }
            CompiledXRJApp.prototype.build = function () {
                var output = this.xrjApp;
                var additions = ["name", "version", "interactAppPath", "guiEvents"];
                for (var iIdx = 0; iIdx < additions.length; iIdx++) {
                    var additionType = additions[iIdx];
                    var additionOutput = "";
                    if (typeof (this[additionType]) != "string") {
                        for (var iJdx = 0; iJdx < this[additionType].length; iJdx++) {
                            if (additionType == "guiEvents")
                                additionOutput += "\t\t\t" + this.guiEvents[iJdx] + "\n";
                        }
                    }
                    else
                        additionOutput = this[additionType];
                    output = output.replaceAll("{{" + additionType + "}}", additionOutput);
                }
                return (output);
            };
            CompiledXRJApp.prototype.writeToFile = function (path) {
                var output = this.build();
                RadJav.IO.TextFile.writeTextToFile(path, output);
            };
            return CompiledXRJApp;
        }());
        Interact.CompiledXRJApp = CompiledXRJApp;
        var CompiledApp = (function () {
            function CompiledApp(name) {
                this.name = name;
                this.html = new CompiledHTML(name);
                this.xrjApp = new CompiledXRJApp(name);
                this.views = [];
                this.compiledStr = "var app = new RadJav.Interact.App (\"" + this.name + "\");";
                this.mainView = "";
                this.root = "";
                this._assetsPath = "";
                this._fileCopyFunc = null;
            }
            CompiledApp.prototype.assetsDir = function (path, fileCopyFunc) {
                this._assetsPath = path;
                this._fileCopyFunc = fileCopyFunc;
            };
            CompiledApp.prototype.writeFiles = function (path) {
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
                var output = this.compiledStr + "\n";
                for (var iIdx = 0; iIdx < this.views.length; iIdx++) {
                    var view = this.views[iIdx];
                    view.writeHTMLCSSToFile(path + "/" + view.name + ".css");
                    output += view.build();
                }
                output += "app.setView (\"" + this.root + "\", " + this.mainView + ");\n";
                RadJav.IO.TextFile.writeTextToFile(path + "/" + this.name + ".js", output);
                if (this.html != null)
                    this.html.writeToFile(path + "/" + this.html.filename);
                if (this.xrjApp != null)
                    this.xrjApp.writeToFile(path + "/" + this.xrjApp.filename);
            };
            return CompiledApp;
        }());
        Interact.CompiledApp = CompiledApp;
        var CompiledView = (function () {
            function CompiledView(name) {
                this.name = name;
                this.compiledHTMLCSS = "";
                this.components = {};
                this.mainComponentName = "";
                this.compiledStr = "var " + this.name + " = new RadJav.Interact.View (\"" + this.name + "\");\n";
            }
            CompiledView.prototype.build = function () {
                var output = this.compiledStr + "\n";
                for (var key in this.components) {
                    var comp = this.components[key];
                    output += comp;
                }
                if (this.mainComponentName != "")
                    output += this.name + ".setMainComponent (" + this.mainComponentName + ");\n";
                return (output);
            };
            CompiledView.prototype.writeHTMLCSSToFile = function (path) {
                if (this.compiledHTMLCSS != "")
                    RadJav.IO.TextFile.writeTextToFile(path, this.compiledHTMLCSS);
            };
            CompiledView.prototype.writeToFile = function (path) {
                var output = this.build();
                RadJav.IO.TextFile.writeTextToFile(path, output);
            };
            return CompiledView;
        }());
        Interact.CompiledView = CompiledView;
    })(Interact = RadJav.Interact || (RadJav.Interact = {}));
})(RadJav || (RadJav = {}));
