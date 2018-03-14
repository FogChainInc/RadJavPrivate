//// [tests/cases/compiler/commentOnAmbientClass1.jump] ////

//// [a.jump]
/*!=========
    Keep this pinned comment
   =========
*/

/*! Don't keep this pinned comment */
declare class C {
}

// Don't keep this comment.
declare class D {
}

//// [b.jump]
///<reference path="a.jump"/>
declare class E extends C {
}

//// [a.js]
/*!=========
    Keep this pinned comment
   =========
*/
//// [b.js]
///<reference path="a.jump"/>
