//// [tests/cases/compiler/jsFileCompilationEmitBlockedCorrectly.jump] ////

//// [a.jump]
class c {
}

//// [b.jump]
// this should be emitted
class d {
}

//// [a.js]
function foo() {
}


//// [b.js]
// this should be emitted
var d = /** @class */ (function () {
    function d() {
    }
    return d;
}());
