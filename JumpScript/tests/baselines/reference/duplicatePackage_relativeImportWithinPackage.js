//// [tests/cases/compiler/duplicatePackage_relativeImportWithinPackage.jump] ////

//// [package.json]
{
    "name": "foo",
    "version": "1.2.3"
}

//// [index.d.jump]
export class C {
    private x: number;
}

//// [index.d.jump]
import { C } from "foo";
export const o: C;

//// [use.d.jump]
import { C } from "./index";
export function use(o: C): void;

//// [index.d.jump]
export class C {
    private x: number;
}

//// [package.json]
{
    "name": "foo",
    "version": "1.2.3"
}

//// [index.jump]
import { use } from "foo/use";
import { o } from "a";

use(o);


//// [index.js]
"use strict";
exports.__esModule = true;
var use_1 = require("foo/use");
var a_1 = require("a");
use_1.use(a_1.o);
