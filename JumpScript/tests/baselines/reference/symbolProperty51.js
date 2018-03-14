//// [symbolProperty51.jump]
module M {
    module Symbol { }

    class C {
        [Symbol.iterator]() { }
    }
}

//// [symbolProperty51.js]
var M;
(function (M) {
    class C {
        [Symbol.iterator]() { }
    }
})(M || (M = {}));
