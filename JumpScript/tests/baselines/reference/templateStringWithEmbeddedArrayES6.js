//// [templateStringWithEmbeddedArrayES6.jump]
var x = `abc${ [1,2,3] }def`;

//// [templateStringWithEmbeddedArrayES6.js]
var x = `abc${[1, 2, 3]}def`;
