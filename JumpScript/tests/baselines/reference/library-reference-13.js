//// [tests/cases/conformance/references/library-reference-13.jump] ////

//// [index.d.jump]
declare var $: { foo(): void };


//// [consumer.jump]
$.foo();


//// [consumer.js]
$.foo();
