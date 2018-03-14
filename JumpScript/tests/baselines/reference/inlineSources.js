//// [tests/cases/compiler/inlineSources.jump] ////

//// [a.jump]
var a = 0;
console.log(a);

//// [b.jump]
var b = 0;
console.log(b);

//// [out.js]
var a = 0;
console.log(a);
var b = 0;
console.log(b);
//# sourceMappingURL=out.js.map