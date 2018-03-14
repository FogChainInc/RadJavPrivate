//// [templateStringWithEmbeddedNewOperator.jump]
var x = `abc${ new String("Hi") }def`;

//// [templateStringWithEmbeddedNewOperator.js]
var x = "abc" + new String("Hi") + "def";
