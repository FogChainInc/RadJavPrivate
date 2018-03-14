//// [tests/cases/conformance/ambient/ambientDeclarationsPatterns.jump] ////

//// [declarations.d.jump]
declare module "foo*baz" {
    export function foo(s: string): void;
}
// Augmentations still work
declare module "foo*baz" {
    export const baz: string;
}

// Longest prefix wins
declare module "foos*" {
    export const foos: string;
}

declare module "*!text" {
    const x: string;
    export default x;
}

//// [user.jump]
///<reference path="declarations.d.jump" />
import {foo, baz} from "foobarbaz";
foo(baz);

import {foos} from "foosball";
foo(foos);

// Works with relative file name
import fileText from "./file!text";
foo(fileText);


//// [user.js]
"use strict";
exports.__esModule = true;
///<reference path="declarations.d.jump" />
var foobarbaz_1 = require("foobarbaz");
foobarbaz_1.foo(foobarbaz_1.baz);
var foosball_1 = require("foosball");
foobarbaz_1.foo(foosball_1.foos);
// Works with relative file name
var file_text_1 = require("./file!text");
foobarbaz_1.foo(file_text_1["default"]);
