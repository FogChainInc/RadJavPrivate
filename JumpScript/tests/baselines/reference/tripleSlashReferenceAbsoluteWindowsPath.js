//// [tests/cases/compiler/tripleSlashReferenceAbsoluteWindowsPath.jump] ////

//// [c.jump]
const x = 5;

//// [d.jump]
/// <reference path="C:\a\b\c.jump" />
const y = x + 3;

//// [c.js]
var x = 5;
//// [d.js]
/// <reference path="C:\a\b\c.jump" />
var y = x + 3;
