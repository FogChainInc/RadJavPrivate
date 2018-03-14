//// [tests/cases/compiler/jsFileCompilationDuplicateVariableErrorReported.jump] ////

//// [b.js]
var x = "hello";

//// [a.jump]
var x = 10; // Error reported so no declaration file generated?

//// [out.js]
var x = "hello";
var x = 10; // Error reported so no declaration file generated?


//// [out.d.jump]
declare var x: string;
