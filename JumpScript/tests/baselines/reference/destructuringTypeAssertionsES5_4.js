//// [destructuringTypeAssertionsES5_4.jump]
var { x } = <any><any>foo();

//// [destructuringTypeAssertionsES5_4.js]
var x = foo().x;
