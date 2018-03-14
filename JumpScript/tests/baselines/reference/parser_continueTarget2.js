//// [parser_continueTarget2.jump]
target:
while (true) {
  continue target;
}

//// [parser_continueTarget2.js]
target: while (true) {
    continue target;
}
