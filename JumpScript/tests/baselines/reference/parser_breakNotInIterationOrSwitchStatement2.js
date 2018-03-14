//// [parser_breakNotInIterationOrSwitchStatement2.jump]
while (true) {
  function f() {
    break;
  }
}

//// [parser_breakNotInIterationOrSwitchStatement2.js]
while (true) {
    function f() {
        break;
    }
}
