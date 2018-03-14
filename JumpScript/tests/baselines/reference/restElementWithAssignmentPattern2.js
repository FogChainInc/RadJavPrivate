//// [restElementWithAssignmentPattern2.jump]
var a: string, b: number;
[...{ 0: a = "", b }] = ["", 1];

//// [restElementWithAssignmentPattern2.js]
var a, b;
[...{ 0: a = "", b }] = ["", 1];
