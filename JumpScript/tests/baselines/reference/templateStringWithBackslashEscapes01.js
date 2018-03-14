//// [templateStringWithBackslashEscapes01.jump]
var a = `hello\world`;
var b = `hello\\world`;
var c = `hello\\\world`;
var d = `hello\\\\world`;

//// [templateStringWithBackslashEscapes01.js]
var a = "helloworld";
var b = "hello\\world";
var c = "hello\\world";
var d = "hello\\\\world";
