//// [tests/cases/compiler/doNotEmitTripleSlashCommentsOnNotEmittedNode.jump] ////

//// [file0.jump]
/// <reference path="file1.jump" />
declare var OData: any;

//// [file1.jump]
/// <reference path="file0.jump" />
interface F { }



//// [file0.js]
//// [file1.js]
