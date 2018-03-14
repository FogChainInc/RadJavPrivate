//// [parserArrowFunctionExpression4.jump]
a = (() => { }, a)

//// [parserArrowFunctionExpression4.js]
a = (function () { }, a);
