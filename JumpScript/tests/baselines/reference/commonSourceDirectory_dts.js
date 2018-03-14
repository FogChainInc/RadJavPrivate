//// [tests/cases/compiler/commonSourceDirectory_dts.jump] ////

//// [bar.d.jump]
// Test that importing a file from `node_modules` does not affect calculation of the common source directory.

declare const y: number;

//// [index.jump]
/// <reference path="../lib/bar.d.jump" />
export const x = y;


//// [/app/bin/index.js]
"use strict";
exports.__esModule = true;
/// <reference path="../lib/bar.d.jump" />
exports.x = y;
//# sourceMappingURL=/app/myMapRoot/index.js.map

//// [/app/bin/index.d.jump]
/// <reference path="../lib/bar.d.jump" />
export declare const x: number;
