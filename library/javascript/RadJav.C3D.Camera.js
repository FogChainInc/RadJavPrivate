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
        var Camera = /** @class */ (function (_super) {
            __extends(Camera, _super);
            function Camera(view, name) {
                var _this = _super.call(this, view, name) || this;
                if (_this._init != null) {
                    _this._init.apply(_this, arguments);
                }
                return _this;
            }
            Camera.prototype.setPerspective = function (perpective) {
                if (this._setPerspective != null) {
                    this._setPerspective.apply(this, arguments);
                }
            };
            Camera.prototype.isPerspective = function () {
                if (this._isPerspective != null) {
                    return this._isPerspective();
                }
            };
            Camera.prototype.setAspectRatio = function (aspect) {
                if (this._setAspectRatio != null) {
                    this._setAspectRatio.apply(this, arguments);
                }
            };
            Camera.prototype.getAspectRatio = function () {
                if (this._getAspectRatio != null) {
                    return this._getAspectRatio();
                }
            };
            Camera.prototype.setAutoAspectRatio = function (auto) {
                if (this._setAutoAspectRatio != null) {
                    this._setAutoAspectRatio.apply(this, arguments);
                }
            };
            Camera.prototype.getAutoAspectRatio = function () {
                if (this._getAutoAspectRatio != null) {
                    return this._getAutoAspectRatio();
                }
            };
            Camera.prototype.setFOV = function (degree) {
                if (this._setFOV != null) {
                    this._setFOV.apply(this, arguments);
                }
            };
            Camera.prototype.getFOV = function () {
                if (this._getFOV != null) {
                    return this._getFOV();
                }
            };
            Camera.prototype.setNearClipPlane = function (distance) {
                if (this._setNearClipPlane != null) {
                    this._setNearClipPlane.apply(this, arguments);
                }
            };
            Camera.prototype.getNearClipPlane = function () {
                if (this._getNearClipPlane != null) {
                    return this._getNearClipPlane();
                }
            };
            Camera.prototype.setFarClipPlane = function (distance) {
                if (this._setFarClipPlane != null) {
                    this._setFarClipPlane.apply(this, arguments);
                }
            };
            Camera.prototype.getFarClipPlane = function () {
                if (this._getFarClipPlane != null) {
                    return this._getFarClipPlane();
                }
            };
            Camera.prototype.setBackgroundColor = function (color) {
                if (this._setBackgroundColor != null) {
                    this._setBackgroundColor.apply(this, arguments);
                }
            };
            Camera.prototype.getBackgroundColor = function () {
                if (this._getBackgroundColor != null) {
                    return this._getBackgroundColor();
                }
            };
            return Camera;
        }(C3D.Object3D));
        C3D.Camera = Camera;
    })(C3D = RadJav.C3D || (RadJav.C3D = {}));
})(RadJav || (RadJav = {}));
