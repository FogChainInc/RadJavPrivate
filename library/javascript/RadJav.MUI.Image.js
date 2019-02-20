var __extends = (this && this.__extends) || (function () {
    var extendStatics = Object.setPrototypeOf ||
        ({ __proto__: [] } instanceof Array && function (d, b) { d.__proto__ = b; }) ||
        function (d, b) { for (var p in b) if (b.hasOwnProperty(p)) d[p] = b[p]; };
    return function (d, b) {
        extendStatics(d, b);
        function __() { this.constructor = d; }
        d.prototype = b === null ? Object.create(b) : (__.prototype = b.prototype, new __());
    };
})();
var RadJav;
(function (RadJav) {
    var MUI;
    (function (MUI) {
        var Image = (function (_super) {
            __extends(Image, _super);
            function Image(obj, text, parent) {
                var _this = this;
                if (obj == null)
                    obj = {};
                if (typeof obj == "string") {
                    var name = obj;
                    obj = { name: name };
                }
                if (obj.size == null) {
                    obj.size = new RadJav.Vector2();
                    obj.size.x = 100;
                    obj.size.y = 100;
                }
                _this = _super.call(this, obj, text, parent) || this;
                _this.type = "RadJav.MUI.Image";
                return _this;
            }
            Image.xmlTag = { tag: "image", type: "Image" };
            Image.ScaleModeAspectFit = 1;
            Image.ScaleModeAspectFill = 2;
            return Image;
        }(RadJav.GUI.GObject));
        MUI.Image = Image;
    })(MUI = RadJav.MUI || (RadJav.MUI = {}));
})(RadJav || (RadJav = {}));
if (RadJav.GUI != null)
    RadJav.GUI["Image"] = RadJav.MUI.Image;
