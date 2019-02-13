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
        var NavigationViewController = (function (_super) {
            __extends(NavigationViewController, _super);
            function NavigationViewController(obj, text, parent) {
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
                _this.type = "RadJav.MUI.NavigationViewController";
                return _this;
            }
            NavigationViewController.xmlTag = { tag: "navigationviewcontroller", type: "NavigationViewController" };
            return NavigationViewController;
        }(RadJav.MUI.ViewController));
        MUI.NavigationViewController = NavigationViewController;
    })(MUI = RadJav.MUI || (RadJav.MUI = {}));
})(RadJav || (RadJav = {}));
