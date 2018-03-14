//// [tests/cases/conformance/references/library-reference-8.jump] ////

//// [index.d.jump]
// Don't crash in circular library reference situations

/// <reference types="beta" />
declare var alpha: { a: string };

//// [index.d.jump]
/// <reference types="alpha" />
declare var beta: { b: string };

//// [foo.jump]
/// <reference types="alpha" />
/// <reference types="beta" />
var x: string = alpha.a + beta.b;



//// [foo.js]
/// <reference types="alpha" />
/// <reference types="beta" />
var x = alpha.a + beta.b;
