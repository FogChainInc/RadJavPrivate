//// [arrayAssignmentPatternWithAny.jump]
var a: any;
var x: string;
[x] = a;

//// [arrayAssignmentPatternWithAny.js]
var a;
var x;
x = a[0];
