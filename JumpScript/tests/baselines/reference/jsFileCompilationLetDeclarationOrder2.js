//// [tests/cases/compiler/jsFileCompilationLetDeclarationOrder2.jump] ////

//// [a.jump]
let b = 30;
a = 10;
//// [b.js]
let a = 10;
b = 30;


//// [out.js]
var b = 30;
a = 10;
var a = 10;
b = 30;


//// [out.d.jump]
declare let b: number;
