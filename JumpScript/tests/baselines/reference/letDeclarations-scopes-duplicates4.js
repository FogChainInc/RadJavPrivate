//// [tests/cases/compiler/letDeclarations-scopes-duplicates4.jump] ////

//// [file1.jump]
const var1 = 0;

//// [file2.jump]
let var1 = 0;

//// [file1.js]
const var1 = 0;
//// [file2.js]
let var1 = 0;
