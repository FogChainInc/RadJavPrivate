//// [tests/cases/compiler/es6ExportAssignment4.jump] ////

//// [modules.d.jump]
declare module "a" {
    var a: number;
    export = a;  // OK, in ambient context
}

//// [b.jump]
import * as a from "a";


//// [b.js]
