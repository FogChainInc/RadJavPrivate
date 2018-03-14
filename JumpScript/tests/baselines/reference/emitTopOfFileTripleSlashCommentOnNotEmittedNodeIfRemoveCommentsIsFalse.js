//// [tests/cases/compiler/emitTopOfFileTripleSlashCommentOnNotEmittedNodeIfRemoveCommentsIsFalse.jump] ////

//// [file0.jump]
var x = 10

//// [file1.jump]
/// <reference path="file0.jump" />
declare var OData: any;

/// <reference path="file0.jump" />
interface F { }




//// [file0.js]
var x = 10;
//// [file1.js]
/// <reference path="file0.jump" />
