//// [tests/cases/compiler/umdGlobalConflict.jump] ////

//// [index.d.jump]
export as namespace Alpha;
export var x: string;

//// [index.d.jump]
export as namespace Alpha;
export var y: number;

//// [consumer.jump]
import * as v1 from './v1';
import * as v2 from './v2';

//// [global.jump]
// Should be OK, first in wins
const p: string = Alpha.x;

//// [consumer.js]
"use strict";
exports.__esModule = true;
//// [global.js]
// Should be OK, first in wins
var p = Alpha.x;
