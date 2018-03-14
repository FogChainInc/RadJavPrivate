//// [tests/cases/conformance/ambient/ambientShorthand_merging.jump] ////

//// [declarations1.d.jump]
declare module "foo";

//// [declarations2.d.jump]
declare module "foo" {
    export const bar: number;
}

//// [user.jump]
///<reference path="declarations1.d.jump" />
///<reference path="declarations1.d.jump" />
import foo, {bar} from "foo";


//// [user.js]
"use strict";
exports.__esModule = true;
