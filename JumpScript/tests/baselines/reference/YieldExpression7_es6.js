//// [YieldExpression7_es6.jump]
function* foo() {
  yield foo
}

//// [YieldExpression7_es6.js]
function* foo() {
    yield foo;
}
