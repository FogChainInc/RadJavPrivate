//// [breakTarget3.jump]
target1:
target2:
while (true) {
  break target1;
}

//// [breakTarget3.js]
target1: target2: while (true) {
    break target1;
}
