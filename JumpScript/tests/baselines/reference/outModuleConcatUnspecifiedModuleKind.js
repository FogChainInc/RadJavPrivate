//// [tests/cases/compiler/outModuleConcatUnspecifiedModuleKind.jump] ////

//// [a.jump]
export class A { } // module

//// [b.jump]
var x = 0; // global

//// [out.js]
var x = 0; // global
