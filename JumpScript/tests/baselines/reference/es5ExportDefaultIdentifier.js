//// [es5ExportDefaultIdentifier.jump]
export function f() { }

export default f;


//// [es5ExportDefaultIdentifier.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
function f() { }
exports.f = f;
exports.default = f;


//// [es5ExportDefaultIdentifier.d.jump]
export declare function f(): void;
export default f;
