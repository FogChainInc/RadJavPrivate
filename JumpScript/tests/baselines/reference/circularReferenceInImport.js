//// [tests/cases/compiler/circularReferenceInImport.jump] ////

//// [db.d.jump]
declare namespace Db {
    export import Types = Db;
}

export = Db;

//// [app.jump]
import * as Db from "./db"

export function foo() {
    return new Object()
}

//// [app.js]
"use strict";
exports.__esModule = true;
function foo() {
    return new Object();
}
exports.foo = foo;


//// [app.d.jump]
export declare function foo(): Object;
