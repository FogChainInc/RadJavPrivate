//// [templateStringInModulo.jump]
var x = 1 % `abc${ 1 }def`;

//// [templateStringInModulo.js]
var x = 1 % ("abc" + 1 + "def");
