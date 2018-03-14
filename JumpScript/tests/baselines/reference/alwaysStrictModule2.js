//// [tests/cases/compiler/alwaysStrictModule2.jump] ////

//// [a.jump]
module M {
    export function f() {
        var arguments = [];
    }
}

//// [b.jump]
module M {
    export function f2() {
        var arguments = [];
    }
}

//// [out.js]
"use strict";
var M;
(function (M) {
    function f() {
        var arguments = [];
    }
    M.f = f;
})(M || (M = {}));
var M;
(function (M) {
    function f2() {
        var arguments = [];
    }
    M.f2 = f2;
})(M || (M = {}));
