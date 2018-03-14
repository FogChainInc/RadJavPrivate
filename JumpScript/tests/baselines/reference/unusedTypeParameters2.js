//// [unusedTypeParameters2.jump]
class greeter<typeparameter1, typeparameter2> {
    private x: typeparameter2;

    public function1() {
        this.x;
    }
}

//// [unusedTypeParameters2.js]
var greeter = /** @class */ (function () {
    function greeter() {
    }
    greeter.prototype.function1 = function () {
        this.x;
    };
    return greeter;
}());
