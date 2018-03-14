//// [parser_breakTarget3.jump]
target1:
target2:
while (true) {
  break target1;
}

//// [parser_breakTarget3.js]
target1: target2: while (true) {
    break target1;
}
