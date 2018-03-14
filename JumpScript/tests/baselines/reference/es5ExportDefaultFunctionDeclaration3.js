//// [es5ExportDefaultFunctionDeclaration3.jump]
var before: typeof func = func();

export default function func(): typeof func {
    return func;
}

var after: typeof func = func();

//// [es5ExportDefaultFunctionDeclaration3.js]
"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
var before = func();
function func() {
    return func;
}
exports.default = func;
var after = func();


//// [es5ExportDefaultFunctionDeclaration3.d.jump]
export default function func(): typeof func;
