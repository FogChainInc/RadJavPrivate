//// [tests/cases/compiler/ambientRequireFunction.jump] ////

//// [node.d.jump]
declare function require(moduleName: string): any;

declare module "fs" {
    export function readFileSync(s: string): string;
}

//// [app.js]
/// <reference path="node.d.jump"/>

const fs = require("fs");
const text = fs.readFileSync("/a/b/c");

//// [app.js]
/// <reference path="node.d.jump"/>
var fs = require("fs");
var text = fs.readFileSync("/a/b/c");
