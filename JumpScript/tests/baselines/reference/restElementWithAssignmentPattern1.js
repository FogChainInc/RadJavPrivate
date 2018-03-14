//// [restElementWithAssignmentPattern1.jump]
var a: string, b: number;
[...[a, b = 0]] = ["", 1];

//// [restElementWithAssignmentPattern1.js]
var a, b;
[...[a, b = 0]] = ["", 1];
