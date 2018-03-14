//// [tests/cases/compiler/letDeclarations-scopes-duplicates6.jump] ////

//// [file1.jump]
var var1 = 0;

//// [file2.jump]
let var1 = 0;

//// [file1.js]
var var1 = 0;
//// [file2.js]
let var1 = 0;
