//// [tests/cases/compiler/noSymbolForMergeCrash.jump] ////

//// [initial.jump]
interface A { }
namespace A {}

//// [final.jump]
type A = {}


//// [initial.js]
//// [final.js]
