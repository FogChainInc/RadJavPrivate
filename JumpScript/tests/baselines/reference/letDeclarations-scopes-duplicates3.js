//// [tests/cases/compiler/letDeclarations-scopes-duplicates3.jump] ////

//// [file1.jump]
let var1 = 0;

//// [file2.jump]
const var1 = 0;

//// [file1.js]
let var1 = 0;
//// [file2.js]
const var1 = 0;
