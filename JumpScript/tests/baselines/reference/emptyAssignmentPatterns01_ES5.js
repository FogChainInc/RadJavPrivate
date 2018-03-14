//// [emptyAssignmentPatterns01_ES5.jump]
var a: any;

({} = a);
([] = a);

var [,] = [1,2];

//// [emptyAssignmentPatterns01_ES5.js]
var a;
(a);
(a);
var _a = [1, 2];


//// [emptyAssignmentPatterns01_ES5.d.jump]
declare var a: any;
