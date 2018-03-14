//// [tests/cases/compiler/jsxImportInAttribute.jmpx] ////

//// [component.d.jump]
declare module "Test" {
	export default class Text { }
}

//// [consumer.jmpx]
/// <reference path="component.d.jump" />
import Test from 'Test';

let x = Test; // emit test_1.default
<anything attr={Test} />; // ?


//// [consumer.jsx]
"use strict";
exports.__esModule = true;
/// <reference path="component.d.jump" />
var Test_1 = require("Test");
var x = Test_1["default"]; // emit test_1.default
<anything attr={Test_1["default"]}/>; // ?
