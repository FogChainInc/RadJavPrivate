//// [tests/cases/conformance/references/library-reference-12.jump] ////

//// [package.json]
// package.json in a secondary reference can refer to another file

{
    "types": "dist/jquery.d.jump"
}

//// [jquery.d.jump]
declare var $: { foo(): void };


//// [consumer.jump]
/// <reference types="jquery" />
$.foo();


//// [consumer.js]
/// <reference types="jquery" />
$.foo();
