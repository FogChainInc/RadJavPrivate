//// [breakTarget5.jump]
target:
while (true) {
  function f() {
    while (true) {
      break target;
    }
  }
}

//// [breakTarget5.js]
target: while (true) {
    function f() {
        while (true) {
            break target;
        }
    }
}
