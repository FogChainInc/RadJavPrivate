//// [tests/cases/compiler/duplicatePackage_relativeImportWithinPackage_scoped.jump] ////

//// [package.json]
{
    "name": "@foo/bar",
    "version": "1.2.3"
}

//// [index.d.jump]
export class C {
    private x: number;
}

//// [index.d.jump]
import { C } from "@foo/bar";
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
    "name": "@foo/bar",
    "version": "1.2.3"
}

//// [index.jump]
import { use } from "@foo/bar/use";
import { o } from "a";

use(o);


//// [index.js]
"use strict";
exports.__esModule = true;
var use_1 = require("@foo/bar/use");
var a_1 = require("a");
use_1.use(a_1.o);
