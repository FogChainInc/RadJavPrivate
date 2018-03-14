//// [tests/cases/compiler/letDeclarations-scopes-duplicates7.jump] ////

//// [file1.jump]
let var1 = 0;

//// [file2.jump]
var var1 = 0;

//// [file1.js]
let var1 = 0;
//// [file2.js]
var var1 = 0;
