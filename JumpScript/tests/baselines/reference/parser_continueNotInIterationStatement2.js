//// [parser_continueNotInIterationStatement2.jump]
while (true) {
  function f() {
    continue;
  }
}

//// [parser_continueNotInIterationStatement2.js]
while (true) {
    function f() {
        continue;
    }
}
