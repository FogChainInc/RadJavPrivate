//// [YieldExpression10_es6.jump]
var v = { * foo() {
    yield(foo);
  }
}


//// [YieldExpression10_es6.js]
var v = { *foo() {
        yield (foo);
    }
};
