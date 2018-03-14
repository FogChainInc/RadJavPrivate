//// [tests/cases/compiler/typeReferenceDirectives3.jump] ////

//// [ref.d.jump]
// $ comes from d.jump file - no need to add type reference directive

interface $ { x }

//// [index.d.jump]
declare let $: { x: number }

//// [app.jump]
/// <reference types="lib"/>
/// <reference path="ref.d.jump" />
interface A {
    x: () => $
}

//// [app.js]
/// <reference types="lib"/>
/// <reference path="ref.d.jump" />


//// [app.d.jump]
/// <reference path="ref.d.jump" />
interface A {
    x: () => $;
}
