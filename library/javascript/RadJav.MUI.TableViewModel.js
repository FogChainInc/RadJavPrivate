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
        /** @class RadJav.MUI.TableViewModel
         * A model for TableView control.
         * Available on platforms: iOS,Android,HTML5
         */
        var TableViewModel = /** @class */ (function () {
            function TableViewModel() {
                this.type = "RadJav.MUI.TableViewModel";
                this.items = [];
                this._appObj = null;
                if (this._init != null) {
                    this._init.apply(this, arguments);
                }
            }
            TableViewModel.prototype.push = function (item) {
                this.items.push(item);
                if (this._itemPushed != null) {
                    this._itemPushed(this.items.length - 1);
                }
            };
            TableViewModel.prototype.remove = function (index) {
                if (index < 0 || index > this.items.length - 1)
                    return;
                this.items.splice(index, 1);
                if (this._itemRemoved != null) {
                    this._itemRemoved(index);
                }
            };
            TableViewModel.prototype.pop = function () {
                this.remove(this.items.length - 1);
            };
            TableViewModel.prototype.clear = function () {
                this.items.splice(0, this.items.length);
                if (this._itemsCleared != null) {
                    this._itemsCleared();
                }
            };
            TableViewModel.prototype.get = function (index) {
                if (index < 0 || index > this.items.length - 1)
                    return null;
                return this.items[index];
            };
            TableViewModel.xmlTag = { tag: "tableviewmodel", type: "TableViewModel" };
            return TableViewModel;
        }());
        MUI.TableViewModel = TableViewModel;
    })(MUI = RadJav.MUI || (RadJav.MUI = {}));
})(RadJav || (RadJav = {}));
