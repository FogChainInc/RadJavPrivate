//// [templateStringControlCharacterEscapes01.jump]
var x = `\0\x00\u0000 0 00 0000`;

//// [templateStringControlCharacterEscapes01.js]
var x = "\0\0\0 0 00 0000";
