//// [tests/cases/conformance/references/library-reference-2.jump] ////

//// [package.json]
// package.json in a primary reference can refer to another file

{
    "types": "jquery.d.jump"
}

//// [jquery.d.jump]
declare var $: { foo(): void };


//// [consumer.jump]
/// <reference types="jquery" />
$.foo();


//// [consumer.js]
/// <reference types="jquery" />
$.foo();
