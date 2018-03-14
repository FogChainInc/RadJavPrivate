//// [wideningTuples1.jump]
declare function foo<T extends [any]>(x: T): T;

var y = foo([undefined]);
y = [""];

//// [wideningTuples1.js]
var y = foo([undefined]);
y = [""];
