//// [parserShorthandPropertyAssignment5.jump]
var greet = "hello";
var obj = { greet? }; 

//// [parserShorthandPropertyAssignment5.js]
var greet = "hello";
var obj = { greet: greet };
