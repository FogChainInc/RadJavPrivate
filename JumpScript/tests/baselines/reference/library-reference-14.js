//// [tests/cases/conformance/references/library-reference-14.jump] ////

//// [index.d.jump]
declare var $: { foo(): void };


//// [consumer.jump]
$.foo();


//// [consumer.js]
$.foo();
