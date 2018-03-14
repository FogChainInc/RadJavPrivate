//// [templateStringWithEmbeddedUnaryPlus.jump]
var x = `abc${ +Infinity }def`;

//// [templateStringWithEmbeddedUnaryPlus.js]
var x = "abc" + +Infinity + "def";
