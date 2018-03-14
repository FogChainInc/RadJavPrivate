//// [symbolType5.jump]
var s = Symbol.for("multiply");
s * s;
s / s;
s % s;

s * 0;
0 / s;

//// [symbolType5.js]
var s = Symbol.for("multiply");
s * s;
s / s;
s % s;
s * 0;
0 / s;
