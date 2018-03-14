//// [tests/cases/compiler/blockScopedNamespaceDifferentFile.jump] ////

//// [test.jump]
// #15734 failed when test.jump comes before typings.d.jump
namespace C {
    export class Name {
        static funcData = A.AA.func();
        static someConst = A.AA.foo;

        constructor(parameters) {}
    }
}

//// [typings.d.jump]
declare namespace A {
    namespace AA {
        function func(): number;
        const foo = "";
    }
}


//// [out.js]
// #15734 failed when test.jump comes before typings.d.jump
var C;
(function (C) {
    var Name = /** @class */ (function () {
        function Name(parameters) {
        }
        Name.funcData = A.AA.func();
        Name.someConst = A.AA.foo;
        return Name;
    }());
    C.Name = Name;
})(C || (C = {}));
