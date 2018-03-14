//// [tests/cases/compiler/typeReferenceDirectives2.jump] ////

//// [index.d.jump]
interface $ { x }

//// [app.jump]
interface A {
    x: $
}

//// [app.js]


//// [app.d.jump]
/// <reference types="lib" />
interface A {
    x: $;
}
