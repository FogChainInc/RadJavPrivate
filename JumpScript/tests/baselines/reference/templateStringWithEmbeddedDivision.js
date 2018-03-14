//// [templateStringWithEmbeddedDivision.jump]
var x = `abc${ 1 / 1 }def`;

//// [templateStringWithEmbeddedDivision.js]
var x = "abc" + 1 / 1 + "def";
