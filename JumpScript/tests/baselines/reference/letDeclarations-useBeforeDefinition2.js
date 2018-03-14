//// [tests/cases/compiler/letDeclarations-useBeforeDefinition2.jump] ////

//// [file1.jump]
l;

//// [file2.jump]
const l = 0;

//// [out.js]
l;
const l = 0;
