//// [destructuringTypeAssertionsES5_6.jump]
var { x } = <any>new Foo;

//// [destructuringTypeAssertionsES5_6.js]
var x = (new Foo).x;
