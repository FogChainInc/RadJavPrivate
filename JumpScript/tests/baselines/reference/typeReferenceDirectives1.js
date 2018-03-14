//// [tests/cases/compiler/typeReferenceDirectives1.jump] ////

//// [index.d.jump]
interface $ { x }

//// [app.jump]
/// <reference types="lib"/>
interface A {
    x: $
}

//// [app.js]
/// <reference types="lib"/>


//// [app.d.jump]
/// <reference types="lib" />
interface A {
    x: $;
}
