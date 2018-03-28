var RadJav;
(function (RadJav) {
    var Color = (function () {
        function Color(r, g, b, a) {
            if (r === void 0) { r = 0; }
            if (g === void 0) { g = 0; }
            if (b === void 0) { b = 0; }
            if (a === void 0) { a = 1; }
            if (typeof r == "object") {
                var color = r;
                this.r = color.r;
                this.g = color.g;
                this.b = color.b;
                this.a = color.a;
            }
            else {
                this.r = r;
                this.g = g;
                this.b = b;
                this.a = a;
            }
        }
        Color.prototype.toHex = function () {
            var red = this.r;
            var green = this.g;
            var blue = this.b;
            red *= 255;
            green *= 255;
            blue *= 255;
            red = parseInt(red).toString(16);
            green = parseInt(green).toString(16);
            blue = parseInt(blue).toString(16);
            if (parseInt(red) <= 9) {
                red = "0" + red;
            }
            if (parseInt(green) <= 9) {
                green = "0" + green;
            }
            if (parseInt(blue) <= 9) {
                blue = "0" + blue;
            }
            return "0x" + red + green + blue;
        };
        Color.prototype.toHTMLColor = function () {
            var hex = this.toHex();
            hex = hex.substring(2);
            return "#" + hex;
        };
        Color.prototype.toHexInt = function () {
            var hex = this.toHex();
            return parseInt(hex);
        };
        return Color;
    }());
    RadJav.Color = Color;
})(RadJav || (RadJav = {}));
RadJav.Color.Black = new RadJav.Color(0, 0, 0, 1);
RadJav.Color.White = new RadJav.Color(1, 1, 1, 1);
RadJav.Color.Red = new RadJav.Color(1, 0, 0, 1);
RadJav.Color.Green = new RadJav.Color(0, 1, 0, 1);
RadJav.Color.Blue = new RadJav.Color(0, 0, 1, 1);
;
