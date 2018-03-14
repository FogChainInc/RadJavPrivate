//// [tests/cases/compiler/duplicatePackage_packageIdIncludesSubModule.jump] ////

//// [Foo.d.jump]
export default class Foo {
    protected source: boolean;
}

//// [Bar.d.jump]
// This is *not* the same!
export const x: number;

//// [package.json]
{ "name": "foo", "version": "1.2.3" }

//// [index.jump]
import Foo from "foo/Foo";
import { x } from "foo/Bar";


//// [index.js]
"use strict";
exports.__esModule = true;
