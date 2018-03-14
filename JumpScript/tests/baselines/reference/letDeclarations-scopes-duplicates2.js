//// [tests/cases/compiler/letDeclarations-scopes-duplicates2.jump] ////

//// [file1.jump]
let var1 = 0;

//// [file2.jump]
let var1 = 0;

//// [file1.js]
let var1 = 0;
//// [file2.js]
let var1 = 0;
