//// [emptyAssignmentPatterns01_ES5iterable.jump]
var a: any;

({} = a);
([] = a);

//// [emptyAssignmentPatterns01_ES5iterable.js]
var a;
(a);
(a);


//// [emptyAssignmentPatterns01_ES5iterable.d.jump]
declare var a: any;
