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
        var Textbox = (function (_super) {
            __extends(Textbox, _super);
            function Textbox(obj, text, parent) {
                var _this = this;
                if (obj == null)
                    obj = {};
                if (typeof obj == "string") {
                    var name = obj;
                    obj = { name: name };
                }
                if (obj.size == null) {
                    obj.size = new RadJav.Vector2();
                    obj.size.x = 80;
                    obj.size.y = 40;
                }
                _this = _super.call(this, obj, text, parent) || this;
                _this.type = "RadJav.MUI.Textbox";
                return _this;
            }
            Textbox.xmlTag = { tag: "textbox", type: "Textbox" };
            Textbox.InputModeText = 1;
            Textbox.InputModeNumber = 2;
            Textbox.InputModeDecimal = 3;
            Textbox.InputModePhone = 4;
            Textbox.InputModeEmail = 5;
            Textbox.InputModePassword = 6;
            return Textbox;
        }(RadJav.GUI.GObject));
        MUI.Textbox = Textbox;
    })(MUI = RadJav.MUI || (RadJav.MUI = {}));
})(RadJav || (RadJav = {}));
