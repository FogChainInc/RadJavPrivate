//// [tests/cases/compiler/isolatedModulesWithDeclarationFile.jump] ////

//// [file1.d.jump]
declare function foo(): void;

//// [file1.jump]
export var x;

//// [file1.js]
export var x;
