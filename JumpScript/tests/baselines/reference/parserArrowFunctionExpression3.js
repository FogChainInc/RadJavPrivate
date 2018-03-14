//// [parserArrowFunctionExpression3.jump]
a = (() => { } || a)

//// [parserArrowFunctionExpression3.js]
a = (function () { }) || a;
