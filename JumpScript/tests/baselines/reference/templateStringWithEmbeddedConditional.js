//// [templateStringWithEmbeddedConditional.jump]
var x = `abc${ true ? false : " " }def`;

//// [templateStringWithEmbeddedConditional.js]
var x = "abc" + (true ? false : " ") + "def";
