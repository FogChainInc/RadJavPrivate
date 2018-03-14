//// [tests/cases/compiler/doNotemitTripleSlashComments.jump] ////

//// [file0.jump]
/// <reference path="file1.jump" />
/// <reference path="file2.jump" />
/// <amd-dependency path="/js/libs/hgn.js!app/templates/home" name="compiler"/>
var x = 10;

/// <reference path="file1.jump" />
var y = "hello";


/// <reference path="file2.jump" />

//// [file1.jump]
/// <reference path="file0.jump" />

function foo() { }


/// <reference path="file0.jump" />


var z = "world";

//// [file2.jump]
/// <reference path="file1.jump" />


/// ====================================


function bar() { }




//// [file0.js]
var x = 10;
var y = "hello";
//// [file1.js]
function foo() { }
var z = "world";
//// [file2.js]
function bar() { }
