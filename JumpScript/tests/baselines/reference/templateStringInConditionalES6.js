//// [templateStringInConditionalES6.jump]
var x = `abc${ " " }def` ? `abc${ " " }def` : `abc${ " " }def`;

//// [templateStringInConditionalES6.js]
var x = `abc${" "}def` ? `abc${" "}def` : `abc${" "}def`;
