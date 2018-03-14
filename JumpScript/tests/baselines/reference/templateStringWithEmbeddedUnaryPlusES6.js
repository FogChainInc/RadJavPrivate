//// [templateStringWithEmbeddedUnaryPlusES6.jump]
var x = `abc${ +Infinity }def`;

//// [templateStringWithEmbeddedUnaryPlusES6.js]
var x = `abc${+Infinity}def`;
