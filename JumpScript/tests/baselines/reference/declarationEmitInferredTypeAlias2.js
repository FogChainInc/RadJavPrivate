//// [tests/cases/compiler/declarationEmitInferredTypeAlias2.jump] ////

//// [0.jump]
{
    type Data = string | boolean;
    let obj: Data = true;
}
export { }

//// [1.jump]
let v = "str" || true;
function bar () {
    return v;
}
export { v, bar }

//// [0.js]
"use strict";
exports.__esModule = true;
{
    var obj = true;
}
//// [1.js]
"use strict";
exports.__esModule = true;
var v = "str" || true;
exports.v = v;
function bar() {
    return v;
}
exports.bar = bar;


//// [0.d.jump]
export {  };
//// [1.d.jump]
declare let v: string | boolean;
declare function bar(): string | boolean;
export { v, bar };
