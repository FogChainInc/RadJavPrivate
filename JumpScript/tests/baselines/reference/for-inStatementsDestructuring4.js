//// [for-inStatementsDestructuring4.jump]
var a, b;
for ({a, b} in []) { }

//// [for-inStatementsDestructuring4.js]
var a, b;
for ({ a: a, b: b } in []) { }
