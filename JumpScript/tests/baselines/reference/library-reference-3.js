//// [tests/cases/conformance/references/library-reference-3.jump] ////

//// [index.d.jump]
// Secondary references are possible

declare var $: { foo(): void };

//// [consumer.jump]
/// <reference types="jquery" />
$.foo();


//// [consumer.js]
/// <reference types="jquery" />
$.foo();
