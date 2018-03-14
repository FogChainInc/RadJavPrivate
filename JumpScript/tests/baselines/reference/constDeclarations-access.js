//// [tests/cases/compiler/constDeclarations-access.jump] ////

//// [file1.jump]
const x = 0

//// [file2.jump]
x++;

//// [file1.js]
const x = 0;
//// [file2.js]
x++;
