//// [tests/cases/conformance/references/library-reference-15.jump] ////

//// [index.d.jump]
declare var $: { foo(): void };

//// [index.d.jump]
declare var $2: { foo(): void };

//// [consumer.jump]
$.foo(); // should OK
$2.foo(); // should error

//// [consumer.js]
$.foo(); // should OK
$2.foo(); // should error
