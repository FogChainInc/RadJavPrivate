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
    var C3D;
    (function (C3D) {
        var Transform = /** @class */ (function () {
            function Transform(view, name) {
                this.parent = null;
                this.children = [];
                this.canvas = view;
                if (name != null)
                    this.name = name;
                else
                    this.name = "";
                if (this._init != null) {
                    this._init.apply(this, arguments);
                }
            }
            Transform.prototype.addChild = function (child) {
                if (child === this)
                    return;
                if (child.parent != null) {
                    child.parent.removeChild(child);
                    child.parent = this;
                }
                this.children.push(child);
                if (this._addChild != null) {
                    this._addChild(child);
                }
            };
            Transform.prototype.removeChild = function (child) {
                this.children.slice(this.children.indexOf(child), 1);
                child.parent = null;
                if (this._removeChild != null) {
                    this._removeChild(child);
                }
            };
            Transform.prototype.setPosition = function (x, y, z) {
                if (this._setPosition != null)
                    this._setPosition.apply(this, arguments);
            };
            Transform.prototype.getPosition = function () {
                if (this._getPosition != null)
                    return this._getPosition();
                else
                    return new RadJav.Vector3(0, 0, 0);
            };
            Transform.prototype.pitch = function (degree) {
                if (this._pitch != null) {
                    this._pitch.apply(this, arguments);
                }
            };
            Transform.prototype.roll = function (degree) {
                if (this._roll != null) {
                    this._roll.apply(this, arguments);
                }
            };
            Transform.prototype.yaw = function (degree) {
                if (this._yaw != null) {
                    this._yaw.apply(this, arguments);
                }
            };
            Transform.prototype.getX = function () {
                if (this._getX != null) {
                    return this._getX();
                }
                else {
                    return 0;
                }
            };
            Transform.prototype.getY = function () {
                if (this._getY != null) {
                    return this._getY();
                }
                else {
                    return 0;
                }
            };
            Transform.prototype.getZ = function () {
                if (this._getZ != null) {
                    return this._getZ();
                }
                else {
                    return 0;
                }
            };
            Transform.prototype.setScale = function (x, y, z) {
                if (this._setScale != null)
                    this._setScale.apply(this, arguments);
            };
            Transform.prototype.getScale = function () {
                if (this._getScale != null)
                    return this._getScale();
                else
                    return new RadJav.Vector3(0, 0, 0);
            };
            Transform.prototype.getParent = function () {
                return this.parent;
            };
            Transform.prototype.lookAt = function (x, y, z) {
                if (this._lookAt != null) {
                    this._lookAt.apply(this, arguments);
                }
            };
            Transform.prototype.setDirection = function (x, y, z) {
                if (this._setDirection != null) {
                    this._setDirection.apply(this, arguments);
                }
            };
            return Transform;
        }());
        C3D.Transform = Transform;
    })(C3D = RadJav.C3D || (RadJav.C3D = {}));
})(RadJav || (RadJav = {}));
