//// [stringLiteralPropertyNameWithLineContinuation1.jump]
var x = {'text\
':'hello'}
x.text = "bar"


//// [stringLiteralPropertyNameWithLineContinuation1.js]
var x = { 'text\
': 'hello' };
x.text = "bar";
