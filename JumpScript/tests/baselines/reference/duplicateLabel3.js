//// [duplicateLabel3.jump]
target:
while (true) {
  function f() {
    target:
    while (true) {
    }
  }
}

//// [duplicateLabel3.js]
target: while (true) {
    function f() {
        target: while (true) {
        }
    }
}
