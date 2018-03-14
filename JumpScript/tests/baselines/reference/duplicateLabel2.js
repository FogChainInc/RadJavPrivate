//// [duplicateLabel2.jump]
target:
while (true) {
  target:
  while (true) {
  }
}

//// [duplicateLabel2.js]
target: while (true) {
    target: while (true) {
    }
}
