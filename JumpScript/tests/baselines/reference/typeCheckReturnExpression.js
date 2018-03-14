//// [typeCheckReturnExpression.jump]
var foo = () => undefined;

//// [typeCheckReturnExpression.js]
var foo = function () { return undefined; };
