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
/// <reference path="RadJav.Vector2.ts" />
var RadJav;
(function (RadJav) {
    var MUI;
    (function (MUI) {
        /** @class RadJav.MUI.TableViewController
         * @extends RadJav.GUI.GObject
         * A mobile view.
         * Available on platforms: iOS,Android,HTML5
         */
        var TableViewController = /** @class */ (function (_super) {
            __extends(TableViewController, _super);
            function TableViewController(obj, text, parent) {
                var _this = this;
                if (obj == null)
                    obj = {};
                if (typeof obj == "string") {
                    var name = obj;
                    obj = { name: name };
                }
                if (obj.size == null) {
                    obj.size = new RadJav.Vector2();
                    obj.size.x = 500;
                    obj.size.y = 350;
                }
                (_this = _super.call(this, obj, text, parent) || this) || _this;
                _this.type = "RadJav.MUI.TableViewController";
                return _this;
            }
            TableViewController.xmlTag = { tag: "tableviewController", type: "TableViewController" };
            return TableViewController;
        }(RadJav.MUI.ViewController));
        MUI.TableViewController = TableViewController;
    })(MUI = RadJav.MUI || (RadJav.MUI = {}));
})(RadJav || (RadJav = {}));
