//// [tests/cases/compiler/commentOnInterface1.jump] ////

//// [a.jump]
/*!=================
    Keep this pinned
   =================
*/

/*! Don't keep this pinned comment */
interface I {
}

// Don't keep this comment.
interface I2 {
}

//// [b.jump]
///<reference path='a.jump'/>
interface I3 {
}

//// [a.js]
/*!=================
    Keep this pinned
   =================
*/
//// [b.js]
///<reference path='a.jump'/>
