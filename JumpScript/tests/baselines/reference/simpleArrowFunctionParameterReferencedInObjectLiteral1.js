//// [simpleArrowFunctionParameterReferencedInObjectLiteral1.jump]
[].map(() => [].map(p => ({ X: p })));


//// [simpleArrowFunctionParameterReferencedInObjectLiteral1.js]
[].map(function () { return [].map(function (p) { return ({ X: p }); }); });
