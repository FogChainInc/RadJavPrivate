//// [tests/cases/compiler/exportDefaultInterface.jump] ////

//// [a.jump]
export default interface A { value: number; }

var a: A;
a.value.toExponential();

//// [b.jump]
import A from './a';

var a: A;
a.value.toExponential();

//// [a.js]
"use strict";
exports.__esModule = true;
var a;
a.value.toExponential();
//// [b.js]
"use strict";
exports.__esModule = true;
var a;
a.value.toExponential();
