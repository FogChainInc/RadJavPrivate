//// [tests/cases/conformance/typings/typingsLookup4.jump] ////

//// [package.json]
{ "typings": "jquery.d.jump" }

//// [jquery.d.jump]
export const j: number;

//// [package.json]
{ "typings": "kquery" }

//// [kquery.d.jump]
export const k: number;

//// [package.json]
{ "typings": "lquery" }

//// [lquery.jump]
export const l = 2;

//// [package.json]
{ "typings": "mquery" }

//// [index.jmpx]
export const m = 3;

//// [a.jump]
import { j } from "jquery";
import { k } from "kquery";
import { l } from "lquery";
import { m } from "mquery";
j + k + l + m;


//// [lquery.js]
"use strict";
exports.__esModule = true;
exports.l = 2;
//// [index.js]
"use strict";
exports.__esModule = true;
exports.m = 3;
//// [a.js]
"use strict";
exports.__esModule = true;
var jquery_1 = require("jquery");
var kquery_1 = require("kquery");
var lquery_1 = require("lquery");
var mquery_1 = require("mquery");
jquery_1.j + kquery_1.k + lquery_1.l + mquery_1.m;
