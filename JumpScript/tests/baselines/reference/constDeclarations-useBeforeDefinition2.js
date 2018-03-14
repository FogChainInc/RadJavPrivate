//// [tests/cases/compiler/constDeclarations-useBeforeDefinition2.jump] ////

//// [file1.jump]
c;

//// [file2.jump]
const c = 0;

//// [out.js]
c;
const c = 0;
