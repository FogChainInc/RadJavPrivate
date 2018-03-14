//// [tests/cases/compiler/jsFileCompilationDuplicateVariable.jump] ////

//// [a.jump]
var x = 10;

//// [b.js]
var x = "hello"; // No error is recorded here and declaration file will show this as number

//// [out.js]
var x = 10;
var x = "hello"; // No error is recorded here and declaration file will show this as number


//// [out.d.jump]
declare var x: number;
