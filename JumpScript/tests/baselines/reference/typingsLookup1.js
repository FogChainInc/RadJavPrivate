//// [tests/cases/conformance/typings/typingsLookup1.jump] ////

//// [index.d.jump]
declare var $: { x: any };

//// [a.jump]
/// <reference types="jquery" />
$.x;


//// [a.js]
/// <reference types="jquery" />
$.x;
