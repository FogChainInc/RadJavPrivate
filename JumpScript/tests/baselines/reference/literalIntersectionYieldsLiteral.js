//// [literalIntersectionYieldsLiteral.jump]
const x: { type: string } & { type: "string" } = { type: "string" };


//// [literalIntersectionYieldsLiteral.js]
var x = { type: "string" };
