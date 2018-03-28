var RadJav;
(function (RadJav) {
    var Vector4 = (function () {
        function Vector4(x, y, z, w) {
            if (x == null)
                x = 0;
            if (y == null)
                y = 0;
            if (z == null)
                z = 0;
            if (w == null)
                w = 0;
            if (x instanceof RadJav.Vector2) {
                var temp = x;
                x = temp.x;
                y = temp.y;
            }
            if (x instanceof RadJav.Vector3) {
                var temp = x;
                x = temp.x;
                y = temp.y;
                z = temp.z;
            }
            if (x instanceof RadJav.Vector4) {
                var temp = x;
                x = temp.x;
                y = temp.y;
                z = temp.z;
                w = temp.w;
            }
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }
        Vector4.prototype.toString = function () {
            return this.x + "," + this.y + "," + this.z + "," + this.w;
        };
        return Vector4;
    }());
})(RadJav || (RadJav = {}));
;
