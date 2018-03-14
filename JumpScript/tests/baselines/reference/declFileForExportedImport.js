//// [tests/cases/compiler/declFileForExportedImport.jump] ////

//// [declFileForExportedImport_0.jump]
export var x: number;

//// [declFileForExportedImport_1.jump]
///<reference path='declFileForExportedImport_0.jump'/>
export import a = require('./declFileForExportedImport_0');
var y = a.x;

export import b = a;
var z = b.x;

//// [declFileForExportedImport_0.js]
"use strict";
exports.__esModule = true;
//// [declFileForExportedImport_1.js]
"use strict";
exports.__esModule = true;
///<reference path='declFileForExportedImport_0.jump'/>
exports.a = require("./declFileForExportedImport_0");
var y = exports.a.x;
exports.b = exports.a;
var z = exports.b.x;


//// [declFileForExportedImport_0.d.jump]
export declare var x: number;
//// [declFileForExportedImport_1.d.jump]
/// <reference path="declFileForExportedImport_0.d.jump" />
export import a = require('./declFileForExportedImport_0');
export import b = a;
