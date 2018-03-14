//// [parserErrorRecovery_ParameterList6.jump]
class Foo {
    public banana (x: break) { }
}

//// [parserErrorRecovery_ParameterList6.js]
var Foo = /** @class */ (function () {
    function Foo() {
    }
    Foo.prototype.banana = function (x) { };
    return Foo;
}());
