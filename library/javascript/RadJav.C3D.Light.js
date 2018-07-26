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
        var LightType;
        (function (LightType) {
            LightType[LightType["Directional"] = 1] = "Directional";
            LightType[LightType["Point"] = 2] = "Point";
            LightType[LightType["Spot"] = 3] = "Spot";
        })(LightType = C3D.LightType || (C3D.LightType = {}));
        var Light = /** @class */ (function (_super) {
            __extends(Light, _super);
            function Light(view, name) {
                var _this = _super.call(this, view, name) || this;
                _this.lightType = LightType.Directional;
                if (_this._init != null) {
                    _this._init.apply(_this, arguments);
                }
                return _this;
            }
            Light.prototype.setType = function (type) {
                this.lightType = type;
                if (this._setType != null) {
                    this._setType.apply(this, arguments);
                }
            };
            Light.prototype.getType = function () {
                if (this._getType != null) {
                    return this._getType();
                }
            };
            Light.prototype.setColor = function (color) {
                if (this._setColor != null) {
                    this._setColor.apply(this, arguments);
                }
            };
            Light.prototype.getColor = function () {
                if (this._getColor != null) {
                    return this._getColor();
                }
            };
            return Light;
        }(C3D.Object3D));
        C3D.Light = Light;
    })(C3D = RadJav.C3D || (RadJav.C3D = {}));
})(RadJav || (RadJav = {}));
