//// [parserArrowFunctionExpression2.jump]
a = () => { } || a

//// [parserArrowFunctionExpression2.js]
a = function () { };
 || a;
