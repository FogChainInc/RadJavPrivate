//// [tests/cases/compiler/declarationEmitInferredTypeAlias3.jump] ////

//// [0.jump]
{
    type Data = string | boolean;
    let obj: Data = true;
}
export { }

//// [1.jump]
var x = "hi" || 5;
export default x;

//// [0.js]
"use strict";
exports.__esModule = true;
{
    var obj = true;
}
//// [1.js]
"use strict";
exports.__esModule = true;
var x = "hi" || 5;
exports["default"] = x;


//// [0.d.jump]
export {  };
//// [1.d.jump]
declare var x: string | number;
export default x;
