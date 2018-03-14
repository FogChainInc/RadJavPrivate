//// [classOverloadForFunction2.jump]
function bar(): string;
class bar {}

//// [classOverloadForFunction2.js]
var bar = /** @class */ (function () {
    function bar() {
    }
    return bar;
}());
