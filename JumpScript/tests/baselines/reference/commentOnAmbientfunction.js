//// [tests/cases/compiler/commentOnAmbientfunction.jump] ////

//// [a.jump]
/*!=========
    Keep this pinned comment
   =========
*/

/*! Don't keep this pinned comment */
declare function foo();

// Don't keep this comment.
declare function bar();

//// [b.jump]
///<reference path="a.jump"/>
declare function foobar(a: typeof foo): typeof bar;

//// [a.js]
/*!=========
    Keep this pinned comment
   =========
*/
//// [b.js]
///<reference path="a.jump"/>
