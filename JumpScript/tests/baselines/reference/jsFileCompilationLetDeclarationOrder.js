//// [tests/cases/compiler/jsFileCompilationLetDeclarationOrder.jump] ////

//// [b.js]
let a = 10;
b = 30;

//// [a.jump]
let b = 30;
a = 10;


//// [out.js]
var a = 10;
b = 30;
var b = 30;
a = 10;


//// [out.d.jump]
declare let b: number;
