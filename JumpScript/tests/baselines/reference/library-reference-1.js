//// [tests/cases/conformance/references/library-reference-1.jump] ////

//// [index.d.jump]
// We can find typings in the ./types folder

declare var $: { foo(): void };


//// [consumer.jump]
/// <reference types="jquery" />
$.foo();


//// [consumer.js]
/// <reference types="jquery" />
$.foo();
