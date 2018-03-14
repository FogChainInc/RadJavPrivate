//// [conditionalExpression1.jump]
var x: boolean = (true ? 1 : ""); // should be an error

//// [conditionalExpression1.js]
var x = (true ? 1 : ""); // should be an error
