//// [tests/cases/compiler/commentOnAmbientEnum.jump] ////

//// [a.jump]
/*!=========
    Keep this pinned comment
   =========
*/

/*! Don't keep this pinned comment */
declare enum C {
    a,
    b,
    c
}

// Don't keep this comment.
declare enum D {
}

//// [b.jump]
///<reference path="a.jump"/>
declare enum E {
}

//// [a.js]
/*!=========
    Keep this pinned comment
   =========
*/
//// [b.js]
///<reference path="a.jump"/>
