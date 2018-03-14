//// [tests/cases/conformance/references/library-reference-6.jump] ////

//// [index.d.jump]
// The primary lookup folder is relative to jmpconfig.json, if present

declare var alpha: { a: string };

//// [foo.jump]
/// <reference types="alpha" />
var x: string = alpha.a;


//// [foo.js]
/// <reference types="alpha" />
var x = alpha.a;
