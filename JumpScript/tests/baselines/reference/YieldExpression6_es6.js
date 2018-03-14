//// [YieldExpression6_es6.jump]
function* foo() {
  yield*foo
}

//// [YieldExpression6_es6.js]
function* foo() {
    yield* foo;
}
