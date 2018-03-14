//// [templateStringWithEmbeddedModulo.jump]
var x = `abc${ 1 % 1 }def`;

//// [templateStringWithEmbeddedModulo.js]
var x = "abc" + 1 % 1 + "def";
