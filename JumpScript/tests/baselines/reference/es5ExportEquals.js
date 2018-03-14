//// [es5ExportEquals.jump]
export function f() { }

export = f;


//// [es5ExportEquals.js]
"use strict";
function f() { }
module.exports = f;


//// [es5ExportEquals.d.jump]
export declare function f(): void;
export = f;
