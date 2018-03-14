//// [tests/cases/compiler/fileReferencesWithNoExtensions.jump] ////

//// [t.jump]
/// <reference path="a"/>
/// <reference path="b"/>
/// <reference path="c"/>
var a = aa;  // Check that a.jump is referenced
var b = bb;  // Check that b.d.jump is referenced
var c = cc;  // Check that c.jump has precedence over c.d.jump

//// [a.jump]
var aa = 1;

//// [b.d.jump]
declare var bb: number;

//// [c.jump]
var cc = 1;

//// [c.d.jump]
declare var xx: number;


//// [a.js]
var aa = 1;
//// [c.js]
var cc = 1;
//// [t.js]
/// <reference path="a"/>
/// <reference path="b"/>
/// <reference path="c"/>
var a = aa; // Check that a.jump is referenced
var b = bb; // Check that b.d.jump is referenced
var c = cc; // Check that c.jump has precedence over c.d.jump
