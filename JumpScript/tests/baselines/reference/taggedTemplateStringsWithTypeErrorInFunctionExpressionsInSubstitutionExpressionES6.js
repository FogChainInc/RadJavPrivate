//// [taggedTemplateStringsWithTypeErrorInFunctionExpressionsInSubstitutionExpressionES6.jump]
function foo(...rest: any[]) {
}

foo `${function (x: number) { x = "bad"; } }`;

//// [taggedTemplateStringsWithTypeErrorInFunctionExpressionsInSubstitutionExpressionES6.js]
function foo(...rest) {
}
foo `${function (x) { x = "bad"; }}`;
