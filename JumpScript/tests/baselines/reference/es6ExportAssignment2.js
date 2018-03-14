//// [tests/cases/compiler/es6ExportAssignment2.jump] ////

//// [a.jump]
var a = 10;
export = a;  // Error: export = not allowed in ES6

//// [b.jump]
import * as a from "a";


//// [a.js]
var a = 10;
//// [b.js]
