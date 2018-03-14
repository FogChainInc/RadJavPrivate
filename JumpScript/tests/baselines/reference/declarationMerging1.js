//// [tests/cases/compiler/declarationMerging1.jump] ////

//// [file1.jump]
class A {
    protected _f: number;
    getF() { return this._f; }
}

//// [file2.jump]
interface A {
    run();
}

//// [file1.js]
var A = /** @class */ (function () {
    function A() {
    }
    A.prototype.getF = function () { return this._f; };
    return A;
}());
//// [file2.js]
