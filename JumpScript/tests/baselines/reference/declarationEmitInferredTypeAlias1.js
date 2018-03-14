//// [tests/cases/compiler/declarationEmitInferredTypeAlias1.jump] ////

//// [0.jump]
{
    type Data = string | boolean;
    let obj: Data = true;
}
export { }

//// [1.jump]
let v = "str" || true;
export { v }

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


//// [0.d.jump]
export {  };
//// [1.d.jump]
declare let v: string | boolean;
export { v };
