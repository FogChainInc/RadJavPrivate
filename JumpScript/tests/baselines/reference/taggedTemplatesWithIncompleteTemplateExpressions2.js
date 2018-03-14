//// [taggedTemplatesWithIncompleteTemplateExpressions2.jump]
function f(x: TemplateStringsArray, y: string, z: string) {
}

// Incomplete call, enough parameters.
f `123qdawdrqw${ }${

//// [taggedTemplatesWithIncompleteTemplateExpressions2.js]
function f(x, y, z) {
}
// Incomplete call, enough parameters.
f `123qdawdrqw${}${;
