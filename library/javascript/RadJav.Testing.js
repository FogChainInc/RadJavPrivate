var RadJav;
(function (RadJav) {
    var Testing;
    (function (Testing) {
        var KeyboardSimulator = (function () {
            function KeyboardSimulator() {
            }
            KeyboardSimulator.keyPress = function (key) {
            };
            return KeyboardSimulator;
        }());
        Testing.KeyboardSimulator = KeyboardSimulator;
        var MouseSimulator = (function () {
            function MouseSimulator() {
            }
            MouseSimulator.click = function (button) {
            };
            MouseSimulator.setPosition = function (pos) {
            };
            return MouseSimulator;
        }());
        Testing.MouseSimulator = MouseSimulator;
        var TouchSimulator = (function () {
            function TouchSimulator() {
            }
            TouchSimulator.setPosition = function (pos) {
            };
            return TouchSimulator;
        }());
        Testing.TouchSimulator = TouchSimulator;
        var AccelerometerSimulator = (function () {
            function AccelerometerSimulator() {
            }
            AccelerometerSimulator.shake = function (pos) {
            };
            return AccelerometerSimulator;
        }());
        Testing.AccelerometerSimulator = AccelerometerSimulator;
    })(Testing = RadJav.Testing || (RadJav.Testing = {}));
})(RadJav || (RadJav = {}));
//# sourceMappingURL=RadJav.Testing.js.map