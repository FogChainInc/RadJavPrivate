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
/// <reference path="RadJav.MUI.View.ts" />
var RadJav;
(function (RadJav) {
    var MUI;
    (function (MUI) {
        /** @class RadJav.MUI.Navigator
         * A mobile view navigator.
         * Available on platforms: iOS,Android,HTML5
         */
        var Navigator = /** @class */ (function () {
            function Navigator(view) {
                this.type = "RadJav.MUI.Navigator";
                this.rootWin = view;
                if (this._init != null) {
                    this._init.apply(this, arguments);
                }
            }
            Navigator.prototype.push = function (view, replace) {
                if (this._push != null) {
                    this._push.apply(this, arguments);
                }
            };
            Navigator.prototype.pop = function (view) {
                if (this._pop != null) {
                    this._pop.apply(this, arguments);
                }
            };
            Navigator.xmlTag = { tag: "navigator", type: "Navigator" };
            return Navigator;
        }());
        MUI.Navigator = Navigator;
    })(MUI = RadJav.MUI || (RadJav.MUI = {}));
})(RadJav || (RadJav = {}));
