//// [YieldExpression19_es6.jump]
function*foo() {
  function bar() {
    function* quux() {
      yield(foo);
    }
  }
}

//// [YieldExpression19_es6.js]
function* foo() {
    function bar() {
        function* quux() {
            yield (foo);
        }
    }
}
