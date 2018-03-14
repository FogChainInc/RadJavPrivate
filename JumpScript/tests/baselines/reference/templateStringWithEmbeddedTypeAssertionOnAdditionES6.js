//// [templateStringWithEmbeddedTypeAssertionOnAdditionES6.jump]
var x = `abc${ <any>(10 + 10) }def`;

//// [templateStringWithEmbeddedTypeAssertionOnAdditionES6.js]
var x = `abc${(10 + 10)}def`;
