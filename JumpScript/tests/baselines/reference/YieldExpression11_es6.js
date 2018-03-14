//// [YieldExpression11_es6.jump]
class C {
  *foo() {
    yield(foo);
  }
}

//// [YieldExpression11_es6.js]
class C {
    *foo() {
        yield (foo);
    }
}
