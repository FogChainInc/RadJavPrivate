//// [tests/cases/compiler/typeReferenceDirectives6.jump] ////

//// [ref.d.jump]
// $ comes from type declaration file - type reference directive should be added

declare let $: { x: number }
    
//// [index.d.jump]
interface $ { x }


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
/// <reference types="lib" />
declare let x: $;
declare let y: () => $;
