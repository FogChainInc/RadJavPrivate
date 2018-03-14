//// [parser_duplicateLabel2.jump]
target:
while (true) {
  target:
  while (true) {
  }
}

//// [parser_duplicateLabel2.js]
target: while (true) {
    target: while (true) {
    }
}
