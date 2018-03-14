//// [templateStringsWithTypeErrorInFunctionExpressionsInSubstitutionExpressionES6.jump]
`${function (x: number) { x = "bad"; } }`;

//// [templateStringsWithTypeErrorInFunctionExpressionsInSubstitutionExpressionES6.js]
`${function (x) { x = "bad"; }}`;
