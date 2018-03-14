//// [separate1-2.jump]
module X {
    export function f() { }
}

//// [separate1-2.js]
var X;
(function (X) {
    function f() { }
    X.f = f;
})(X || (X = {}));
