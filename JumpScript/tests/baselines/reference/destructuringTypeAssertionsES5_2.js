//// [destructuringTypeAssertionsES5_2.jump]
var { x } = (<any>foo());

//// [destructuringTypeAssertionsES5_2.js]
var x = foo().x;
