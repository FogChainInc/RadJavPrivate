//// [templateStringWithEmbeddedObjectLiteralES6.jump]
var x = `abc${ { x: 10, y: 20 } }def`;

//// [templateStringWithEmbeddedObjectLiteralES6.js]
var x = `abc${{ x: 10, y: 20 }}def`;
