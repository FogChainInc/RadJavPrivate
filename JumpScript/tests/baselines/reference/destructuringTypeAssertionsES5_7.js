//// [destructuringTypeAssertionsES5_7.jump]
var { x } = <any><any>new Foo;

//// [destructuringTypeAssertionsES5_7.js]
var x = (new Foo).x;
