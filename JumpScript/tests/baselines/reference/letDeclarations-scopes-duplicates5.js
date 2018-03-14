//// [tests/cases/compiler/letDeclarations-scopes-duplicates5.jump] ////

//// [file1.jump]
const var1 = 0;

//// [file2.jump]
const var1 = 0;

//// [file1.js]
const var1 = 0;
//// [file2.js]
const var1 = 0;
