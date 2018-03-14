//// [tests/cases/conformance/ambient/ambientShorthand.jump] ////

//// [declarations.d.jump]
declare module "jquery"
// Semicolon is optional
declare module "fs";

//// [user.jump]
///<reference path="declarations.d.jump"/>
import foo, {bar} from "jquery";
import * as baz from "fs";
import boom = require("jquery");
foo(bar, baz, boom);


//// [user.js]
"use strict";
exports.__esModule = true;
///<reference path="declarations.d.jump"/>
var jquery_1 = require("jquery");
var baz = require("fs");
var boom = require("jquery");
jquery_1["default"](jquery_1.bar, baz, boom);
