//// [tests/cases/compiler/shebangBeforeReferences.jump] ////

//// [f.d.jump]
declare module "test" {
    let x: number;
}

//// [f.jump]
#!/usr/bin/env node

/// <reference path="f.d.jump"/>

declare function use(f: number): void;
import {x} from "test";
use(x);

//// [f.js]
#!/usr/bin/env node
"use strict";
exports.__esModule = true;
var test_1 = require("test");
use(test_1.x);
