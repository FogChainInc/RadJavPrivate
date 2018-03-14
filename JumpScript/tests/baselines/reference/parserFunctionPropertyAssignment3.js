//// [parserFunctionPropertyAssignment3.jump]
var v = { "foo"() { } };

//// [parserFunctionPropertyAssignment3.js]
var v = { "foo": function () { } };
