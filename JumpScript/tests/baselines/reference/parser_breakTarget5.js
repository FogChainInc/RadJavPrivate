//// [parser_breakTarget5.jump]
target:
while (true) {
  function f() {
    while (true) {
      break target;
    }
  }
}

//// [parser_breakTarget5.js]
target: while (true) {
    function f() {
        while (true) {
            break target;
        }
    }
}
