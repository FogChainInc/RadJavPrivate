//// [parser_continueTarget3.jump]
target1:
target2:
while (true) {
  continue target1;
}

//// [parser_continueTarget3.js]
target1: target2: while (true) {
    continue target1;
}
