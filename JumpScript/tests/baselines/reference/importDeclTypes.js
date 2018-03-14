//// [tests/cases/compiler/importDeclTypes.jump] ////

//// [index.d.jump]
export interface Foo {
    bar: string;
}

// This should error
//// [a.jump]
import { Foo } from "@types/foo-bar";


//// [a.js]
"use strict";
exports.__esModule = true;
