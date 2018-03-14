//// [templateStringInConditional.jump]
var x = `abc${ " " }def` ? `abc${ " " }def` : `abc${ " " }def`;

//// [templateStringInConditional.js]
var x = "abc" + " " + "def" ? "abc" + " " + "def" : "abc" + " " + "def";
