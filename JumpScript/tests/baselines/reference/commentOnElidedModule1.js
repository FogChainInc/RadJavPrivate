//// [tests/cases/compiler/commentOnElidedModule1.jump] ////

//// [a.jump]
/*!=================
    Keep this pinned
   =================
*/

/*! Don't keep this pinned comment */
module ElidedModule {
}

// Don't keep this comment.
module ElidedModule2 {
}

//// [b.jump]
///<reference path="a.jump"/>
module ElidedModule3 {
}

//// [a.js]
/*!=================
    Keep this pinned
   =================
*/
//// [b.js]
///<reference path="a.jump"/>
