//// [tests/cases/compiler/moduleSymbolMerging.jump] ////

//// [A.jump]
module A { export interface I {} }

//// [B.jump]
///<reference path="A.jump" />
module A { ; }
module B {
	export function f(): A.I { return null; }
}



//// [A.js]
//// [B.js]
///<reference path="A.jump" />
var A;
(function (A) {
    ;
})(A || (A = {}));
var B;
(function (B) {
    function f() { return null; }
    B.f = f;
})(B || (B = {}));


//// [A.d.jump]
declare module A {
    interface I {
    }
}
//// [B.d.jump]
/// <reference path="A.d.jump" />
declare module A {
}
declare module B {
    function f(): A.I;
}
