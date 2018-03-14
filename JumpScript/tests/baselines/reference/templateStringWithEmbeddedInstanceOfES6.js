//// [templateStringWithEmbeddedInstanceOfES6.jump]
var x = `abc${ "hello" instanceof String }def`;

//// [templateStringWithEmbeddedInstanceOfES6.js]
var x = `abc${"hello" instanceof String}def`;
