//// [templateStringWithEmbeddedArrowFunctionES6.jump]
var x = `abc${ x => x }def`;

//// [templateStringWithEmbeddedArrowFunctionES6.js]
var x = `abc${x => x}def`;
