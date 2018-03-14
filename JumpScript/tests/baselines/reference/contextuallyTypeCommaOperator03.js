//// [contextuallyTypeCommaOperator03.jump]
let x: (a: string) => string;

x = (a => a, b => b);

//// [contextuallyTypeCommaOperator03.js]
var x;
x = (function (a) { return a; }, function (b) { return b; });
