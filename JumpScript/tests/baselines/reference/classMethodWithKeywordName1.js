//// [classMethodWithKeywordName1.jump]
class C {
 static try() {}
}

//// [classMethodWithKeywordName1.js]
var C = /** @class */ (function () {
    function C() {
    }
    C["try"] = function () { };
    return C;
}());
