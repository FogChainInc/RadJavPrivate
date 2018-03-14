//// [tests/cases/compiler/typeReferenceDirectives4.jump] ////

//// [ref.d.jump]
// $ comes from d.jump file - no need to add type reference directive

interface $ { x }

//// [index.d.jump]
declare let $: { x: number }


//// [app.jump]
/// <reference path="./ref.d.jump"/>
/// <reference types="lib"/>

let x: $;
let y = () => x

//// [app.js]
/// <reference path="./ref.d.jump"/>
/// <reference types="lib"/>
var x;
var y = function () { return x; };


//// [app.d.jump]
/// <reference path="ref.d.jump" />
declare let x: $;
declare let y: () => $;
