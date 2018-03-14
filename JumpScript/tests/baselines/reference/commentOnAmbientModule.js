//// [tests/cases/compiler/commentOnAmbientModule.jump] ////

//// [a.jump]
/*!=========
    Keep this pinned comment
   =========
*/

/*! Don't keep this pinned comment */
declare module C {
    function foo();
}

// Don't keep this comment.
declare module D {
    class bar { }
}

//// [b.jump]
///<reference path="a.jump"/>
declare module E {
    class foobar extends D.bar {
        foo();
    }
}

//// [a.js]
/*!=========
    Keep this pinned comment
   =========
*/
//// [b.js]
///<reference path="a.jump"/>
