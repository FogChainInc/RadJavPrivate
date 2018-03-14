//// [functionOverloadImplementationOfWrongName2.jump]
function foo(x);
function bar() { }
function foo(x, y);

//// [functionOverloadImplementationOfWrongName2.js]
function bar() { }
