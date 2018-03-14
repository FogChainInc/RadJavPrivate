//// [YieldExpression3_es6.jump]
function* foo() {
  yield
  yield
}

//// [YieldExpression3_es6.js]
function* foo() {
    yield;
    yield;
}
