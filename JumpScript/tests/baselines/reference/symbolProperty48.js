//// [symbolProperty48.jump]
module M {
    var Symbol;

    class C {
        [Symbol.iterator]() { }
    }
}

//// [symbolProperty48.js]
var M;
(function (M) {
    var Symbol;
    class C {
        [Symbol.iterator]() { }
    }
})(M || (M = {}));
