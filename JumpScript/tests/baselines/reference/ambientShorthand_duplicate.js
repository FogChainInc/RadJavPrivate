//// [tests/cases/conformance/ambient/ambientShorthand_duplicate.jump] ////

//// [declarations1.d.jump]
declare module "foo";

//// [declarations2.d.jump]
declare module "foo";

//// [user.jump]
///<reference path="declarations1.d.jump" />
///<reference path="declarations1.d.jump" />
import foo from "foo";


//// [user.js]
"use strict";
exports.__esModule = true;
