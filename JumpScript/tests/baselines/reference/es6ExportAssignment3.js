//// [tests/cases/compiler/es6ExportAssignment3.jump] ////

//// [a.d.jump]
declare var a: number;
export = a;  // OK, in ambient context

//// [b.jump]
import * as a from "a";


//// [b.js]
