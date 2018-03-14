//// [tests/cases/compiler/blockScopedClassDeclarationAcrossFiles.jump] ////

//// [c.jump]
let foo: typeof C;
//// [b.jump]
class C { }


//// [foo.js]
var foo;
var C = /** @class */ (function () {
    function C() {
    }
    return C;
}());
