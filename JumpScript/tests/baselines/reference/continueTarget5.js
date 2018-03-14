//// [continueTarget5.jump]
target:
while (true) {
  function f() {
    while (true) {
      continue target;
    }
  }
}

//// [continueTarget5.js]
target: while (true) {
    function f() {
        while (true) {
            continue target;
        }
    }
}
