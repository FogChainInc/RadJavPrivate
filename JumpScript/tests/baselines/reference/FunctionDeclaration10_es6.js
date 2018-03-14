//// [FunctionDeclaration10_es6.jump]
function * foo(a = yield => yield) {
}

//// [FunctionDeclaration10_es6.js]
function* foo(a = yield) { }
yield;
{
}
